#include <stdio.h>
#include "uart.h"

int main()
{
    struct UartDevice dev;
    char buf[100];
    char buf2[120];

    dev.filename = "/dev/ttyS1";
    dev.rate = 9600;
    uart_init(&dev);

    printf("UART DEMO\r\n");
    uart_write(&dev, "UART DEMO\r\n");

    while(1) {
        if(uart_read(&dev, buf)) {
            printf("%s\r\n", buf);
            snprintf(buf2, 120, "loopback: %s\r\n", buf);
            uart_write(&dev, buf2);
        }
    }

    uart_free(&dev);
    return 0;
}
