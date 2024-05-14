import serial
import time


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
        heartRate = heartRate - 120 ;
        if(heartRate <= 50 ) :
          heartRate = 0
        
        print('BPM value:', heartRate)
         
        
    except ValueError:
        print('Invalid BPM value:', lux_str)

time.sleep(0.5)