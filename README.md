# Led lamp dimmer with touch buttons
Led lamp controller with touch dimmer function.
There are 2 touch buttons that increase and decrease brightness. Brightness is controller via PWM output using power mosfet.
Dimming function has 128 levels mapped via exponential function to 10bits of PWM.
Pressing any button for less than about 0.5s turns lamp off. Pressing any button again turns lamp on with previous brightness level saved.

Project is developed in MPLAB X IDE v3.51.
Microcontroller used is Microchip PIC16F1823.
Unfortunately mtouch library consumed almost all resources, and I had to manually optimize it. Side effect is that MPLAB Code Configurator can no longer automatically merge changes made in gui. 
It would be better to get microcontroller with 2 times more memory, but it's just what I had in my parts bin.

## PCB and schematic
There is small board prepared, and sent to manufacturing by itead.cc.
Schematic, and board was drew in Eagle. Files are in pcb folder.
Production files are prepared in Prototype1.zip.

Boards were manufactured in cheap prototyping pool in ITEAD.cc
Here is a board assembled with dummy load connected:
![Board with dummy load](/pcb/pcb_with_dummy_load.jpg)

Main tasks were completed, but this is still work in progress.

## Code statistics
Compiled with free version of xc8 v1.40
- data: 100/128 bytes used (78%)
- code: 1988/2048 words used (97%)

## Next tasks:
- [x] optimize libraries further to make more place for application code.
- [x] add quick on/off function (right now brightness can be lowered to 0)
- [x] add schematic, and design PCB
- [ ] mount to bed with hidden touch buttons, and connect IKEA led lamps

## Fail 1 - lesson learned
When I run prototype at high power, transistor drain pins were getting extremely hot - I measured 50 C over ambient temperature.
I immediately blamed power transistor losses, because I used different type than in breadboard prototype. After trying 3 other power transistors from my scrap bin, and thinking about better gate drive, and attaching heat sink I spotted that R5 is 100 ohm instead of 100kOhm.
First I thought that these are switching losses. I checked if transistor is turning on correctly, and it seemed fine. Just to be sure I lowered gate resistor to 11 Ohm.
Then I correlated temperature rise with power output, and checked Rds_on (indirectly by measuring Vds with oscillosope, and looking on average current on power supply). This also looked ok.
I lost couple hours before I noticed incorrect resistor value. It also helped to notice that the transistor case was unporportionaly colder than it should be at that high drain leads temperature. Probably source leads were sinking enough heat to keep plastic case cool.
After all that I rised gate resistor back to 47 Ohms to prevent microcontroller output latchup (it's already quite stressed - peak current when turning on is around 100mA for 80ns).
Here are Vd rise and fall waveforms:
![Vd turn on](/doc/Vd_turn_on.png) ![Vd turn off](/doc/Vd_turn_off.png)
Vg gate waveforms:
![Vg turn on](/doc/Vg_turn_on.png) ![Vg turn off](/doc/Vg_turn_off.png)
This is Vd, and I can't quite explain slow rise after turn off - possibly it's capacitance in LEDs.
![Vd waveform](/doc/Vd.png) 