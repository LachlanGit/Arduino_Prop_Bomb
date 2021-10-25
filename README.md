# Arduino Prop Bomb
Prop bomb for use in Airsoft/Gel ball/Nerf games

[Check out the video here](https://youtu.be/Dwf5lzY_378)

Submit issues via the issues tab. Contact me at lachlan.github@outlook.com

## Electronics parts list: 
(It may be possible to use alternative parts, however, the code will have to be customised for them)
- Arduino Mega 2560
- 12 button keypad (0-9, *, #)
- 128 * 64 3-wire SPI LCD display (I used [this](https://www.digikey.com.au/product-detail/en/dfrobot/DFR0091/DFR0091-ND/7597123) one)
- Red and green buttons either with built in LED's or separate LED's (Make sure to use the appropriate resistor for your LED's)
- 3 position 2 pole keyswitch if using for power and to enter config mode **or** 2 position 1 pole switch if just for config mode (Use a separate switch for power)
- Square wave buzzer
- 7 to 12 volt power supply for Arduino (in my case, I used 2 18650 Li-Ion cells in series in battery holders)
- BMS to prevent over charging/over discharging of batteries (important if using Li-Po or Li-Ion batteries without integrated BMS)
- Various jumper cables/ribbon cables to connect everything together

## Optional parts list: 
- Some kind of case or enclosure (I used a black ABS 197Lx112Wx63Hmm box)
- Protective acrylic cover for LCD
- Charging PCB for Li-ion or Li-po batteries (I have omitted this and just remove the batteries when I need to charge them)
- Handle for carrying
- Protective corners to prevent damage to the bomb

## Wiring connections: 
*NO = Normally open, NC = Normally closed*

*These can be edited in the "Pins" section of the code*

1. SPI LCD Display
    1. LCD VCC - Arduino 5V
    2. LCD GND - Arduino GND
    3. LCD RS - Arduino 4
    4. LCD RW - Arduino 3
    5. LCD E - Arduino 2


2. Keypad
    1. Keypad 2 - Arduino 20
    2. Keypad 3 - Arduino 19
    3. Keypad 4 - Arduino 18
    4. Keypad 5 - Arduino 17
    5. Keypad 6 - Arduino 16
    6. Keypad 7 - Arduino 15
    7. Keypad 8 - Arduino 14

*Pinout on my keypad* ![Image of keypad pinout](https://github.com/Voltage99/Arduino_Prop_Bomb/blob/d9e85f99d4aa42dad52defa2432119da7a1e23b5/Keypad%20pinout.jpg)

3. Buttons
    1. Red button COM - Arduino GND (Or use 49 to save on wiring)
    2. Red button NO - Arduino 47
    3. Green button COM - Arduino GND (Or use 46 to save on wiring)
    4. Green button NO - Arduino 48

4. LED's
    1. Red LED Cathode (-) - Arduino GND
    2. Red LED Anode (+) - Arduino 53 (Must use appropriate resistor)
    3. Green LED Cathode (-) - Arduino GND
    4. Green LED Anode (+) - Arduino 52 (Must use appropriate resistor)

5. Keyswitch
    1. Key GND - Arduino GND (Or use 24 to save on wiring)
    2. Key NO - Arduino 22

6. Buzzer
    1. Buzzer - Arduino 11 + 12 (Polarity irrelevant)

7. Battery (Use second pole of keyswitch **or** a separate switch in series to toggle power)
    1. Battery Ground (-) - Arduino GND
    2. Battery Positive (+) - Arduino VIN

## Required libraries
- U8g2 by oliver (Available in library manager or [here](https://github.com/olikraus/u8g2))
- Keypad by Community https://github.com/Chris--A/Keypad (Available in library manager or [here](https://github.com/Chris--A/Keypad))
- Button by t3db0t (Available [here](https://github.com/virgildisgr4ce/Button))
- ToneAC by Tim Eckel (Available in library manager or [here](https://github.com/teckel12/arduino-toneac))
- EEPROMEx by Thijs Elenbaas (Available in library manager or [here](http://playground.arduino.cc/Code/EEPROMex))
- Timers by LachlanGit/Me (Available [here](https://github.com/LachlanGit/Arduino_Timer_Library))

## Configuration mode usage
- Turn key to config position (will be different depending on your key switch used)
- Use Red and Green buttons to cycle through screens
- Type in setting with numbers, taking note of the unit used (milliseconds/seconds/etc.)
- Press "#" to save the setting to EEPROM, or press "\*" to clear entered value
- Turn key out of config position to go back to play mode

## Game play usage
- Use [this video](https://youtu.be/Dwf5lzY_378) as a rough guide or follow on-screen instructions
