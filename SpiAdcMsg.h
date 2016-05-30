/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef SPIADCMSG_H
#define SPIADCMSG_H

#include "SpiDeviceMsg.h"

/**
 * @brief Пространство имен для работы с устройствами на шине SPI
 **/
namespace spi {

/**
 * @brief Класс сообщения от/для устройства АЦП
 **/
class SpiAdcMsg : public SpiDeviceMsg {
public:
	/**
	 * @brief Конструктор
	 */
	SpiAdcMsg();

	/**
	 * @overload
	 */
	virtual ~SpiAdcMsg();

	/**
	 * @overload
	 */
	virtual SpiDeviceData makeSpiData();

	/**
	 * @overload
	 */
	virtual bool parseSpiData(const SpiDeviceData& packet);

	/**
	 * @brief Вернуть значение 1 канала АЦП
	 */
	int getAdcChannel1() const;

	/**
	 * @brief Вернуть значение 2 канала АЦП
	 */
	int getAdcChannel2() const;

	/**
	 * @brief Вернуть значение 3 канала АЦП
	 */
	int getAdcChannel3() const;

	/**
	 * @brief Вернуть значение 4 канала АЦП
	 */
	int getAdcChannel4() const;

	/**
	 * @brief Вернуть значение резервного питания в вольтах
	 */
	double getReservePower() const;

	/**
	 * @brief Вернуть значение основного питания в вольтах
	 */
	double getMainPower() const;
};

}

#endif // SPIADCMSG_H
