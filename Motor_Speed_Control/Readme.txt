Simple project to control Motor speed using a PWM Signal as input to Motor Module
Motor speed is decided by a potentiometer that converts its analog value to digital 
through ADC Peripheral and Display ADC Value on LCD.
ADC and Timer drivers are implemented in struct configuration method to increase the 
portability and readablility of the code. Also applying call back function method on 
ISRs to increase portability of the driver and also to reduce interrupt latency
The code is well documented.