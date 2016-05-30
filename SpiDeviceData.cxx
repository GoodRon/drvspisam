/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <stddef.h>

#include "SpiDeviceData.h"

namespace spi {

SpiDeviceData::SpiDeviceData():
	source(sdSam),
	type(sptData),
	data() {
}

SpiDeviceData::SpiDeviceData(SpiSource otherDevice, SpiPacketType otherType, SpiBytes&& otherBytes) :
	source(otherDevice),
	type(otherType),
	data(otherBytes) {
}

SpiBytes SpiDeviceData::makeRawPacket(SpiPacketHeader header) const {
	SpiBytes rawPacket;
	size_t length = data.size();

	rawPacket.push_back(static_cast<uint8_t>(header));
	rawPacket.push_back(static_cast<uint8_t>(source | type));
	rawPacket.push_back(0xFF & length);
	rawPacket.push_back(0xFF & (length >> 8));
	rawPacket.insert(rawPacket.end(), data.begin(), data.end());
	return rawPacket;
}

}
