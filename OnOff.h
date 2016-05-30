/*
 * Created by Roman Meyta 2013 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

/** @file
 * Вспомогательный файл, содержащий перечисления для переключения состояний объекта
 */

#ifndef ONOFF_H
#define ONOFF_H

/**
 * @brief Перечисление состояний запуск/остановка
 **/
enum StartStop_t {
	STOP = 0,
	START = 1
};
typedef enum StartStop_t StartStop;

/**
 * @brief Перечисление состояний вкл/выкл
 **/
enum OnOff_t {
	OFF = 0,
	ON = 1
};
typedef enum OnOff_t OnOff;

#endif // ONOFF_H
