You can plug the boards into your printers control board but I wanted external control to move all timing sensitive commands off of the Raspberry Pi as to not interfere with printing. That's why I made the 4 bit parallel interface code as well as the corresponding decoder on the Arduino. If Klipper adds some direct I2C commands in the future I will most definitely switch to that for ease of coding and simplicity.

This is just **MY** example code, feel free to add, modify, or write your own to control the colors. Please share it back here; I’d love to see what you create!