# Animator-inator
Animation Software and Firmware for the Robotics Club Dot Matrix Display. To access the browser-based animation software,
visit [nirav.com.np/Animator-inator](https://nirav.com.np/Animator-inator).

## Introduction
This repository contains a firmware to Display animations in a 32x32 combination of two 32x16 P10(1r)-V70 
Dot Matrix Displays. And it also contains a small JavaScript program that you can use to make animations
for the Dot Matrix Display setup. The program runs on your browser, and exports a header file that you can
compile your firmware with and flash into the AVR.

Read [this post](https://nirav.com.np/2019/06/12/animating-led-dot-matrix-displays-with-avr.html) for all details.

[![screenshot of animator-inator](https://nirav.com.np/assets/img/Screenshot-2019-6-15%20Animator-inator.png)](https://nirav.com.np/Animator-inator)

## How to use?
The overall procedure varies from set-up to set-up. It would be extremely coincidental if you could use these
programs as-is without modification. However, the general procedure should be:

1. Git clone this repository or download the zip file.
2. Hit `./upload.sh dmd.c` to compile and upload the program. You might have to modify the `upload.sh`
	file to suit your In-System Programmer and AVR device. 
3. Use the animation program to make animations. It should be fairly intuitive. Once you are done, hit 
`Ctrl+Shift+S` to save the file as `matrixdata.h` in the root directory of the project, replacing the original one.
4. Flash and repeat.

## Other works used
The animation software relies on [FileSaver.js](https://github.com/eligrey/FileSaver.js/) written by [Eli Gray](https://eligrey.com/) and released under MIT license.
