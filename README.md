# The Sensational Sensors

## ASL Character Detection Glove

This is the code repository for our ECE167 final project, a glove that is
intended to detect American Sign Language (ASL) characters. It serves as a proof
of concept for further exploration of ASL parsing and translation, using a PIC32
Uno32 microcontroller.

All code that runs on the Uno32 is contained in the `ASL_Glove_Flex_Test.X`
folder. The main code within this folder is `Flexion_Test.c`, which is compiled
by the MPLAB X IDE to be programmed onto the microcontroller. Compilation can be
done using `make` in a terminal, but it is easier to use the "Make and program
device" feature after opening the folder in the IDE.

This code relies on the custom `Node.h/.c` library, contained within the
`Common` folder, with other library headers and source files. This library
contains all the necessary functions and data structures to create and interact
with the tree structure used for detecting characters. The library does not need
direct interaction, and is already included in compilation by the IDE.

The `matlab` folder contains the glove calibration MATLAB code, which compares
unfiltered and filtered flex sensor testing. The `Progress Images` folder
contains a couple images of our work on the data tree and traversal algorithm,
as well as graphs from the MATLAB code.
