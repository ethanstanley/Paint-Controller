## Your Project Answers

### Project Description

The goal of this project is to design a painting controller for a python GUI. This means that the user would be able to draw designs of their choice, using a joystick to control the position of the paintbrush on the GUI. In the GUI, the user will be able to begin our drawing game. The artist will be presented with a word to draw and then the countdown timer will begin decrementing. The artist will be able to draw using the microcontroller and the button functionality as described above. Once the timer finishes, the artist will not be able to edit the drawing anymore, and can restart the game if they would like. 

The five buttons will be used for the following: One to toggle through paint colors, one to act as the “press to draw” button, one to act as the “press to erase” button, a start button, and a reset button. The 4-digit 7 segment display will be used as the timer for the drawing board. We can control the RGB LED using dedicated PWM pins to display the color that is being displayed in the GUI. This way, there is a visual indicator for the user to tell which color they are currently using.
### Technical Approach

In terms of the equipment needed for this project, we are planning on using a joystick module, 5 buttons, RGB LED, a 4-digit 7 segment display, a breadboard, and the FRDM-KL46Z microcontroller that we already have. Our group has access to all materials mentioned.

We will use the board’s dedicated ADC inputs to read the analog signal from the joystick, the board’s dedicated PWM pins to power the RGB LED, and other GPIO pins for digital reading from the buttons. We can connect to our laptop using the board’s dedicated serial port over USB, and our GUI will be built in python.
In terms of the equipment needed for this project, we are planning on using a joystick module, 5 buttons, RGB LED, a 4-digit 7 segment display, a breadboard, and the FRDM-KL46Z microcontroller that we already have. Our group has access to all materials mentioned.

We will use the board’s dedicated ADC inputs to read the analog signal from the joystick, the board’s dedicated PWM pins to power the RGB LED, and other GPIO pins for digital reading from the buttons. We can connect to our laptop using the board’s dedicated serial port over USB, and our GUI will be built in python.

Check our website out with the video at https://pages.github.coecis.cornell.edu/ece3140-sp22/ehs86-mck65-tws76
