## Your Project Answers

### Project Description

The goal of this project is to design a painting controller for a python GUI. This means that the user would be able to draw designs of their choice, using a joystick to control the position of the paintbrush on the GUI. In the GUI, the user will be able to begin our drawing game. The artist will be presented with a word to draw and then the countdown timer will begin decrementing. The artist will be able to draw using the microcontroller and the button functionality as described above. Once the timer finishes, the artist will not be able to edit the drawing anymore, and can restart the game if they would like. 

The five buttons will be used for the following: One to toggle through paint colors, one to act as the “press to draw” button, one to act as the “press to erase” button, a start button, and a reset button. The 4-digit 7 segment display will be used as the timer for the drawing board. We can control the RGB LED using dedicated PWM pins to display the color that is being displayed in the GUI. This way, there is a visual indicator for the user to tell which color they are currently using.
### Technical Approach

In terms of the equipment needed for this project, we are planning on using a joystick module, 5 buttons, RGB LED, a 4-digit 7 segment display, a breadboard, and the FRDM-KL46Z microcontroller that we already have. Our group has access to all materials mentioned.

We will use the board’s dedicated ADC inputs to read the analog signal from the joystick, the board’s dedicated PWM pins to power the RGB LED, and other GPIO pins for digital reading from the buttons. We can connect to our laptop using the board’s dedicated serial port over USB, and our GUI will be built in python.
In terms of the equipment needed for this project, we are planning on using a joystick module, 5 buttons, RGB LED, a 4-digit 7 segment display, a breadboard, and the FRDM-KL46Z microcontroller that we already have. Our group has access to all materials mentioned.

We will use the board’s dedicated ADC inputs to read the analog signal from the joystick, the board’s dedicated PWM pins to power the RGB LED, and other GPIO pins for digital reading from the buttons. We can connect to our laptop using the board’s dedicated serial port over USB, and our GUI will be built in python.

## Feedback.

This project has a lot of hardware components! I like the idea of mathcing the RGB led to the drawing color. Just in terms of working on the whole systems, you should first focus on the core functionality and then add more features later. It seem like the joystick is going to be main thing to show that the drawing game works. 

Is the joystick module based on potentiometers or does it output voltages? Double check that it works on a 3.3V system. Specifically, if it is a 5V module add appropriate voltage dividers if neccesary. 

## Welcome team! Please edit me.
### You can use the 'main' branch for you code.
### The GitHub web-page should be in the 'gh-pages' branch
You can access the page adding a "pages" prefix to the url for the git repo e.g. if your browser currently shows https://github.coecis.cornell.edu/ece3140-sp22/ehs86-mck65-tws76 then the GitHub page is at https://pages.github.coecis.cornell.edu/ece3140-sp22/ehs86-mck65-tws76 . You can edit the source in the gh-pages branch or use the "automatic page generator" acessible via settings->options in the GitHub Pages Section toward the bottom. We recommend the latter approach since it is quick and easy. We made an initial wepage that you can edit and a description of the headings we expect to see.
