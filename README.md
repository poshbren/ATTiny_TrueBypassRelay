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

