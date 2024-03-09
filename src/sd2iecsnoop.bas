100 rem *********************
101 rem sd2iec snoop by exrom
102 rem https://github.com/exrom/sd2iec-snoop
103 rem licensed under the gnu general public license
104 rem *********************
105 rem
1000 rem ***********************
1001 rem          main
1002 rem ***********************
1020 da=8: rem initial device adress
1030 POKE53280,12:POKE53281,12
1040 GOSUB10000:rem menu
1050 GOSUB22000:rem status
1060 PRINT
1070 GETa$:IFa$=""THEN1070
1080 IFa$="{f1}"THENGOSUB20000
1090 IFasc(a$)=134THENGOSUB21000
1100 IFasc(a$)=135THENGOSUB24000
1110 IFasc(a$)=136THENGOSUB25000
1120 IFa$="$"THENGOSUB26000
1122 IFa$="c"THENGOSUB30100
1130 PRINT"<hit any key>"
1140 GETa$:IFa$=""THEN1140
1150 GOTO1030


10000 rem ***********************
10010 rem          menu
10020 rem ***********************
PRINT"{clear}";
PRINTtab(20)"{rvs on}{blue}                 "chr$(223)
PRINTtab(20)"{ctrl-r}  {green}  {blue} {green}  {blue}  {green}  {blue}  {orange}   {blue}  "chr$(223)
PRINTtab(20)"{ctrl-r} {green} {blue}   {green} {blue} {green} {blue}   {green} {blue}       "
PRINTtab(20)"{ctrl-r} {green}   {blue} {green} {blue} {green} {blue} {green}   {blue}  {orange}    {blue} "
PRINTtab(20)"{ctrl-r}   {green} {blue} {green} {blue} {green} {blue} {green} {blue}         "
PRINTtab(20)"{ctrl-r} {green}   {blue} {green}  {blue}  {green}   {blue}  {orange}    {blue} "
PRINTtab(20)"{ctrl-r}{blue}                   "
PRINTtab(20)"{ctrl-r} {green}   {blue}  {green}  {blue}  {green}  {blue}  {orange}    {blue} "
PRINTtab(20)"{ctrl-r}  {green} {blue}  {green} {blue}   {green} {blue}         "
PRINTtab(20)"{ctrl-r}  {green} {blue}  {green}   {blue} {green} {blue}    {orange}    {blue} "
PRINTtab(20)"{ctrl-r}  {green} {blue}  {green} {blue}   {green} {blue}         "
PRINTtab(20)"{ctrl-r} {green}   {blue}  {green}  {blue}  {green}  {blue}  {orange}    {blue} "
PRINTtab(20)"{ctrl-r}{blue}                   "
PRINT"{home}{blue}sd2iec snoop{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}{grey3}v0.4  by exrom{blue}{darkgrey}":PRINT
PRINT"{grey3}f1{darkgrey} show dos{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}version"
PRINT"{grey3}f3{darkgrey} change drive{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}number"da
PRINT"{grey3}f5{darkgrey} show sd2iec's{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}cbm commands"
PRINT"{grey3}f7{darkgrey} boot"chr$(34)"*"chr$(34)","chr$(48+da)",1":PRINT
PRINT"{grey3}c{darkgrey}  create empty d64{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}on sd2iec":PRINT
PRINT"https://github.com/exrom/sd2iec-snoop":PRINT
RETURN


20000 rem ***********************
20010 rem         version
20020 rem ***********************
20030 PRINT"dos version:"
20040 OPEN1,da,15
20050 PRINT#1,"ui"
20060 INPUT#1,a1$,a2$,a3$,a4$
20070 CLOSE1
20080 PRINTa1$","a2$","a3$","a4$
20090 IFNOTleft$(a2$,6)="sd2iec"THENRETURN
20100 OPEN1,da,15
20110 PRINT#1,"x?"
20120 INPUT#1,a1$,a2$,a3$,a4$
20130 PRINTa1$","a2$","a3$","a4$
20140 CLOSE1
20150 RETURN

21000 rem ***********************
21010 rem         adress
21020 rem ***********************
21030 PRINT"old device adress:"da
21040 INPUT"new device adress";nd
21050 IFnd<4ORnd>30THENRETURN
21060 PRINT"changing temporarily to"nd"..."
21070 OPEN1,da,15
21080 PRINT#1,"u0>"+chr$(nd)
21090 CLOSE1
21100 da=nd
21110 RETURN

22000 rem ***********************
22010 rem         status
22020 rem ***********************
22030 OPEN1,da,15
22040 INPUT#1,a1$,a2$,a3$,a4$
22050 PRINTa1$","a2$","a3$","a4$
22060 CLOSE1
22070 RETURN

23000 rem ***********************
23010 rem          rtc
23020 rem ***********************
23030 PRINT"- not implemented yet -"
23040 RETURN

24000 rem ***********************
24010 rem    show cbm commands
24020 rem ***********************
24030 PRINT:PRINT"{clear}some sd2iec cbm commands"
24032 PRINT"========================"
24040 PRINT:PRINT"change to sub directory foo"
24050 PRINT"open1,8,15,"chr$(34)"cd:foo"chr$(34)":close1"
24060 PRINT:PRINT"change to root directory"
24070 PRINT"open1,8,15,"chr$(34)"cd//"chr$(34)":close1"
24080 PRINT:PRINT"change to parent directory .."
24090 PRINT"open1,8,15,"chr$(34)"cd{$5f}"chr$(34)":close1"
24100 PRINT:PRINT"change drive address temporarily"
24110 PRINT"open 1,8,15,"chr$(34)"U0>9"chr$(34)":close1"
24120 PRINT:PRINT"find complete docu in 'readme' file of"
24130 PRINT"git web interface at www.sd2iec.de"
24140 RETURN

25000 rem ***********************
25010 rem          boot
25020 rem ***********************
25030 PRINT:PRINTchr$(17)"load"chr$(34)"*"chr$(34)","da",1";:PRINTchr$(145)chr$(145)chr$(145)
25040 FORi=1to1541:NEXT
25050 POKE631,131:POKE198,1
25060 END

26000 rem ***********************
26010 rem   sd2iec fake output
26020 rem ***********************
26040 PRINT"dos version:"
26050 PRINT"73,sd2iec v1.0.0atentdead0-36,00,00"
26060 PRINT"09,sd2iec v1.0.0atentdead0-36-m1284p-sw2,00,00"
26070 RETURN


30100 rem ***********************
30101 rem   create d64 image
30102 rem ***********************
30105 PRINT"{clear}create empty d64 image"
30106 PRINT"======================":PRINT
#           <----- 40 columns screen width --------><----- 40 columns screen width -------->
30110 PRINT"an empty d64 image file with 174848     bytes in size will be created."
30112 PRINT"this will work only on a real c64       with an sd2iec attached."
30114 PRINT"make sure enough free space is availableand no d64 is currently mounted."
30122 PRINT:PRINT"continue? {grey3}y{darkgrey}es"
30124 GETa$:ifa$=""THEN30124
30126 IFa$<>"y"THENRETURN
30127 PRINT
30130 PRINT"name of image    blank.d64{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}";:inputin$
30140 PRINT"device address  "da"{cursor left}{cursor left}{cursor left}{cursor left}";:inputdr
30150 PRINT"disc label       empty{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}";:inputdn$
30160 PRINT
30170 PRINT"creating "in$" on drive"dr"..."
30180 OPEN1,dr,3,in$+",p,w"
30190 OPEN2,dr,15,"p"+chr$(3)+chr$(233)+chr$(170)+chr$(2)+chr$(0)
30200 PRINT#1,"created by sn2iec snoop";
30210 CLOSE1
30220 IFst<>0thenPRINT"error creating image!":CLOSE2:RETURN
30230 PRINT"mounting..."
30240 PRINT#2,"cd:"+in$
30245 IFst<>0THENPRINT"error mounting image!":CLOSE2:RETURN
30248 PRINT"formatting..."
30250 PRINT#2,"n:"+dn$+",00"
30255 PRINT"unmounting..."
30260 PRINT#2,"cd{$5f}"
30270 CLOSE2
30280 PRINT:PRINT"done. enjoy using "in$
30290 RETURN
