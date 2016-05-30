/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <syslog.h>
#include <string>
#include <algorithm>
#include <fcntl.h>
#include <stropts.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/spi/spidev.h>

#include "Drvspisam.h"

using namespace std;
using namespace spi;

/**
 * @brief Флаг работы процесса
 */
static bool m_isRun = true;

/**
 * @brief Обработчик сигнала завершения работы
 */
static void terminationHdl(int signal) {
	syslog(LOG_ERR, "signal %d!", signal);
	m_isRun = false;
}

Drvspisam::Drvspisam(const std::string& spi, uint8_t mode, uint8_t bits, long speed):
	m_samSocket(-1),
	m_channels(),
	m_spiQueue(),
	m_isWriteBufEmpty(true),
	m_readBuf(),
	m_writeBuf() {

	m_samSocket = initSpi(spi, mode, bits, speed);

	m_readBuf.resize(m_spiBlock, 0xFF);
	m_writeBuf.resize(m_spiBlock, 0xFF);

	openlog("drvspisam", LOG_CONS, LOG_LOCAL3);
	setlogmask(LOG_UPTO(LOG_ERR));

	signal(SIGTERM, terminationHdl);
	signal(SIGPIPE, SIG_IGN);
}

Drvspisam::~Drvspisam() {
	closeSpi(m_samSocket);

	for (auto& pair: m_channels) {
		Channel& channel = pair.second;
		close(channel.acceptSocket);

		for (auto& client: channel.clients) {
			close(client.socket);
		}
	}

	syslog(LOG_ERR, "Bye!");
	closelog();
}

void Drvspisam::run() {
	if (m_samSocket < 0) {
		syslog(LOG_ERR, "SAM socket is broken! Drvspisam was terminated");
		return;
	}

	struct timeval timer = {};
	fd_set rfds;
	fd_set wfds;

	createChannels();

	while (m_isRun) {
		int maxSocket = m_samSocket;

		FD_ZERO(&rfds);
		FD_ZERO(&wfds);

		FD_SET(m_samSocket, &rfds);
		FD_SET(m_samSocket, &wfds);

		for (auto& pair: m_channels) {
			Channel& channel = pair.second;
			FD_SET(channel.acceptSocket, &rfds);

			if (channel.acceptSocket > maxSocket) {
				maxSocket = channel.acceptSocket;
			}

			for (auto& client: channel.clients) {
				FD_SET(client.socket, &wfds);
				FD_SET(client.socket, &rfds);

				if (client.socket > maxSocket) {
					maxSocket = client.socket;
				}
			}
		}

		timer = {1, 0};

		int ret = select(maxSocket + 1, &rfds, &wfds, nullptr, &timer);
		if (ret > 0) {
			readFds(rfds);
			writeFds(wfds);
			disconnectClosedClients();
		}

		if (ret == 0) {
			syslog(LOG_DEBUG, "timeout on select()!");
		}

		if (ret < 0) {
			if (errno == EINTR) {
				continue;
			}
			syslog(LOG_ERR, "error on select(): %s", strerror(errno));
		}

		// Искусственная задержка. Дескриптор SPI всегда открыт для чтения, при том,
		// что пакетов с данными там может и не быть. Уменьшаем нагрузку на процессор
		usleep(100000);
	}
}

void Drvspisam::createChannels() {
	for (auto& row: deviceTable) {
		if (access(row.socketPath.c_str(), F_OK) == 0) {
			unlink(row.socketPath.c_str());
		}

		int sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (sockFd == -1) {
			syslog(LOG_ERR, "Can't create socket: %s", strerror(errno));
			continue;
		}

		struct sockaddr_un serv_addr;
		bzero(reinterpret_cast<char*>(&serv_addr), sizeof(serv_addr));
		serv_addr.sun_family = AF_UNIX;
		strcpy(serv_addr.sun_path, row.socketPath.c_str());
		int servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

		if (::bind(sockFd, reinterpret_cast<struct sockaddr*>(&serv_addr), servlen) < 0) {
			syslog(LOG_ERR, "Can't bind socket: %s", strerror(errno));
			close(sockFd);
			continue;
		}

		if (listen(sockFd, 1024) < 0) {
			syslog(LOG_ERR, "Can't listen socket: %s", strerror(errno));
			close(sockFd);
			continue;
		}

		Channel channel;
		channel.acceptSocket = sockFd;
		m_channels[row.device] = move(channel);

		syslog(LOG_DEBUG, "Created channel for %s", row.socketPath.c_str());
	}
}

void Drvspisam::disconnectClosedClients() {
	// Удаляем отвалившихся клиентов
	for (auto& pair: m_channels) {
		Channel& channel = pair.second;
		if (!channel.isNeedCleanup) {
			continue;
		}

		auto iterator = remove_if(channel.clients.begin(), channel.clients.end(),
							[](const Client& client)->bool{
			return !client.isAlive;
		});
		channel.clients.erase(iterator, channel.clients.end());
		channel.isNeedCleanup = false;

		// Если клиентов у канала больше нет, очищаем очередь пакетов
		if (channel.clients.empty()) {
			channel.queue.clear();
		}
	}
}

void Drvspisam::readFds(const fd_set& rfds) {
	// Работаем со всеми каналами данных
	for (auto& pair: m_channels) {
		Channel& channel = pair.second;

		// Читаем данные от клиентов
		for (auto& client: channel.clients) {
			if (!client.isAlive || !FD_ISSET(client.socket, &rfds)) {
				continue;
			}

			int length = read(client.socket, &m_readBuf[0], m_spiBlock);
			if (length <= 0) {
				syslog(LOG_DEBUG, "Client was disconnected");

				close(client.socket);
				client.isAlive = false;
				channel.isNeedCleanup = true;
				continue;
			}

			vector<SpiDeviceData> packets = move(selectSpiPacketsLazily(sphSingle, m_readBuf,
																		length));
			m_spiQueue.insert(m_spiQueue.end(), packets.begin(), packets.end());
		}

		// Принимаем новые соединения
		if (FD_ISSET(channel.acceptSocket, &rfds)) {
			struct sockaddr_un client_addr;
			size_t len = sizeof(client_addr);
			int sockFd = accept(channel.acceptSocket, (struct sockaddr*)&client_addr,
								reinterpret_cast<socklen_t*>(&len));

			if (sockFd < 0) {
				continue;
			}

			Client client;
			client.socket = sockFd;
			client.isAlive = true;
			channel.clients.push_back(move(client));

			syslog(LOG_DEBUG, "Client was connected");
		}
	}

	// Читаем данные от SAM
	if (FD_ISSET(m_samSocket, &rfds)) {
		// Отправляем данные только если они есть
		uint8_t* outBuf = nullptr;
		if (!m_isWriteBufEmpty) {
			m_isWriteBufEmpty = true;
			outBuf = &m_writeBuf[0];
		}

		// Полудуплексный обмен данными
		int length = transferSpiBuffers(m_samSocket, outBuf, &m_readBuf[0], m_spiBlock);
		if (length > 0) {
			vector<SpiDeviceData> bigPackets = move(selectSpiPacketsLazily(sphGroup, m_readBuf,
																		   length));
			for (auto& bigPacket: bigPackets) {
				// Разделяем пакеты по устройствам
				vector<SpiDeviceData> packets = move(selectSpiPacketsLazily(sphSingle,
																			bigPacket.data));

				for (auto& packet: packets) {
					// Проверяем, есть ли канал для пакетов от этого устройства
					auto iterator = m_channels.find(packet.source);
					if (iterator == m_channels.end()) {
						continue;
					}

					auto& channel = iterator->second;
					// Игнорируем канал, если у него нет клиентов
					if (channel.clients.empty()) {
						continue;
					}

					// Записываем данные в очередь
					channel.queue.push_back(move(packet));

					int delta = channel.queue.size() - m_queueSize;
					if (delta > 0) {
						channel.queue.erase(channel.queue.begin(), channel.queue.begin() + delta);
					}
				}
			}
		}
	}
}

void Drvspisam::writeFds(const fd_set& wfds) {
	// Передаем пакеты клиентам
	for (auto& pair: m_channels) {
		Channel& channel = pair.second;

		if (channel.queue.empty()) {
			continue;
		}

		for (auto& client: channel.clients) {
			if (!client.isAlive || !FD_ISSET(client.socket, &wfds)) {
				continue;
			}

			ssize_t ret = 0;
			for (auto& packet: channel.queue) {
				if (pair.first == sdUsart0) {
					string message;
					for (size_t i = 0; i < packet.data.size(); ++i) {
						message.push_back(static_cast<char>(packet.data[i]));
					}
					ret = write(client.socket, &message[0], message.size());
				} else {
					SpiBytes message = move(packet.makeRawPacket());
					if (client.lastPacket == message) {
						continue;
					}

					ret = write(client.socket, &message[0], message.size());
					client.lastPacket = move(message);
				}

				if (ret <= 0) {
					syslog(LOG_DEBUG, "Client was disconnected");

					close(client.socket);
					client.isAlive = false;
					channel.isNeedCleanup = true;
					break;
				}
			}
		}
		channel.queue.clear();
	}

	// Пишем в SAM
	// Можем потерять пакеты, которым не хватило места в буффере. При необходимости доработать
	if (!m_spiQueue.empty() && (m_isWriteBufEmpty) && FD_ISSET(m_samSocket, &wfds)) {
		m_isWriteBufEmpty = false;
		size_t writeIndex = 0;

		memset(&m_writeBuf[0], 0xFF, m_spiBlock);

		for (auto& packet: m_spiQueue) {
			SpiDeviceData bigPacket(sdSam, sptData, move(packet.makeRawPacket()));
			SpiBytes raw = move(bigPacket.makeRawPacket(sphGroup));

			size_t rawLength = raw.size();
			if (writeIndex + rawLength > m_spiBlock) {
				break;
			}

			for (size_t ri = 0; (ri < rawLength) && (writeIndex < m_spiBlock); ++ri, ++writeIndex) {
				m_writeBuf[writeIndex] = raw[ri];
			}
		}
		m_spiQueue.clear();
	}
}
