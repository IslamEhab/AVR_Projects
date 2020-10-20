Control ECU code
This ECU uses UART to communicate with HMI ECU, I2C to communicate with external EEPROM,
timers to count time, motors to open/close the door.
Timers, I2C, and UART drivers are implemented in struct configuration method to increase the 
portability and readablility of the code. Also applying call back function method on 
ISRs to increase portability of the driver and also to reduce interrupt latency.