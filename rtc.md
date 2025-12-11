# RTC

Find here a few investigations and findings around the RTC commands. 
The basic snippets below can be clipboard pasted into VICE. 
SD2IEC can be connected to VICE emulator using a XUM1541.

All tests, especially the day of week calculation were done with SD2IEC firmware  
    73,sd2iec v1.0.0atentdead0-20,00,00  
    09,sd2iec v1.0.0atentdead0-20-m644p-sw2-mini,00,00   
and an NKC electronics Rev C sd2iec V1.2 with an AZDelivery DS3231 module attached.

The basic snippets below can be clipboard pasted into VICE.

# SD2IEC RTC commands

RTC can be set with this one-liner:

open1,8,15,"t-wafri. 11/28/25 03:45:30 pm":close1  
Read commands need a basic program due to input (illegal direct error) and gotos.

## read decimal 

new  
10 open1,8,15,"t-rd"  
20 get#1,a$  
25 printasc(a$);  
30 ifa$<>chr$(13)then20  
40 close1  
run  
 5  125  11  28  3  45  30  1  13  

## read bcd

new  
10 open1,8,15,"t-rb"  
20 get#1,a$  
25 printasc(a$);  
30 ifa$<>chr$(13)then20  
40 close1  
run  
 5  37  17  40  3  69  50  1  13  

## read iso string

new  
10 open1,8,15,"t-ri"  
20 get#1,a$  
25 printa$;  
30 ifa$<>chr$(13)then20  
40 close1  
run  
2025-11-28t15:45:30 fri  

#### read ascii string

new  
10 open1,8,15,"t-ra"  
20 get#1,a$  
25 printa$;  
30 ifa$<>chr$(13)then20  
40 close1  
run  
fri. 11/28/25 03:45:31 pm  

##### set time ascii

open1,8,15,"t-wafri. 12/31/25 11:59:00 pm": close 1  

SD2IEC will accept ascii string only if dow string is an allowed one but not verify if it matches the date (no internal verification). 
. Spacing characters '.', ':', '/' are ignored. Else than 'PM' defaults to 'AM'. Hour 13-23 is accepted only if not 'PM'.
Nonsens dates such as 2/31/xx are accepted. 

# set time decimal

new  
0 do=0  
1 yr=25   
2 mo=12  
3 dy=31  
4 hr=23  
5 mi=59  
6 se=11  
7 ap=0  
10 ts$=cH(do)+cH(yr)+cH(mo)+cH(dy)+cH(hr)+cH(mi)+cH(se)+cH(ap)  
20 oP1,8,15,"t-wd"+ts$  
30 close1  
40 open1,8,15,"t-ri"  
50 get#1,a$  
60 printa$;  
70 ifa$<>chr$(13)then50  
80 close1  
run  
sun. 12/31/25 11:59:11 pm  

Day of week is provided by the format. sd2iec accepts it but does not verify it.

# set time iso

oP1,8,15,"t-wi2025-11-28t13:23:45 sun":clO1:rU  

Though the day of week is part of the format, sd2iec completely ignores it and calculates dow itself.

sd2iec also accepts  
oP1,8,15,"t-wi2025-11-28t23:59:59 xxx":clO1:rU  
oP1,8,15,"t-wi2025-11-28t23:59:59 ":clO1:rU  
oP1,8,15,"t-wi2025 11 28 23 59 59":clO1:rU  

Interestingly, the calculated dow is not always correct. 
oP1,8,15,"t-wi2025-11-28t23:59:59 ":clO1:rU  
mon. 11/28/25 11:59:59 pm  
28th november 2025 is a friday, not a monday.  

# day of week calculation

As far as it seems and as far as i understand sd2iec's firmware, it calculates the day of week itself only if iso format is used for rtc setting. No idea why, there may be reasons.  
I cross-checked the calculated output using libre office calc and =CHOOSE(WEEKDAY(A1);"SUN";"MON";"TUES";"WED";"THUR";"FRI";"SAT")

With this simple basic program one can check the output 

new  
10 for yr=2010 to 2028  
20 yr$=str$(yr)  
30 open1,8,15,"t-wi"+yr$+"-12-31t00:00:00 xxx":close1  
40 open1,8,15,"t-ri"  
50 get#1,a$  
60 printa$;  
70 ifa$<>chr$(13)then50  
80 close1  
90 next  
run  
2010-12-31t00:00:00 fri   
2011-12-31t00:00:00 sat   
2012-12-31t00:00:00 mon  
2013-12-31t00:00:00 tue  
2014-12-31t00:00:00 wed  
2015-12-31t00:00:00 thu  
2016-12-31t00:00:00 sat  
2017-12-31t00:00:00 sun   
2018-12-31t00:00:00 mon  
2019-12-31t00:00:00 fri   wrong. it's a TUES  
2020-12-31t00:00:00 sun   wrong. it's a THUR  
2021-12-31t00:00:00 mon   wrong. it's a FRI  
2022-12-31t00:00:00 tue   wrong. it's a SAT   
2023-12-31t00:00:00 wed   wrong. it's a SUN  
2024-12-31t00:00:00 fri   wrong. it's a TUES  
2025-12-31t00:00:00 sat   wrong. it's a WED  
2026-12-31t00:00:00 sun   wrong. it's a THUR  
2027-12-31t00:00:00 mon   wrong. it's a FRI  
2028-12-31t00:00:00 wed   wrong. it's a SUN  

The calculation seems still ok for 1st january of each year but not for the 31th december.

Leap year calculation (day after 28th feb each year) seems handled correct. Checked for 2025-2045.

When dow is given by other formats, sd2iec takes it unchecked and increases it correctly on date change.  
So, one could think about not using iso format, self-calc dow to work around this issue.


# time format used by sd2iec

Sd2iec snoop uses iso format to set RTC because other commands cause trouble when one of the data fields is zero. 
A zero in a command string is not a problem for commodore DOS (they are terminated by chr$(13)) but it is seen as string terminator in C.
The functions lib function cbm_open() used by sd2iec snoop stops sending data to drive if zero is in the middle of the string. 
To solve this would mean to reimplement this function which is not worth the effort.
To use iso format is also simpler as no string to integer calculation is needed and helps to keep program size small.

sd2iec snoop does not try to work around the dow bug because this would cause more effort (and program complexity) on c64 side.


### other rtc tools reference

https://sd2iec-lcd.com/wp-content/uploads/2025/05/sd2iec_pack.zip  
    configurator.prg    Configurator version 0.3   (c) Back Friderik 2021-2022   
    set rtc.prg         set real-time clock by doug cotton      (c) 1990 by creative micro designs, inc.    

