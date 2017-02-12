# Led lamp dimmer with touch buttons
Led lamp controller with touch dimmer function.
There are 2 touch buttons that increase and decrease brightness. Brightness is controller via PWM output using power mosfet.

Microcontroller used is Microchip PIC16F1823.
Unfortunately mtouch library consumed almost all resources, and I had to manually optimize it. Side effect is that MPLAB Code Configurator can no longer automatically merge changes made in gui.

This is still work in progress.

Right now circuit works on breadboard.

Next tasks:
- add quick on/off function (right now brightness can be lowered to 0)
- add schematic, and design PCB
