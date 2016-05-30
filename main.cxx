/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <string>

#include <linux/spi/spidev.h>

#include "SpiDevice.h"
#include "Drvspisam.h"
#include "powerControl.h"

using namespace std;
using namespace spi;
using namespace power;

/**
 * @brief Точка входа для приложения drvspisam
 *
 * @return int
 **/
int main() {
	samPower(OFF);
	samPower(ON);

	Drvspisam drvspisam(samSpiDevice, SPI_MODE_0, 8, 5000000L);
	drvspisam.run();

	samPower(OFF);
	return 0;
}
