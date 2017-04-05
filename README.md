# Led lamp dimmer with touch buttons
Led lamp controller with touch dimmer function.
There are 2 touch buttons that increase and decrease brightness. Brightness is controller via PWM output using power mosfet.
Dimming function has 128 levels mapped via exponential function to 10bits of PWM.

Project is developed in MPLAB X IDE v3.51.
Microcontroller used is Microchip PIC16F1823.
Unfortunately mtouch library consumed almost all resources, and I had to manually optimize it. Side effect is that MPLAB Code Configurator can no longer automatically merge changes made in gui. 
It would be better to get microcontroller with 2 times more memory, but it's just what I had in my parts bin.

## PCB and schematic
There is small board prepared, and sent to manufacturing by itead.cc.
Schematic, and board was drew in Eagle. Files are in pcb folder.
Production files are prepared in Prototype1.zip.

I didn't get PCB yet, so it is not verified.

This is still work in progress.
Right now circuit works on breadboard.

## Code statistics
- data: 101/128 bytes used (79%)
- code: 2021/2048 words used (99%)

## Next tasks:
- optimize libraries further to make more place for application code.
- add quick on/off function (right now brightness can be lowered to 0)
- add schematic, and design PCB - done
