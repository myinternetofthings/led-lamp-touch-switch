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
