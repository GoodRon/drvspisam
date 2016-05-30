/*
 * Created by Roman Meyta 2013 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "powerControl.h"
#include "gpio.h"
#include "gpio_driver.h"

namespace power {

const char gpioDevice[] = "/dev/mgpio";

int navigatorPower(OnOff state) {
	int gpio = open(gpioDevice, O_RDONLY);
	if (gpio < 0) {
		syslog(LOG_ERR, "GPIO open error : %m (%s)", gpioDevice);
		return -1;
	}

	int ret = 0;
	if (state == ON) {
		int rc = ioctl(gpio, GPIO_IOCT1PIN, AT91_PIN_PB4);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	} else {
		int rc = ioctl(gpio, GPIO_IOCT0PIN, AT91_PIN_PB4);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	}
	close(gpio);
	return ret;
}

int modemPower(OnOff state) {
	int gpio = open(gpioDevice, O_RDONLY);
	if (gpio < 0) {
		syslog(LOG_ERR, "GPIO open error : %m (%s)", gpioDevice);
		return -1;
	}

	int ret = 0;
	if (state == ON) {
		int rc = ioctl(gpio, GPIO_IOCT0PIN, AT91_PIN_PB5);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	} else {
		int rc = ioctl(gpio, GPIO_IOCT1PIN, AT91_PIN_PB5);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	}
	close(gpio);
	return ret;
}

int printerPower(OnOff state) {
	int gpio = open(gpioDevice, O_RDONLY);
	if (gpio < 0) {
		syslog(LOG_ERR, "GPIO open error : %m (%s)", gpioDevice);
		return -1;
	}

	int ret = 0;
	if (state == ON) {
		int rc = ioctl(gpio, GPIO_IOCT1PIN, AT91_PIN_PB3);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	} else {
		int rc = ioctl(gpio, GPIO_IOCT0PIN, AT91_PIN_PB3);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	}
	close(gpio);
	return ret;
}

int dkmPower(OnOff state) {
	int gpio = open(gpioDevice, O_RDONLY);
	if (gpio < 0) {
		syslog(LOG_ERR, "GPIO open error : %m (%s)", gpioDevice);
		return -1;
	}

	int ret = 0;
	if (state == ON) {
		int rc = ioctl(gpio, GPIO_IOCT1PIN, AT91_PIN_PB7);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	} else {
		int rc = ioctl(gpio, GPIO_IOCT0PIN, AT91_PIN_PB7);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	}
	close(gpio);
	return ret;
}

int samPower(OnOff state) {
	int gpio = open(gpioDevice, O_RDONLY);
	if (gpio < 0) {
		syslog(LOG_ERR, "GPIO open error : %m (%s)", gpioDevice);
		return -1;
	}

	int ret = 0;
	if (state == ON) {
		int rc = ioctl(gpio, GPIO_IOCT1PIN, AT91_PIN_PA28);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	} else {
		int rc = ioctl(gpio, GPIO_IOCT0PIN, AT91_PIN_PA28);
		if (rc < 0 ) {
			syslog(LOG_ERR, "GPIO ioctl error: %m");
			ret = -1;
		}
	}
	close(gpio);
	return ret;
}

}
