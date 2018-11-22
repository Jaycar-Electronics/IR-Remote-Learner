# IR Remote Learner
With our IR Remote Learner kit, you will be able to learn the custom codes of those hard to replace remotes. Running off a 9V battery, this portable unit includes a keypad and well-lit display to make the job easy.									

## Bill of Materials
| Qty | Code | Description |
| --- | --- |---|
|1 | [XC4616](http://jaycar.com.au/p/XC4616) | Nokia Screen
|1 | [XC4426](http://jaycar.com.au/p/XC4426) | Infrared Transmitter
|1 | [XC4427](http://jaycar.com.au/p/XC4427) | Infrared Receiver
|1 | [XC4414](http://jaycar.com.au/p/XC4414) | Arduino Nano
|1 | [HP9550](http://jaycar.com.au/p/HP9550) | PCB Board
|1 | [XC4602](http://jaycar.com.au/p/XC4602) | Touch Keypad
|1 | [PH9235](http://jaycar.com.au/p/PH9235) | 9V Battery Holder
|1 | [SS0834](http://jaycar.com.au/p/SS0834) | Micro slide switch
|1 | [WB2022](http://jaycar.com.au/p/WB2022) | Cat5 solid core cable.

###### Optional Extras:
* [WW4346](http://jaycar.com.au/p/WW4346) Blue Wire wrap (other colours available)


## Software and libraries

The code was developed with these versions, your versions and results may be different.

| Library | Author | Version |
| --- | --- | --- |
| Adafruit_GFX | Adafruit | 1.0.0
| Adafruit_PCD8544 | Adafruit | 1.2.9
| IRRemote | sheriff | 2.2.3
| EEPROM | Arduino | 1.8.5 (with ide) |

## Connection table
| Nano | Module | Pin |  Description
| --- | --- | --- | --- |
| 2 | IR Receiver | S | sense wire for IR receiver module |
| 3 | IR Transmitter | S | Output wire for IR Transmitter |
| 4 | Keypad | SCL | Keypad signal clock |
| 5 | Keypad | SDO | Data Output for Keypad |
| 6 | LCD | DC  | LCD Data select |
| 7 | LCD | RST | LCD reset  |
| 8 | LCD | SCE | LCD chip select  |
| 11 | LCD | DN(MOSI) | LCD SPI Data bus |
| 13 | LCD | CLK | LCD SPI Data clock |

###### Power:
| Nano | Connects to | Done? |
| ---- | ---- | --- |
| 5V | LCD_VCC | [ ] |
| | IRRecv_VCC (middle pin) | [ ] |
| | Keypad VCC |  [ ] |
| GND | LCD_GND  | [ ] |
| | Keypad GND| [ ] |
| | IRRecv_GND (-) | [ ] |
| | IRTrans_GND (-) | [ ]|
| | 9V Battery Negative | [ ]  |

* 9V Battery positive rail will connect to the VIN pin on the nano, through the SS0834 micro switch.


## Assembly

1. make battery connectors by poking wires through, twisting around, and back down
2. solder in place.
3. place components, like headers, etc. as picture
4. put in arduino, sensors, LCD screen
5. battery goes on second last, solder in place, snip to size.
6. screw if needed.
6. put keypad over battery terminals
7. run code.
