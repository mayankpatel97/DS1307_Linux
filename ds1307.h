#ifndef DS1307_H
#define DS1307_H

#define DS1307_ADDRESS 0x68 // I2C address of the DS1307

// Time structure
typedef struct {
    int sec;
    int min;
    int hour;
    int day;
    int date;
    int month;
    int year;
} rtc_time;

// Function prototypes
int rtc_init(const char *i2c_device);
int rtc_read_time(int file, rtc_time *time);
int rtc_write_time(int file, const rtc_time *time);

#endif // DS1307_H
