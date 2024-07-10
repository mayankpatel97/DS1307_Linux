#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "ds1307.h"

// Convert binary coded decimal to decimal
static int bcd_to_dec(int val) {
    return ((val / 16 * 10) + (val % 16));
}

// Convert decimal to binary coded decimal
static int dec_to_bcd(int val) {
    return ((val / 10 * 16) + (val % 10));
}

int rtc_init(const char *i2c_device) {
    int file = open(i2c_device, O_RDWR);
    if (file < 0) {
        perror("Failed to open I2C device");
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, DS1307_ADDRESS) < 0) {
        perror("Failed to set I2C address");
        close(file);
        exit(1);
    }

    return file;
}

int rtc_read_time(int file, rtc_time *time) {
    unsigned char buffer[7];

    // Set register pointer to 0
    buffer[0] = 0x00;
    if (write(file, buffer, 1) != 1) {
        perror("Failed to reset register pointer");
        return -1;
    }

    // Read 7 bytes of data
    if (read(file, buffer, 7) != 7) {
        perror("Failed to read data from RTC");
        return -1;
    }

    time->sec = bcd_to_dec(buffer[0]);
    time->min = bcd_to_dec(buffer[1]);
    time->hour = bcd_to_dec(buffer[2]);
    time->day = bcd_to_dec(buffer[3]);
    time->date = bcd_to_dec(buffer[4]);
    time->month = bcd_to_dec(buffer[5]);
    time->year = bcd_to_dec(buffer[6]) + 2000; // Assuming year is 2000+

    return 0;
}

int rtc_write_time(int file, const rtc_time *time) {
    unsigned char buffer[8];

    buffer[0] = 0x00; // Start at register 0
    buffer[1] = dec_to_bcd(time->sec);
    buffer[2] = dec_to_bcd(time->min);
    buffer[3] = dec_to_bcd(time->hour);
    buffer[4] = dec_to_bcd(time->day);
    buffer[5] = dec_to_bcd(time->date);
    buffer[6] = dec_to_bcd(time->month);
    buffer[7] = dec_to_bcd(time->year - 2000); // Assuming year is 2000+

    if (write(file, buffer, 8) != 8) {
        perror("Failed to write data to RTC");
        return -1;
    }

    return 0;
}
