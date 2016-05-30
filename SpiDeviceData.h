/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef SPIDEVICEDATA_H
#define SPIDEVICEDATA_H

#include <vector>
#include <stdint.h>

#include "SpiDevice.h"

/**
 * @brief Пространство имен для работы с устройствами на шине SPI
 **/
namespace spi {

/**
 * @brief Структура данных от устройства на шине SPI
 **/
struct SpiDeviceData {
	/**
	 * @brief Конструктор
	 **/
	SpiDeviceData();

	/**
	 * @brief Перемещающий конструктор
	 *
	 * @param otherDevice
	 * @param otherType
	 * @param otherBytes
	 **/
	SpiDeviceData(SpiSource otherDevice, SpiPacketType otherType, SpiBytes&& otherBytes);

	/**
	 * @brief Вернуть SPI пакет (данные в виде последовательности байт)
	 *
	 * @param header
	 * @return SpiBytes
	 **/
	SpiBytes makeRawPacket(SpiPacketHeader header = sphSingle) const;

	/**
	 * @brief Идентификатор устройства
	 **/
	SpiSource source;

	/**
	 * @brief Тип пакета
	 **/
	SpiPacketType type;

	/**
	 * @brief Данные
	 **/
	SpiBytes data;
};

}

#endif // SPIDEVICEDATA_H
