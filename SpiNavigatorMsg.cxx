/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "SpiNavigatorMsg.h"

namespace spi {

SpiNavigatorMsg::SpiNavigatorMsg():
	SpiDeviceMsg(sdUsart0) {
}

SpiNavigatorMsg::SpiNavigatorMsg(SamComSpeed speed):
	SpiDeviceMsg(sdUsart0) {
	m_packet.type = sptCommand;
	m_packet.data.push_back(speed);
	m_packet.data.push_back(0);
	m_packet.data.push_back(0);
	m_packet.data.push_back(0);
}

SpiNavigatorMsg::~SpiNavigatorMsg() {
}

SpiDeviceData SpiNavigatorMsg::makeSpiData() {
	return m_packet;
}

bool SpiNavigatorMsg::parseSpiData(const SpiDeviceData& packet) {
	if (packet.source != sdUsart0) {
		return false;
	}
	m_packet = packet;
	return true;
}

}
