#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int main() {
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
            // printf("No data available. Waiting...\n");
            usleep(500000); // Sleep for 100 ms (adjust as needed)
        } else {
            // Print received data
            printf("Received %d bytes: ", rx_length);
            for (int i = 0; i < rx_length; i++) {
                printf("%c", rx_buffer[i]);
            }
            printf("\n");
        }
    }

    // Close the serial port
    close(uart0_filestream);
    return 0;
}
