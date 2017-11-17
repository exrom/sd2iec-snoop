# SD2IEC Snoop

SD2IEC Snoop is a configuration utility for the sd2iec floppy drive emulator for the C64. 

The DOS commands implemented by sd2iec are hard to remember and complicated to type.
SD2IEC Snoop is a commodore program running on the real C64. It shows a menu that provides the following functions.

* get sd2iec version
* change device address
* set real time clock
* show a help page with the raw DOS commands (in case you still want to type them)

![screenshot](/images/sd2iecsnoop.png)


# how to

The source of the basic V2 program is saved as ascii text in the repository for easier reading, diffing etc.
VICE is used for the conversion between txt and the memory representation in the C64 (prg).

## convert from txt to prg

Open a destination d64 in vice or configure the disk for redirection to a folder on your pc directly.
Copy txt file contents to clipboard. In VICE, type new, select paste, and type save"sd2iec snoop",8

Note that the text file contains abbreviated basic commands like cH (chr$) or ? (print). This is because control codes doncannot be pasted. Also note, the basic line may finally be larger than 80 characters when listed.

## convert from prg to txt

VICE can copy the screen content into the clipboard, but the program won't fit on the screen.
To get the whole program exported at once an emulated printer can be used.
Configure a printer in VICE and let it print to a file (default: viceprnt.out) and then type


open1,4,7:cmd1,"listing":list
close1

This viceprnt.out file contains LF at the end of each line (unix line ending on a pc) and CR in the next line. Remove the CR (\r).


# contribute

Feel free to fork this repository on github https://github.com/exrom/sd2iec-snoop change the program and create a pull request

# links

sd2iec wiki     https://www.c64-wiki.com/wiki/SD2IEC
sd2iec firmware https://sd2iec.de/


# credits
all contributors of sd2iec-snoop, especially Ingo Korb for his continuous efforts
