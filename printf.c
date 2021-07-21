#include <unistd.h>
#include <stdbool.h>
#include "printf.h"
#include "uart.h"

static char out_buf[1000]; // buffer for _vprintf()
static struct UartDevice dev = {
    .filename = "/dev/ttyS1",
    .rate = 9600,
};

static int _vsnprintf(char * out, size_t n, const char * s, va_list arg_ptr)
{
    size_t out_pos = 0;
    bool format = false;

    for(; *s; s++) {
        if(format) {
            switch(*s) {
                case 'd': {
                    int num = va_arg(arg_ptr, int);
                    if(num < 0) {
                        num = -num;
                        if(out && out_pos < n) {
                            out[out_pos] = '-';
                        }
                        out_pos++;
                    }
                    int digits = 1;
                    for(int nn = num; nn /= 10; digits++);
                    for(int i = digits-1; i >=0; i--) {
                        if(out && out_pos + i < n) {
                            out[out_pos + i] = '0' + (num % 10);
                        }
                        num /= 10;
                    }
                    out_pos += digits;
                    format = false;
                    break;
                }
                case 's': {
                    const char * s2 = va_arg(arg_ptr, const char *);
                    for(; *s2; s2++) {
                        if(out && out_pos < n) {
                            out[out_pos] = *s2;
                        }
                        out_pos++;
                    }
                    format = false;
                    break;
                }
                default:
                    break;
            }
        } else if (*s == '%') {
            format = true;
        } else {
            if(out && out_pos < n) {
                out[out_pos] = *s;
            }
            out_pos++;
        }
    }

    // add '\0' to the end
    if(out && out_pos < n) {
        out[out_pos] = '\0';
    } else if (out && n) {
        out[n - 1] = '\0';
    }

    return out_pos;
}

static int _vprintf(const char * s, va_list arg_ptr)
{
    //int len = _vsnprintf(NULL, -1, s, arg_ptr);
    //if(len + 1 >= sizeof(out_buf)) {
    //    uart_write(&dev, "error: output string size overflow\n");
    //}

    //_vsnprintf(out_buf, len + 1, s, arg_ptr);
    // 发现的问题: 重复执行时, arg_ptr发生变化, 第二次执行获取的值异常.
    int len = _vsnprintf(out_buf, 1000, s, arg_ptr);
    uart_write(&dev, out_buf);

    return len;
}

int printf(const char * s, ...)
{
    uart_init(&dev);

    int result = 0;
    va_list arg_ptr;
    va_start(arg_ptr, s);
    result = _vprintf(s, arg_ptr);
    va_end(arg_ptr);

    uart_free(&dev);

    return result;
}
