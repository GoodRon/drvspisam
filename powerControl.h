/*
 * Created by Roman Meyta 2013 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

/**
 * @file Библиотека для управления питанием подсистем мобис'а
 */

#ifndef POWERCONTROL_H
#define POWERCONTROL_H

#include "OnOff.h"

/**
 * @brief Пространство имен для библиотеки управления питанием
 */
namespace power {

/**
 * @brief Включить/выключить питание GPS навигатора
 *
 * @param state ON - вкл, OFF - выкл
 * @return int 0 - успешное завершение, -1 - ошибка
 */
int navigatorPower(OnOff state);

/**
 * @brief Включить/выключить питание GSM модема
 *
 * @param state ON - вкл, OFF - выкл
 * @return int 0 - успешное завершение, -1 - ошибка
 */
int modemPower(OnOff state);

/**
 * @brief Включить/выключить питание принтера
 *
 * @param state ON - вкл, OFF - выкл
 * @return int 0 - успешное завершение, -1 - ошибка
 */
int printerPower(OnOff state);

/**
 * @brief Включить/выключить питание клавиатурно-дисплейного модуля (КДМ)
 *
 * @param state ON - вкл, OFF - выкл
 * @return int 0 - успешное завершение, -1 - ошибка
 */
int dkmPower(OnOff state);

/**
 * @brief Включить/выключить SAM
 *
 * @param state ON - вкл, OFF - выкл
 * @return int 0 - успешное завершение, -1 - ошибка
 */
int samPower(OnOff state);

}

#endif // POWERCONTROL_H
