// В качестве магического номера используем свободный символ 'g'
#define GPIO_IOC_MAGIC 'g'

#define GPIO_IOCRESET _IO(GPIO_IOC_MAGIC, 0)

/*
 * Смысл символа, идущего сразу после GPIO_IOC 
 * S означает "Set" через аргумент-указатель
 * T означает "Tell" с непосредственно заданым аргументом
 * G означает "Get": возврат значения через указатель
 * Q означает "Query": вернуть непосредственное значение
 * X означает "eXchange": автоматически G и S
 * H означает "sHift": автоматически T и Q
 */
#define GPIO_IOCT0PIN	_IO(GPIO_IOC_MAGIC,   1)
#define GPIO_IOCT1PIN	_IO(GPIO_IOC_MAGIC,   2)
#define GPIO_IOCQPIN	_IO(GPIO_IOC_MAGIC,   3)
// Для целей отладки
#define GPIO_IOCDEBUG	_IO(GPIO_IOC_MAGIC,   4)

#define GPIO_IOC_MAXNR 4

