#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ds1307.h"

int main() {
    const char *i2c_device = "/dev/i2c-1";
    rtc_time time;
    int file = rtc_init(i2c_device);

    // Example: Set time to 12:34:56 on 7th July 2024
    time.sec = 56;
    time.min = 34;
    time.hour = 12;
    time.day = 1; // Sunday
    time.date = 7;
    time.month = 7;
    time.year = 2024;

    if (rtc_write_time(file, &time) < 0) {
        printf("Failed to set time\n");
        close(file);
        return 1;
    }

    sleep(1);

    if (rtc_read_time(file, &time) < 0) {
        printf("Failed to read time\n");
        close(file);
        return 1;
    }

    printf("Current Time: %02d:%02d:%02d\n", time.hour, time.min, time.sec);
    printf("Current Date: %02d-%02d-%04d\n", time.date, time.month, time.year);

    close(file);
    return 0;
}
