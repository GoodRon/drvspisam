/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef SPINAVIGATORMSG_H
#define SPINAVIGATORMSG_H

#include "SpiDeviceMsg.h"

#include <string>

/**
 * @brief Пространство имен для работы с устройствами на шине SPI
 **/
namespace spi {

/**
 * @brief Класс сообщения от/для навигационного приемника
 */
class SpiNavigatorMsg : public SpiDeviceMsg {
public:
	/**
	 * @brief Конструктор
	 */
	SpiNavigatorMsg();

	/**
	 * @brief Конструктор сообщения для установки скорости UART'а между SAM'ом и навигационным
	 * приемником
	 *
	 * @param speed скорость
	 */
	SpiNavigatorMsg(SamComSpeed speed);

	/**
	 * @overload
	 */
	virtual ~SpiNavigatorMsg();

	/**
	 * @overload
	 */
	virtual SpiDeviceData makeSpiData();

	/**
	 * @overload
	 */
	virtual bool parseSpiData(const SpiDeviceData& packet);
};

}

#endif // SPINAVIGATORMSG_H
