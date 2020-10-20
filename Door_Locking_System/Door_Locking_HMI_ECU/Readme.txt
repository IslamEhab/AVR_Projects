HMI ECU Code
This ECU uses UART to communicate with Control ECU, timers to count time, LCD and keypad.
Timers, I2C, and UART drivers are implemented in struct configuration method to increase the 
portability and readablility of the code. Also applying call back function method on 
ISRs to increase portability of the driver and also to reduce interrupt latency