#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "i2c1602.h"
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>


#define LCD_ADDR 0x27 // địa chỉ của lcd i2c, dùng lệnh "i2cdetect -y 1" để xem địa chỉ
I2C16x2 lcd; // tạo struct lcd chứa địa chỉ I2C


int main() {

    wiringPiSetup(); // gọi wiringPiSetup để lấy thời gian cho các hàm delay và delayMicroseconds
	lcd_init(LCD_ADDR);
    ClrLcd();
    
    int uart0_filestream = -1;

    // Open the serial port (UART)
    uart0_filestream = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1) {
        printf("Error - Unable to open UART.\n");
        return 1;
    }

    // Configure the serial port
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; // 9600 baud, 8-bit data, no parity, 1 stop bit
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    // Read from the serial port continuously
    while (1) {
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void*)rx_buffer, sizeof(rx_buffer));
        if (rx_length < 0) {
            printf("Error - Reading from UART.\n");
            break; // Exit the loop on error
        } else if (rx_length == 0) {
            usleep(500000); // Sleep for 500 ms (adjust as needed)
        } else {
            // Convert bytes to string
            char lux_str[rx_length + 1];
            for (int i = 0; i < rx_length; i++) {
                lux_str[i] = rx_buffer[i];
            }
            lux_str[rx_length] = '\0';

            // Convert string to float
            float heartRate = atof(lux_str);
            // heartRate -= 120;
            // if (heartRate <= 50) {
            //     heartRate = 0;
            // }

            // Print heart rate value
            printf("BPM value: %.2f\n", heartRate);
            char heartRateStr[60];
             ClrLcd();
            sprintf(heartRateStr, "BPM value: %.2f", heartRate);
            typeString(heartRateStr); // in ra chuỗi "hello"
           
    
        }
    }

    // Close the serial port
    close(uart0_filestream);
    return 0;
}



/* 
// Author: Le Thanh Vinh
// Contact: thanhvinhkma@gmail.com
*/


 