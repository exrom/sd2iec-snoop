rem sd2iec snoop by exrom
rem github.com/exrom/sd2iec-snoop
rem licensed under the GPL
rem
main:
100 rem ******** main **********
da=8: rem initial device adress
poke53280,12:poke53281,12
GOSUB menu
GOSUB status
PRINT
wait4fkey:
GETa$:IFa$=""THEN wait4fkey
IFa$="{f1}"THENGOSUB version
IFa$="{f3}"THENGOSUB address
IFa$="{f5}"THENGOSUB show_cmds
IFa$="{f7}"THENGOSUB boot
IFa$="$"THENGOSUB fake_sd2iec
IFa$="c"THENGOSUB create_d64
PRINT"<hit any key>"
wait4key:
GETa$:IFa$=""THEN wait4key
goto main


menu:
10000 rem ******** menu **********
PRINT"{clear}";
PRINTtab(20)"{rvs on}{blue}                 {$DF}"
PRINTtab(20)"{rvs on}  {green}  {blue} {green}  {blue}  {green}  {blue}  {orange}   {blue}  {$DF}"
PRINTtab(20)"{rvs on} {green} {blue}   {green} {blue} {green} {blue}   {green} {blue}       "
PRINTtab(20)"{rvs on} {green}   {blue} {green} {blue} {green} {blue} {green}   {blue}  {orange}    {blue} "
PRINTtab(20)"{rvs on}   {green} {blue} {green} {blue} {green} {blue} {green} {blue}         "
PRINTtab(20)"{rvs on} {green}   {blue} {green}  {blue}  {green}   {blue}  {orange}    {blue} "
PRINTtab(20)"{rvs on}                   "
PRINTtab(20)"{rvs on} {green}   {blue}  {green}  {blue}  {green}  {blue}  {orange}    {blue} "
PRINTtab(20)"{rvs on}  {green} {blue}  {green} {blue}   {green} {blue}         "
PRINTtab(20)"{rvs on}  {green} {blue}  {green}   {blue} {green} {blue}    {orange}    {blue} "
PRINTtab(20)"{rvs on}  {green} {blue}  {green} {blue}   {green} {blue}         "
PRINTtab(20)"{rvs on} {green}   {blue}  {green}  {blue}  {green}  {blue}  {orange}    {blue} "
PRINTtab(20)"{rvs on}                   "
PRINT"{home}sd2iec snoop{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}{grey3}v0.4  by exrom{blue}{darkgrey}":PRINT
PRINT"{grey3}f1{darkgrey} show dos{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}version"
PRINT"{grey3}f3{darkgrey} change drive{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}number"da
PRINT"{grey3}f5{darkgrey} show sd2iec's{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}cbm commands"
PRINT"{grey3}f7{darkgrey} boot"chr$(34)":*"chr$(34)","chr$(48+da)",1":PRINT
PRINT"{grey3}c{darkgrey}  create empty d64{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor down}on sd2iec":PRINT
PRINT"github.com/exrom/sd2iec-snoop":PRINT
RETURN

version:
20000 rem ******* version ********
PRINT"dos version:"
OPEN1,da,15
PRINT#1,"ui"
INPUT#1,a1$,a2$,a3$,a4$
CLOSE1
PRINTa1$","a2$","a3$","a4$
IFNOTleft$(a2$,6)="sd2iec"THENRETURN
OPEN1,da,15
PRINT#1,"x?"
INPUT#1,a1$,a2$,a3$,a4$
PRINTa1$","a2$","a3$","a4$
CLOSE1
RETURN

address:
21000 rem ******* adress *********
PRINT"old device adress:"da
INPUT"new device adress";nd
IFnd<4ORnd>30THENRETURN
PRINT"changing temporarily to"nd"..."
OPEN1,da,15
PRINT#1,"u0>"+chr$(nd)
CLOSE1
da=nd
RETURN

status:
22000 rem ******* status *********
OPEN1,da,15
INPUT#1,a1$,a2$,a3$,a4$
PRINTa1$","a2$","a3$","a4$
CLOSE1
RETURN

rtc:
23000 rem ******** rtc ***********
PRINT"- not implemented yet -"
RETURN

show_cmds:
24000 rem ****** show cbm commands ******
PRINT :PRINT"{clear}some sd2iec cbm commands"
PRINT"========================"
PRINT :PRINT"change to sub directory foo"
PRINT"open1,8,15,"chr$(34)"cd:foo"chr$(34)":close1"
PRINT :PRINT"change to root directory"
PRINT"open1,8,15,"chr$(34)"cd//"chr$(34)":close1"
PRINT :PRINT"change to parent directory .."
PRINT"open1,8,15,"chr$(34)"cd{$5f}"chr$(34)":close1"
PRINT :PRINT"change drive address temporarily"
PRINT"open 1,8,15,"chr$(34)"U0>9"chr$(34)":close1"
PRINT :PRINT"find complete docu in 'readme' file of"
PRINT"git web interface at www.sd2iec.de"
RETURN

boot:
25000 rem ******** boot **********
PRINT :PRINTchr$(17)"load"chr$(34)":*"chr$(34)","da",1";:PRINTchr$(145)chr$(145)chr$(145)
FORi=1to1541:NEXT
POKE631,131:POKE198,1
END

fake_sd2iec:
26000 rem ****** sd2iec fake output ******
PRINT"dos version:"
PRINT"73,sd2iec v1.0.0atentdead0-36,00,00"
PRINT"09,sd2iec v1.0.0atentdead0-36-m1284p-sw2,00,00"
RETURN


create_d64:
30100 rem ***** create d64 image ********
PRINT"{clear}create empty d64 image"
PRINT"======================":PRINT
#     <----- 40 columns screen width --------><----- 40 columns screen width -------->
PRINT"an empty d64 image file with 174848     bytes in size will be created."
PRINT"this will work only on a real c64       with an sd2iec attached."
PRINT"make sure enough free space is availableand no d64 is currently mounted."
PRINT :PRINT"continue? {grey3}y{darkgrey}es"
GETa$:ifa$=""THEN30124
IFa$<>"y"THENRETURN
PRINT
PRINT"name of image    blank.d64{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}";:inputin$
PRINT"device address  "da"{cursor left}{cursor left}{cursor left}{cursor left}";:inputdr
PRINT"disc label       empty{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}{cursor left}";:inputdn$
PRINT
PRINT"creating "in$" on drive"dr"..."
OPEN1,dr,3,in$+",p,w"
OPEN2,dr,15,"p"+chr$(3)+chr$(233)+chr$(170)+chr$(2)+chr$(0)
PRINT#1,"created by sd2iec snoop";
CLOSE1
IFst<>0thenPRINT"error creating image!":CLOSE2:RETURN
PRINT"mounting..."
PRINT#2,"cd:"+in$
IFst<>0THENPRINT"error mounting image!":CLOSE2:RETURN
PRINT"formatting..."
PRINT#2,"n:"+dn$+",00"
PRINT"unmounting..."
PRINT#2,"cd{$5f}"
CLOSE2
PRINT:PRINT"done. enjoy using "in$
RETURN
