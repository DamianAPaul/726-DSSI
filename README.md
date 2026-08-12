# 726 Digital Sound Sequencing Instrument (726-DSSI)

A standalone digital synthesizer and 64-step sequencer designed around the Teensy 4.1.

The 726-DSSI is a personal embedded systems project combining real-time digital audio processing, custom C++ firmware, PCB design, physical controls, sequencing, and visual feedback.

## Features

- Up to 8 simultaneous oscillator, noise, or sample-based voices
- Per-voice filtering and amplitude envelopes
- 64-step sequencer
- Teensy 4.1 microcontroller
- PJRC Audio Shield
- SSD1306 OLED display
- MAX7219 8x8 LED matrix
- MCP23017 GPIO expansion
- CD74HC4067 analog multiplexing
- 3x4 keyboard matrix
- Custom C++ firmware using the Teensy Audio Library

## Hardware

I designed the schematic and PCB in KiCad, including custom schematic symbols and PCB footprints for several project-specific modules.

The first PCB revision has been fabricated, assembled, and validated; and is now awaiting shipping.

## Firmware

The firmware is written in C++ for the Teensy 4.1 using the Teensy Audio Library.

The firmware handles:

- Real-time audio synthesis and processing
- Voice management
- Sequencing
- Control scanning
- Analog multiplexing
- Keyboard input
- OLED and LED matrix feedback

## Repository Structure

- `726-DSSI_Firmware/` — Teensy C++ firmware
- `726-DSSI_CAD/` — KiCad schematic, PCB design files, custom footprint library, and gerbers
- `726-DSSI_Images/` — project images, mainly from prototyping phase

## Project Status

The breadboard prototype has been used for firmware and subsystem development. The first custom PCB revision has been fabricated, validated, and is awaiting shipping.

## Author

Damian Paul  
University of California, Berkeley  
Physics | Electrical Engineering & Computer Science
