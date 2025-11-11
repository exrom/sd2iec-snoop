# Overview

SD2IEC Snoop is a helper tool for the sd2iec floppy drive emulator running on the C64. 

![scroller](/images/scroller.apng)

It simplifies several tasks on the sd2iec , e.g.:

* get sd2iec's version
* change device address
* create a new d64 image file
* set and show real time clock - if supported by sd2iec

# Main menu

Most menu items are self explaining. Explanation for the others...

## get drive versions

This will query all possible disk drive numbers 8...11 and show drive's response to the UI command.
This gives an overview which drives currently are actively listening on the cbm bus.
In case of sd2iec, also the extended version (cmd X?) is shown.

Some commands which aim to a certain drive will use what is shown as 'current drive' on the main menu. SD2IEC Snoop will auto-set the current drive to the number where an sd2iec was detected.

## change drive id

This changes the device number in the drive's RAM using the u0> command. This works on both cbm drives and sd2iec. 
Choose a free device number to avoid collisions. 
You may connect a 1541 and an sd2iec to the bus (both listening to drive address 8) and switch the 1541 off. Then change sd2iec's address to 9 and switch 1541 back on. Check the new drive id with 'drive versions'. This feature does not work in VICE 3.9.

## create empty d64

This will create an empty d64 image (file with 174848 bytes). The file name and the disc title can be chosen.
This features works only on a real C64 with an sd2iec attached. It uses the current drive number shown in main menu.
Make sure you have no d64 already mounted on the sd2iec (because it makes no sense to create a d64 inside a d64...).
Make sure, the inserted SD card is formatted, writeable and has >171KiB (638 blocks) free space.
Afterwards, this new d64 is mounted and ready for use. 

## boot from disk

This will execute the command  
    load":*",<current drive>,1
and terminate sd2iec snoop thereby. May be useful if 1st file on a d64 is a file browser.

## set rtc

Can display (TODO and set) time+date of sd2iec's real time clock. Works only if sd2iec firmware and hardware supports RTC.

# Remarks

SD2IEC Snoop is written in C using the CC65 environment. Is uses Visual Studio Code with the VS64 extension.  
The drive interaction is done by kernal cbm commands and there are as few as possible c64 specialities used, so porting to other systems (vic20, pet,...) should be easy.

Former versions of SD2IEC Snoop were written in basic and are no longer maintained but kept for reference in sub folder 'basic'.

If you have bug reports or feature suggestions, get in contact at https://github.com/exrom/sd2iec-snoop

## credits/links

sd2iec firmware                     https://sd2iec.de/  
sd2iec wiki                         https://www.c64-wiki.com/wiki/SD2IEC  
code plugin VS64 by rolandshacks    https://marketplace.visualstudio.com/items?itemName=rosc.vs64  
cc65 c6502 C compiler project       https://cc65.github.io/  
cbm bus docu by mist64              https://www.pagetable.com/?p=1018  
german forum                        https://www.forum64.de  

