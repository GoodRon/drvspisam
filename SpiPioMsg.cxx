/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "SpiPioMsg.h"

namespace spi {

SpiPioMsg::SpiPioMsg():
	SpiDeviceMsg(sdPio) {
}

SpiPioMsg::SpiPioMsg(uint8_t pins):
	SpiDeviceMsg(sdPio) {
	m_packet.type = sptCommand;
	m_packet.data.push_back(pins);
}

SpiPioMsg::~SpiPioMsg() {
}

SpiDeviceData SpiPioMsg::makeSpiData() {
	if (m_packet.data.size() < 1) {
		m_packet.data.push_back(0);
	}
	return m_packet;
}

bool SpiPioMsg::parseSpiData(const SpiDeviceData& packet) {
	if (packet.source != sdPio) {
		return false;
	}
	m_packet = packet;
	return true;
}

bool SpiPioMsg::checkPin(Pin pin) const {
	if (m_packet.data.size() < 1) {
		return false;
	}
	return ((m_packet.data[0] & pin) != 0);
}

uint8_t SpiPioMsg::getPins() const {
	if (m_packet.data.size() < 1) {
		return 0;
	}
	return m_packet.data[0];
}

}
