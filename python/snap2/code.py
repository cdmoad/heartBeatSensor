import serial
import time
import drivers
 
display = drivers.Lcd()


ser = serial.Serial(port='/dev/ttyUSB0', baudrate=9600,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS,
                    timeout=1)

while True:
    lux_bytes = ser.read(10)
    lux_str = lux_bytes.decode('utf-8').strip()  # Convert bytes to string
    try:
        heartRate = float(lux_str)  # Convert string to float
        
        print('BPM value:', heartRate)
        display.lcd_clear()  # Clear the LCD
        res = "BPM value: {}".format(heartRate)
        display.lcd_display_string(res, 1)  # Display new content
        
                
        
    except ValueError:
        print('Invalid BPM value:', lux_str)

time.sleep(0.5)



 


 
  
     
         