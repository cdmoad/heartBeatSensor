#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "i2c1602.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstring> // For string manipulation
#include <cstdio>  // For sprintf

#define LCD_ADDR 0x27

int main() {
    wiringPiSetup();
    I2C16x2 lcd;
    lcd_init(LCD_ADDR);
    ClrLcd();

    int uart0_filestream = -1;

    // Open the serial port (UART)
    uart0_filestream = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1) {
        std::cerr << "Error - Unable to open UART." << std::endl;
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
    while (true) {
        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, rx_buffer, sizeof(rx_buffer));
        if (rx_length < 0) {
            std::cerr << "Error - Reading from UART." << std::endl;
            break;
        } else if (rx_length == 0) {
            usleep(500000); // Sleep for 500 ms (adjust as needed)
        } else {
            // Convert bytes to string
            std::string lux_str(reinterpret_cast<char*>(rx_buffer), rx_length);

            // Convert string to float
            float heartRate = std::stof(lux_str);
            heartRate -= 120;
            if (heartRate <= 50) {
                heartRate = 0;
            }

            // Print heart rate value
            std::cout << "BPM value: " << heartRate << std::endl;
            
            // Convert float to string
            char heartRateStr[60];
            ClrLcd();
            sprintf(heartRateStr, "BPM value: %.2f", heartRate);
            typeString(heartRateStr); // Display on LCD
        }
    }

    // Close the serial port
    close(uart0_filestream);
    return 0;
}
