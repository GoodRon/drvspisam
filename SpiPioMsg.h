/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef SPIPIOMSG_H
#define SPIPIOMSG_H

#include "SpiDeviceMsg.h"

/**
 * @brief Пространство имен для работы с устройствами на шине SPI
 **/
namespace spi {

/**
 * @brief Класс сообщения от/для PIO
 */
class SpiPioMsg : public SpiDeviceMsg {
public:
	/**
	 * @brief Перечисление используемых PIN'ов
	 */
	enum Pin {
		/**
		 * @brief PIN тревожной кнопки
		 */
		pinAlarmButton = 1,

		/**
		 * @brief PIN зажигания
		 */
		pinIgnition = 4
	};

	/**
	 * @brief Конструктор
	 */
	SpiPioMsg();

	/**
	 * @brief Конструктор сообщения для установки PIN'ов
	 *
	 * @param pins
	 */
	SpiPioMsg(uint8_t pins);

	/**
	 * @overload
	 */
	virtual ~SpiPioMsg();

	/**
	 * @overload
	 */
	virtual SpiDeviceData makeSpiData();

	/**
	 * @overload
	 */
	virtual bool parseSpiData(const SpiDeviceData& packet);

	/**
	 * @brief Проверить PIN
	 *
	 * @param pin
	 * @return bool true - установлен, false - не установлен
	 */
	bool checkPin(Pin pin) const;

	/**
	 * @brief Вернуть PIN'ы
	 *
	 * @return uint8_t
	 */
	uint8_t getPins() const;
};

}

#endif // SPIPIOMSG_H
