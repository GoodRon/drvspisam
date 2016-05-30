/*
 * Created by Roman Meyta 2015 <meyta@incom.tomsk.ru>
 * Incom inc Tomsk Russia http://incom.tomsk.ru/
 */

#ifndef SPI_H
#define SPI_H

#include <string>
#include <vector>
#include <inttypes.h>
#include <stdint.h>

#include "SpiDevice.h"
#include "SpiDeviceData.h"
#include "SpiDeviceMsg.h"

/**
 * @page SamProto Протокол обмена данными с сопроцессором SAM по шине SPI
 * @section proto Описание протокола
 *
 * ВАЖНО! Размер буферов для обмена данными по шине SPI должен быть равен 3128 (не спрашивайте
 * почему, прибито гвоздями к прошивке SAM)
 *
 * Формат одиночного пакета данных (заголовок 0xEE):
 * |Байт 1|Байт 2|Байт 3|Байт 4|Байт 5|...|
 * |------|------|------|------|------|------|
 * |Заголовок пакета|Идентифицирующие флаги|Младший байт длины данных|Старший байт длины данных|Данные|...|
 *
 * Идентифицирующие флаги включают в себя признак данных или команды для всего пакета и
 * идентификатор устройства, сложенных побитово.
 *
 * Признаки:
 * |Признак данных|Признак команды|
 * |------|------|
 * |0x00|0x80|
 *
 * Идентификаторы устройств:
 * |USART0|USART1|DBGU|PWR MASK|PWR CTRL|ADC|PIO|
 * |------|------|------|------|------|------|------|
 * |0x01|0x02|0x03|0x04|0x05|0x06|0x07|
 *
 * SAM на работает с одиночными пакетами данных. Обмен всегда происходит групповыми пакетами (даже
 * если он содержит только один одиночный пакет). Одиночные пакеты распологаются в данных
 * группового пакета
 *
 * Формат группового пакета (заголовок 0xEA):
 * |Байт 1|Байт 2|Байт 3|Байт 4|Байт 5|Байт 6|Байт 7|Байт 8|Байт 9|...|
 * |------|------|------|------|------|------|------|------|------|------|
 * |Заголовок группового пакета|Идентифицирующие флаги (признак команды не используется)|Младший байт длины данных|Старший байт длины данных|Заголовок одиночного пакета|Признак данных или команды|Младший байт длины данных|Старший байт длины данных|Данные|...|
 *
 * Пример:
 * |Байт 1|Байт 2|Байт 3|Байт 4|Байт 5|Байт 6|Байт 7|Байт 8|Байт 9|Байт 10|Байт 11|Байт 12|
 * |------|------|------|------|------|------|------|------|------|------|------|------|
 * |0xEA|0x00|0x08|0x00|0xEE|0x81|0x04|0x00|0x0E|0x00|0x00|0x00|
 *
 */

/**
 * @brief Пространство имен для работы с устройствами на шине SPI
 **/
namespace spi {

/**
 * @brief Получить файловый дескриптор для шины SPI
 *
 * @param device путь к представлению шины в файловой системе
 * @param mode режим работы
 * @param bits количество бит на слово
 * @param speed скорость обмена данными
 * @return int дескриптор или код ошибки
 **/
int initSpi(const std::string& device, uint8_t mode, uint8_t bits, long speed);

/**
 * @brief Завершить работу с шиной SPI
 *
 * @param spiFd дескриптор шины SPI
 * @return int
 */
int closeSpi(int spiFd);

/**
 * @brief Осуществить обмен данными по шине SPI
 *
 * @param spiFd дескриптор шины SPI
 * @param wbuf буфер данных для передачи
 * @param rbuf буфер принимаемых данных
 * @param size размер буферов
 * @return int
 */
int transferSpiBuffers(int spiFd, uint8_t* wbuf, uint8_t* rbuf, size_t size);

/**
 * @brief Выделить из потока байт пакет данных от сопроцессора SAM
 * Возвращает часть от пакета, который не был захвачет в кадр полностью
 *
 * @param header заголовочный байт
 * @param rawData сырые данные с дескриптора шины SPI
 * @param residure буфер для обрывочного пакета
 * @return std::vector<SpiDeviceData>
 */
std::vector<SpiDeviceData> selectSpiPackets(SpiPacketHeader header, const SpiBytes& rawData,
											SpiBytes& residure);

/**
 * @brief Выделить из потока байт пакет данных от сопроцессора SAM
 *
 * @param header заголовочный байт
 * @param rawData сырые данные с дескриптора шины SPI
 * @return std::vector<SpiDeviceData>
 */
std::vector<SpiDeviceData> selectSpiPackets(SpiPacketHeader header, const SpiBytes& rawData);

/**
 * @brief Выделить из потока байт пакет данных от сопроцессора SAM. Ленивая версия. Останавливается
 * при нахождении байта 0xFF, знаменующего пустые данные
 *
 * @param header заголовочный байт
 * @param rawData сырые данные с дескриптора шины SPI
 * @param length длина данных в фуфере
 * @return std::vector<SpiDeviceData>
 */
std::vector<SpiDeviceData> selectSpiPacketsLazily(SpiPacketHeader header, const SpiBytes& rawData,
												  size_t length = 0);

}

#endif // SPI_H
