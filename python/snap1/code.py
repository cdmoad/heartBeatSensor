#! /usr/bin/env python

# Simple string program. Writes and updates strings.
# Demo program for the I2C 16x2 Display from Ryanteck.uk
# Created by Matthew Timmons-Brown for The Raspberry Pi Guy YouTube channel

# Import necessary libraries for communication and display use
import drivers
from time import sleep

# Load the driver and set it to "display"
# If you use something from the driver library use the "display." prefix first
display = drivers.Lcd()


import time
import Adafruit_ADS1x15

# Create ADC object for ADS1115.
adc = Adafruit_ADS1x15.ADS1115()

# Set the gain (adjust this as needed)
GAIN = 2/3

# Define the channel to which the LDR is connected
LDR_CHANNEL = 3  # Assuming LDR is connected to A0 on the ADS1115

# Number of readings to average
NUM_READINGS = 25

# Main loop. Use Ctrl-C to stop the code
try:
    while True:
        # Initialize sum of readings
        sum_readings = 0
        
        # Collect multiple reading
        for _ in range(NUM_READINGS):
            sum_readings += adc.read_adc(LDR_CHANNEL, gain=GAIN)
            time.sleep(0.1)  # Small delay between readings
        
        # Calculate the average
        avg_reading = sum_readings / NUM_READINGS
        
        # Print the average ADC value
        res= ((avg_reading)*-1 -5)*5 -200
        if res < 50:
            print("No heartbeat found")
            display.lcd_clear()  # Clear the LCD
            display.lcd_display_string("No heartbeat found", 1)  # Display new content
        else:     
            print("BPM:", res)
            display.lcd_clear()  # Clear the LCD
            display.lcd_display_string(f"{res:.2f} BPM", 1)  # Display new content

        # Add a short delay before the next reading
        time.sleep(0.2)

except KeyboardInterrupt:
    print("\nExiting...")