/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <fcntl.h>
#include <stropts.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <linux/spi/spidev.h>

#include "spi.h"

#include <iostream>

using namespace std;

namespace spi {

int initSpi(const string& device, uint8_t mode, uint8_t bits, long speed) {
	int fd = open(device.c_str(), O_RDWR);
	if (fd < 0) {
		return fd;
	}

	// Задаем режим работы SPI
	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
		return -1;
	}

	if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) {
		return -1;
	}

	// Задаем количество бит в слове
	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
		return -1;
	}

	if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
		return -1;
	}

	// Задаем скорость обмена (Гц)
	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
		return -1;
	}

	if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
		return -1;
	}

	return fd;
}

int closeSpi(int spiFd) {
	return close(spiFd);
}

int transferSpiBuffers(int spiFd, uint8_t* wbuf, uint8_t* rbuf, size_t size) {
	struct spi_ioc_transfer transfer;
	transfer.tx_buf = reinterpret_cast<__u64>(wbuf);
	transfer.rx_buf = reinterpret_cast<__u64>(rbuf);
	transfer.len = size;
	transfer.delay_usecs = 0;
	transfer.speed_hz = 0;
	transfer.bits_per_word = 0;

	return ioctl(spiFd, SPI_IOC_MESSAGE(1), &transfer);
}

vector<SpiDeviceData> selectSpiPackets(SpiPacketHeader header, const SpiBytes& rawData,
									   SpiBytes& residure) {
	const size_t headerLenght = 4;
	vector<SpiDeviceData> packets;
	size_t pos = 0;

	size_t rawDataSize = rawData.size();

	while (pos < rawDataSize) {
		// Ищем начало пакета
		for (; pos < rawDataSize; ++pos) {
			if (rawData[pos] == header) {
				break;
			}
		}

		if (pos >= rawDataSize) {
			break;
		}

		if (pos + 3 >= rawDataSize) {
			residure = move(SpiBytes(rawData.begin() + pos, rawData.end()));
			break;
		}

		// Вычисляем длину всего пакета
		size_t length;
		length = (rawData[pos + 2] | rawData[pos + 3] << 8) + headerLenght;
		if (pos + length > rawDataSize) {
			residure = move(SpiBytes(rawData.begin() + pos, rawData.end()));
			break;
		}

		// Отделяем пакет от шума
		packets.emplace_back(static_cast<SpiSource>(rawData[pos + 1]),
				sptData, SpiBytes(rawData.begin() + pos + headerLenght,
								  rawData.begin() + pos + length));
		pos += length;
	}
	return packets;
}

vector<SpiDeviceData> selectSpiPackets(SpiPacketHeader header, const SpiBytes& rawData) {
	const size_t headerLength = 4;
	vector<SpiDeviceData> packets;
	size_t pos = 0;

	size_t rawDataSize = rawData.size();

	while (pos < rawDataSize) {
		// Ищем начало пакета
		for (; pos < rawDataSize; ++pos) {
			if (rawData[pos] == header) {
				break;
			}
		}

		if (pos + 3 >= rawDataSize) {
			break;
		}

		// Вычисляем длину всего пакета
		size_t length;
		length = (rawData[pos + 2] | rawData[pos + 3] << 8) + headerLength;
		if (pos + length > rawDataSize) {
			break;
		}

		// Отделяем пакет от шума
		packets.emplace_back(static_cast<SpiSource>(rawData[pos + 1]),
				sptData, SpiBytes(rawData.begin() + pos + headerLength,
								  rawData.begin() + pos + length));
		pos += length;
	}
	return packets;
}

vector<SpiDeviceData> selectSpiPacketsLazily(SpiPacketHeader header, const SpiBytes& rawData,
											 size_t length) {
	const size_t headerLength = 4;
	vector<SpiDeviceData> packets;
	size_t pos = 0;

	size_t rawDataSize = (length != 0) && (rawData.size() > length) ? length : rawData.size();

	while (pos < rawDataSize) {
		// Ищем начало пакета
		for (; pos < rawDataSize; ++pos) {
			if (rawData[pos] == 0xFF) {
				return packets;
			}

			if (rawData[pos] == header) {
				break;
			}
		}

		if (pos + 3 >= rawDataSize) {
			break;
		}

		// Вычисляем длину всего пакета
		size_t length;
		length = (rawData[pos + 2] | rawData[pos + 3] << 8) + headerLength;
		if (pos + length > rawDataSize) {
			break;
		}

		// Отделяем пакет от шума
		packets.emplace_back(static_cast<SpiSource>(rawData[pos + 1]),
				sptData, SpiBytes(rawData.begin() + pos + headerLength,
								  rawData.begin() + pos + length));
		pos += length;
	}
	return packets;
}

}
