# Snes2PC
Small Arduino Sketch to convert a Super Nintendo (SNES) Controller to an HID peripheral using a Raspberrry Pi Pico board.

For now we added the mincraft keybinds

## Hardware setup
SNES Controller Pinout
<pre>
 -----------------\
| 1 2 3 4 | 5 6 7  |
 -----------------/</pre>
 <ul>
  <li>Pin 1: 5V</li>
  <li>Pin 2: Data_Clock (Pico pin 13)</li>
  <li>Pin 3: Data_Latch (Pico pin 12)</li>
  <li>Pin 4: Data_Serial (Pico pin 11)</li>
  <li>Pin 5: N/A</li>
  <li>Pin 6: N/A</li>
  <li>Pin 7: GND</li>
 </ul>
 
 And you need to connect an RGB led (WS2812b) to the pin 10 of the Pico

## Snes viewer
The app runs on tkinter and Pillow, cou can install them with:
`pip install tk Pillow` or `pip3 install tk Pillow`

And run it with:
`python ./SnesViewer/app.py` or `python3 ./SnesViewer/app.py`

### 3D
You can find all the 3d files in the 3D folder
the Raspberry Pico and the Neopixel led are placed using hot glue

### Credits
This project was heavily inspired by :
https://github.com/burks10/Arduino-SNES-Controller
