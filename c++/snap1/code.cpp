#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int main() {
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
            break; // Exit the loop on error
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
        }
    }

    // Close the serial port
    close(uart0_filestream);
    return 0;
}
