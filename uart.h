#ifndef __UART_H__
#define __UART_H__

#include <termios.h>

struct UartDevice {
    char * filename;
    int rate;
    int fd;
    struct termios * tty;
};

int uart_init(struct UartDevice * dev);
int uart_write(struct UartDevice * dev, char * buf);
int uart_read(struct UartDevice * dev, char * buf);
void uart_free(struct UartDevice * dev);

#endif /* __UART_H__ */
