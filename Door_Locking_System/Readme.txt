Door locking system project is consist of 2 ECUs, HMI ECU used to interact with human with LCD & Keypad
and Control ECU to take the proper decisions. User will enter the password using HMI system then, HMI System
will send data to Control system through UART communication protocol. Control ECU will compare Password given
by user with the saved password on External EEPROM chip which use I2C communication protocol. If correct, 
system will proceed to next step which is change password or open the door. However, if wrong for 3 times consecutively,
alarm will turn on for 1 minute and LCD will display 'Theif !!'