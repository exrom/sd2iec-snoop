# SD2IEC Snoop

SD2IEC Snoop is a configuration utility for the sd2iec floppy drive emulator for the C64. 

The DOS commands implemented by sd2iec are hard to remember and complicated to type.
SD2IEC Snoop is a basic program running on the real C64. It shows a menu that provides the following functions.

* get sd2iec version
* change device address
* create d64 image file
* load+run 1st PRG on the SD card
* show a help page with the raw DOS commands (in case you still want to type them)

![screenshot](/images/sd2iecsnoop.png)

It is meant to be placed as second file on the SD card as the first file typically is a file browser.


## Known issues

When sd2iec's drive address is changed permanantly, one should change the *initial device adress* in the program.  
If no device listens to 8, the program hangs at the input#1 line.


## contribute

This project is developed with VS64.
Feel free to fork this repository and create a pull request.

## links

sd2iec wiki     https://www.c64-wiki.com/wiki/SD2IEC
sd2iec firmware https://sd2iec.de/


# credits

Ingo Korb for his continuous efforts on SD2IEC  
rolandshacks for the great VS64 development environment extension for Visual Studio Code

