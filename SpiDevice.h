/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef SPIDEVICE_H
#define SPIDEVICE_H

#include <string>
#include <vector>

/**
 * @brief Пространство имен для работы с устройствами на шине SPI
 **/
namespace spi {

/**
 * @brief Алиас для вектора байт
 **/
typedef std::vector<uint8_t> SpiBytes;

/**
 * @brief Перечисление идентификаторов источников данных на шине SPI
 **/
enum SpiSource {
	/**
	 * @brief SAM
	 **/
	sdSam = 0,

	/**
	 * @brief USART0 (навигационный приемник)
	 **/
	sdUsart0 = 1,

	/**
	 * @brief USART1 (термопринтер)
	 **/
	sdUsart1 = 2,

	/**
	 * @brief Отладочный com-порт
	 **/
	sdDbgu = 3,

	/**
	 * @brief Маска управления питанием
	 **/
	sdPwrMask = 4,

	/**
	 * @brief Управление питанием
	 **/
	sdPwrCtrl = 5,

	/**
	 * @brief Устройства АЦП
	 **/
	sdAdc = 6,

	/**
	 * @brief PIO
	 **/
	sdPio = 7
};

/**
 * @brief UNIX сокет для USART0 (дефолтно навигационный приемник)
 **/
const std::string usart0Device = "/opt/flash/USART0";

/**
 * @brief UNIX сокет для USART1 (дефолтно принтер)
 **/
const std::string usart1Device = "/opt/flash/USART1";

/**
 * @brief UNIX сокет для DBGU
 **/
const std::string dbguDevice = "/opt/flash/DBGU";

/**
 * @brief UNIX сокет для ADC
 **/
const std::string adcDevice = "/opt/flash/ADC";

/**
 * @brief UNIX сокет для PIO
 **/
const std::string pioDevice = "/opt/flash/PIO";

/**
 * @brief UNIX сокет для SAM
 **/
const std::string samSpiDevice = "/dev/spidev0.2";

/**
 * @brief Строка таблицы соответствия идентификаторов устройств сокетам в файловой
 * системе
 **/
struct DeviceTableRow {
	/**
	 * @brief Идентификатор источника данных на шине SPI
	 **/
	SpiSource device;

	/**
	 * @brief Путь к UNIX сокету в файловой системе
	 **/
	std::string socketPath;
};

/**
 * @brief Строка таблица соответствия идентификаторов устройств сокетам в файловой
 * системе
 **/
static DeviceTableRow deviceTable[] = {
	{sdUsart0, usart0Device},
	{sdUsart1, usart1Device},
	{sdDbgu, dbguDevice},
	{sdAdc, adcDevice},
	{sdPio, pioDevice}
};

/**
 * @brief Перечисление типов SPI пакетов
 **/
enum SpiPacketType {
	/**
	 * @brief Данные
	 **/
	sptData = 0x00,

	/**
	 * @brief Комманда
	 **/
	sptCommand = 0x80
};

/**
 * @brief Перечисление заголовков SPI пакетов
 **/
enum SpiPacketHeader {
	/**
	 * @brief Группа пакетов
	 **/
	sphGroup = 0xEA,

	/**
	 * @brief Одиночный пакет
	 **/
	sphSingle = 0xEE
};

/**
 * @brief Перечисление скоростей для настройки UART'ов SAM'а
 **/
enum SamComSpeed {
	s50 = 1,
	s75,
	s110,
	s134,
	s150,
	s200,
	s300,
	s600,
	s1200,
	s1800,
	s2400,
	s4800,
	s9600,
	s19200,
	s38400,
	s57600,
	s115200
};

}

#endif // SPIDEVICE_H
