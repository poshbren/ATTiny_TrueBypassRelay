# True Bypass Relay for DIY Guitar Pedals / Effects

A beginner friendly intro to programming / microcontrollers for DIY pedal enthusiasts.

This repo should give you everything you need to add digital relay controls to your projects. This is a fully DIY project. At this time I do not sell pre-programed or pre-fab boards.
This repo will give you the files needed to get your own PCBs assembled / programmed and to include the framework into your own custom PCBs.

## Features

* Quick press to toggle Effect On / Bypass.
* Hold down when off to temporarily engage effect.
* Hold down when on to temporarily bypass effect.
* Remembers settings when powered off.
* Programmable Status LED. One built in or control the LED on a parent PCB if using PedalPCB boards.
* Wiring is directly compatable with PedalPCB.com kits.
* Rated up to 6.4 million cycles. (Can be extended in program but that will be overkill)
* Fully open source. Use in your DIY pedals or adapt into your commercial projects.

## What you will need

* An Arduino or dedicated UPDI programmer. Recommended: Adafruit UPDI Friend [Adafruit UPDI Friend](https://www.adafruit.com/product/5879)
* A SPST Momentary Foot Switch / Button - Normally Open. Recommended: [Love My Switches Pro Footswitch](https://lovemyswitches.com/spst-momentary-foot-switch-normally-open-soft-touch-pre-wired-with-connector/)
* The PCB. JLCPCB Production Files and BOM are provided. Can easily be modified to your preffered PCB vendor.
* The Arduino IDE [Arduino.cc](https://www.arduino.cc/en/software/)
* (Optional but recommended) JST SH Compatible 1mm Pitch 3-pin Plug-Plug Cable for programming. [JST Cable from Adafruit](https://www.adafruit.com/product/6404)

## Userguide

See Guide.PDF in main repo.

## PCB Assembly Guide

The included production files are in a JLCPCB friendly format. Other manufacturers can be used but the BOM file may need to be adjusted.

### Production Tips

The included pcb layout does not include all the markers needed for fabrication. Due to the small board size it is impractical to add tooling marks and fiducials directly to the PCB. Instead look for these options when placing your PCB order:

* Delivery Format - If ordering in bulk choose "Panel by JLCPCB" or your preffered manufacturer. This will make assembly much easier for the manufacturer. 
* Edge Rails/Fiducials - These must be added for assembly. Choose "Added by JLCPCB". Due to the small board size they may ask to panelize the boards for assembly.
* Confirm Parts Placement - Through JLCPCB's ordering you should have a chance to check the positions / rotations of the components before assembly. In either case select this option to ensure correct placement.
* All other settings can be kept as default. Change the board color to your preference. 

## Programming

1. Download and install the Arduino IDE [Arduino.cc](https://www.arduino.cc/en/software/)
2. Download a zip file of this Repo, unzip the folder and re-name to TrueBypassRelay (Arduino needs the parent folder to have the same name as the .ino file)
3. Navigate to the Arduino folder that was installed with the Arduino IDA. Copy the TrueBypassRelay into the Arduino folder.
4. Open the truebypassrelay.ino file. This will open up the Arduino IDE. 
5. If using an Arduino Uno / Nano for programming follow the steps here https://github.com/ElTangas/jtag2updi.git. Note that the board must be programmed with 5V logic. 3.3V logic will not work.
6. If you are using the Adafruit UPDI Friend or a similar UPDI programmer follow the guide here https://learn.adafruit.com/adafruit-updi-friend/advanced-reprogramming-with-updi 
7. This board uses the ATtiny412 make sure the board chip is set to "ATtiny412" and the clock speed is set to "20 MHz internal". For everything else follow the guide.
8. Upload the Arduino sketch using the "Upload Using Programmer" option. Your relay board is now ready to use!

Notes: The True Bypass Relay board can be programmed "in circuit" meaning you can wire it to your guitar pedal and still program the board. Re-program as many times as you like.
