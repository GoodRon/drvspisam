/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef SPIDEVICEMSG_H
#define SPIDEVICEMSG_H

#include "SpiDeviceData.h"

/**
 * @brief Пространство имен для работы с устройствами на шине SPI
 **/
namespace spi {

/**
 * @brief Абстрактный класс сообщения от/для устройства на шине SPI
 */
class SpiDeviceMsg {
public:
	/**
	 * @brief Конструктор
	 *
	 * @param device идентификатор устройства
	 */
	SpiDeviceMsg(SpiSource device): m_packet() {
		m_packet.source = device;
	}

	/**
	 * @brief Деструктор
	 */
	virtual ~SpiDeviceMsg() {}

	/**
	 * @brief Вернуть идентификатор устройства
	 *
	 * @return SpiSource
	 */
	SpiSource getSource() const {
		return m_packet.source;
	}

	/**
	 * @brief Вернуть тип пакета
	 *
	 * @return SpiPacketType
	 */
	SpiPacketType getType() const {
		return m_packet.type;
	}

	/**
	 * @brief Вернуть пакет в виде массива байт
	 *
	 * @param SpiDeviceData
	 */
	virtual SpiDeviceData makeSpiData() = 0;

	/**
	 * @brief Распарсить данные от устройства на шине SPI
	 *
	 * @param bool признак успешного выполнения
	 */
	virtual bool parseSpiData(const SpiDeviceData& packet) = 0;

protected:
	/**
	 * @brief Данные устройства на шине SPI
	 */
	SpiDeviceData m_packet;
};

}

#endif // SPIDEVICEMSG_H
