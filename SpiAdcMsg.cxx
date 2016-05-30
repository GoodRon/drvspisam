/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include "SpiAdcMsg.h"

namespace spi {

static const double m_adcCoefficient = 0.0786328125;

SpiAdcMsg::SpiAdcMsg():
	SpiDeviceMsg(sdPio) {
}

SpiAdcMsg::~SpiAdcMsg() {
}

SpiDeviceData SpiAdcMsg::makeSpiData() {
	return m_packet;
}

bool SpiAdcMsg::parseSpiData(const SpiDeviceData& packet) {
	if (packet.source != sdAdc) {
		return false;
	}
	m_packet = packet;
	return true;
}

int SpiAdcMsg::getAdcChannel1() const {
	if (m_packet.data.size() < 6) {
		return 0;
	}
	return static_cast<int>(m_packet.data[0]);
}

int SpiAdcMsg::getAdcChannel2() const {
	if (m_packet.data.size() < 6) {
		return 0;
	}
	return static_cast<int>(m_packet.data[1]);
}

int SpiAdcMsg::getAdcChannel3() const {
	if (m_packet.data.size() < 6) {
		return 0;
	}
	return static_cast<int>(m_packet.data[2]);
}

int SpiAdcMsg::getAdcChannel4() const {
	if (m_packet.data.size() < 6) {
		return 0;
	}
	return static_cast<int>(m_packet.data[3]);
}

double SpiAdcMsg::getReservePower() const {
	if (m_packet.data.size() < 6) {
		return 0;
	}
	return static_cast<double>(m_packet.data[4]) * m_adcCoefficient;
}

double SpiAdcMsg::getMainPower() const {
	if (m_packet.data.size() < 6) {
		return 0;
	}
	return static_cast<double>(m_packet.data[5]) * m_adcCoefficient;
}

}
