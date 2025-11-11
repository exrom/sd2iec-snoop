# Developers notes

If you prefer fork the project rather than creating an issue/pull request, please keep original version number, add your 3-char name postfix to the version.

# Picture slide show

Start VICE, Drive 8: Real/OpenCBM+ZoomFloppy+SD2IEC, Drive 9 1541 TrueDrive  
Take screenshots (pause) --> png  
ffmpeg -f image2 -framerate 0.33 -pattern_type glob -i '*.png' -vf scale=768:544 -plays 0 -f apng images/scroller.apng  

# make release 

c1541 -format "sd2iec snoop,25" d64 sd2iec_snoop.d64 -attach sd2iec_snoop.d64 -write build/sd2iecsnoop.prg "sd2iec snoop 1.0"  
c1541 -attach sd2iec_snoop.d64 -write . "UCCCCCCCCCCCCCCI,s"  
c1541 -attach sd2iec_snoop.d64 -write . "B  by  exrom   B,s"  
c1541 -attach sd2iec_snoop.d64 -write . "JCCCCCCCCCCCCCCK,s"  
