#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "uart.h"

int uart_init(struct UartDevice * dev)
{
    struct termios * tty;
    int fd;

    fd = open(dev->filename, O_RDWR|O_NOCTTY);
    if(fd < 0) {
        printf("%s: failed to open UART device.\n", __func__);
        return -1;
    }

    tty = malloc(sizeof(*tty));
    if(!tty) {
        printf("%s: failed to allocate UART tty instance\n", __func__);
        return -1;
    }

    memset(tty, 0, sizeof(*tty));

    // set baud rate
    tty->c_cflag |= dev->rate;
    // ignore framing and parity errors in input
    tty->c_iflag |= IGNPAR;
    // use 8-bit characters
    tty->c_cflag |= CS8;
    // enable receiver
    tty->c_cflag |= CREAD;
    // canonical
    //tty->c_lflag |= ICANON;
    
    // maintain best compatibility with normal behaviour of terminals
    tty->c_cc[VTIME] = 0;
    tty->c_cc[VMIN] = 1;
    
    // flush port
    tcflush(fd, TCIFLUSH);
    // apply attribute
    tcsetattr(fd, TCSANOW, tty);

    dev->fd = fd;
    dev->tty = tty;

    return 0;
}

int uart_read(struct UartDevice * dev, char * buf)
{
    int n;
    n = read(dev->fd, buf, 100);
    if(n < 0) {
        printf("%s: failed to read uart data.\n", __func__);
        return -1;
    }

    buf[n] = '\0';
    return n;
}

int uart_write(struct UartDevice * dev, char * buf)
{
    int n;
    size_t buf_len = strlen(buf);
    
    n = write(dev->fd, buf, buf_len);
    return n;
}

void uart_free(struct UartDevice * dev)
{
    free(dev->tty);
}
