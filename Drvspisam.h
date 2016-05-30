/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef DRVSPISAM_H
#define DRVSPISAM_H

#include <string>
#include <map>

#include "spi.h"

/**
 * @brief Класс приложения для управления сопроцессором SAM
 */
class Drvspisam {
public:
	/**
	 * @brief Конструктор
	 *
	 * @param spi путь к представлению шины SPI в файловой системе
	 * @param mode режим работы
	 * @param bits количество бит на слово
	 * @param speed скорость обмена данными
	 */
	Drvspisam(const std::string& spi, uint8_t mode, uint8_t bits, long speed);

	/**
	 * @brief Деструктор
	 */
	virtual ~Drvspisam();

	/**
	 * @brief Основной цикл выполнения
	 */
	void run();

private:
	/**
	 * @brief Создать и настроить сокеты для всех устройств на шине SPI
	 */
	void createChannels();

	/**
	 * @brief Деинициализировать отключенных клиентов
	 */
	void disconnectClosedClients();

	/**
	 * @brief Обработать набор дескрипторов для чтения
	 */
	void readFds(const fd_set& rfds);

	/**
	 * @brief Обработать набор дескрипторов для записи
	 */
	void writeFds(const fd_set& wfds);

private:
	/**
	 * @brief Максимальный размер очереди пакетов
	 */
	static const int m_queueSize = 3;

	/**
	 * @brief Фиксированный размер блока данных для обмена с сопроцессором SAM по SPI
	 * ВНИМАНИЕ! Изменение этой константы может и гарантированно приведет к неопределенному
	 * поведению программного кода прошивки SAM
	 */
	static const size_t m_spiBlock = 3128;

	/**
	 * @brief Структура клиента
	 */
	struct Client {
		/**
		 * @brief Конструктор
		 */
		Client(): socket(-1), isAlive(false), lastPacket() {}

		/**
		 * @brief Открытый сокет
		 */
		int socket;

		/**
		 * @brief Признак активности клиента
		 */
		bool isAlive;

		/**
		 * @brief Последний переданный пакет
		 */
		spi::SpiBytes lastPacket;
	};

	/**
	 * @brief Структура канала данных от устройства на шине SPI
	 */
	struct Channel {
		/**
		 * @brief Конструктор
		 */
		Channel(): acceptSocket(-1), queue(), clients(), isNeedCleanup(false) {}

		/**
		 * @brief Сокет для регистрации клиентов
		 */
		int acceptSocket;

		/**
		 * @brief Очередь пакетов для клиентов
		 */
		std::vector<spi::SpiDeviceData> queue;

		/**
		 * @brief Пул клиентов
		 */
		std::vector<Client> clients;

		/**
		 * @brief Признак наличия отключенных клиентов в пуле клиентов
		 */
		bool isNeedCleanup;
	};

	/**
	 * @brief Сокет для общения с SAM по SPI
	 */
	int m_samSocket;

	/**
	 * @brief Мэп соответствия источников данных каналам
	 */
	std::map<spi::SpiSource, Channel> m_channels;

	/**
	 * @brief Очередь пакетов для передачи в SAM
	 */
	std::vector<spi::SpiDeviceData> m_spiQueue;

	/**
	 * @brief Признак заполнения буфера передачи в SAM
	 */
	bool m_isWriteBufEmpty;

	/**
	 * @brief Буфер для чтения данных от SAM
	 */
	spi::SpiBytes m_readBuf;

	/**
	 * @brief Буфер для записи данных в SAM
	 */
	spi::SpiBytes m_writeBuf;
};

#endif // DRVSPISAM_H
