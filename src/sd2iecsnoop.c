/** 
 * 
 * SD2IEC Snoop is a helper tool for the sd2iec floppy drive emulator running on the C64. 
 *
 * Created 2017 by exrom   exrom @ posteo.de
 *
 * This work is licensed under the terms of the GPL V3. See COPYING for details.
 *
 * THE PROGRAMS ARE DISTRIBUTED IN THE HOPE THAT THEY WILL BE USEFUL,
 * BUT WITHOUT ANY WARRANTY. USE THEM AT YOUR OWN RISK!
 *
 * https://github.com/exrom/sd2iec-snoop
 */

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <c64.h>
#include <cbm.h>
#include <errno.h>      // _oserror

#define SCREEN_POS_LOGO_X               (21)
#define SCREEN_COLUMN_OFFSET_MENU       (3)

#define SCREEN_RAM          ((unsigned char*)0x400)
#define SCREEN_RAM_SIZE     (1000u)
#define SCREEN_LINE_LEN     (40u)
#define SCREEN_LINE_NUM     (25u)


#define CBM_COMMAND         15       /* missing in cbm.h!? */
#define CBM_ERROR_CODE_DEVICE_NOT_PRESENT       5   /* missing in cbm.h!? */

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

uint8_t snp_u8CurrentDevice = 8;
uint8_t snp_u8sd2iecDevice = 0;
uint8_t snp_buf[128];

const uint8_t d64_seek_array[] = {'p', 3, 255, 170, 2, 0};
const uint8_t logo_color[] = {
    102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
    102, 85, 86, 101, 86, 101, 134, 136, 102, 102,
    86, 102, 86, 86, 102, 86, 102, 102, 102, 102,
    86, 85, 86, 86, 86, 85, 102, 136, 136, 102,
    102, 86, 86, 86, 86, 102, 102, 102, 102, 102,
    86, 85, 86, 101, 86, 85, 102, 136, 136, 102,
    102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
    86, 85, 102, 85, 102, 85, 102, 136, 136, 102,
    102, 101, 86, 102, 86, 102, 102, 102, 102, 102,
    102, 101, 86, 85, 86, 102, 102, 136, 136, 102,
    102, 101, 86, 102, 86, 102, 102, 102, 102, 102,
    86, 85, 102, 85, 102, 85, 102, 136, 136, 102,
    102, 102, 102, 102, 102, 102, 102, 102, 102, 102
};

const uint8_t *dos_error_msg[] = {
    "",
    "too many files",
    "file open",
    "file not open",
    "file not found",
    "device not present",
    "not input-file",
    "not output-file",
    "missing file-name",
    "illegal device-number",
    "STOP-key pushed",
    "general I/O-error"
};

const uint8_t* menutxt[] =
{ 
    "drive versions",      
    "sd2iec cheats",
    "real time clock",
    "create empty d64",
    "change drive id",
    "boot from disk",
    "list directory",
    "exit program",
};

    /* Convert a 16 bit unsigned int to a string and prints it with cputs(). Avoid using expensive printf */
void cput_u16(const unsigned int u16Number)
{
    utoa(u16Number, snp_buf, 10);
    cputs(snp_buf);
}

void cput_newline(void)
{
    cputs("\r\n");
}

void screen_scroll(void)
{
    memcpy(SCREEN_RAM, SCREEN_RAM+SCREEN_LINE_LEN, SCREEN_RAM_SIZE-SCREEN_LINE_LEN);
    memset(SCREEN_RAM+SCREEN_RAM_SIZE-SCREEN_LINE_LEN, 32, SCREEN_LINE_LEN);
    gotox(0);
}

/* Reads a string from keyboard into *string with max length len 2...255 */
void cgets(uint8_t *string, const uint8_t len)
{
    uint8_t idx = 0;
    uint8_t key;
    
    cputc('?'); cputc(' ');     /* prompt */
    cursor(1);                  /* blink */
    do
    {
        key = cgetc();
        cputc(key);
        if ((key == '\n'))      /* stop on enter */
        {
            cputc('\r');        /* + carriage return */
            string[idx]=0;
            break;
        }
        string[idx]=key;
        if (idx<len) ++idx;     /* keep printing entered chars even if buffer end reached */
    }
    while(1);
    cursor(0);
}

void printDOSErrorMsg(const uint8_t u8Error)
{
    if (u8Error <= sizeof(dos_error_msg)) 
    {
        cputs(dos_error_msg[u8Error]);
        cput_newline();
    }
}

uint8_t dos_command(const uint8_t u8Lfn, const uint8_t u8Device, const uint8_t u8Secondary, const uint8_t* szCmd, uint8_t* szResponse, uint8_t u8Len)
{
    uint8_t u8Ret;
    
    if (cbm_open(u8Lfn, u8Device, u8Secondary, szCmd) == 0)
    {
        if (szResponse != NULL)
        {
            u8Ret = cbm_read(u8Lfn, szResponse, u8Len);
            if (u8Ret<u8Len) szResponse[u8Ret]=0;
        }
    }
    cbm_close(u8Lfn);
    return _oserror;
}

void printDOSVersions(void)
{
    #define MAX_CMD_STR_LEN      35
    uint8_t u8Tmp;
    uint8_t u8Device;
    
    for(u8Device=8; u8Device <=11; u8Device++)
    {
        textcolor(COLOR_GRAY1);
        cput_u16(u8Device);cput_newline();

        u8Tmp = dos_command(15, u8Device, CBM_COMMAND, "ui", snp_buf, sizeof(snp_buf));
        if (!u8Tmp)
        {
            snp_buf[MAX_CMD_STR_LEN]=0;      /* forcefully terminate string to match screen width TODO: screensize()-3 */
            //for(u8Tmp=MAX_CMD_STR_LEN; u8Tmp!=0; u8Tmp--) if (snp_buf[u8Tmp]=='\n') snp_buf[u8Tmp]=0;   /* cputs already does LF */
            textcolor(COLOR_GRAY3);
            cputs(snp_buf);
            if (strstr(snp_buf,"sd2iec"))
            {
                cput_newline();
                /* this is a sd2iec - print extended drive info */
                dos_command(15, u8Device, CBM_COMMAND, "x?", snp_buf, sizeof(snp_buf));
                //for(u8Tmp=MAX_CMD_STR_LEN; u8Tmp!=0; u8Tmp--) if (snp_buf[u8Tmp]=='\n') snp_buf[u8Tmp]=0;   /* cputs already does LF */
                cputs(snp_buf);
                snp_u8CurrentDevice = u8Device;
                snp_u8sd2iecDevice = u8Device;
            }
        }
        else
        {
            printDOSErrorMsg(u8Tmp);
        }
        cput_newline();
    }
}

void cbm_printdir (uint8_t lfn)
{
    uint8_t byte, u8State, u8newline = 0;
    uint16_t u16Size;

    if (snp_u8sd2iecDevice == snp_u8CurrentDevice)
    {
        /* this drive is a sd2iec - print extended dir info */
        memcpy(snp_buf, "$=t", 4);
    }
    else
    {
        /* standard directory format */
        memcpy(snp_buf, "$", 2);
    }
    if (cbm_opendir(lfn, snp_u8CurrentDevice, snp_buf) == 0)
    {
        if (!cbm_k_chkin(lfn))
        {
            //if (cbm_k_readst()) return;
            u8State = 0;
            while(1)
            {
                byte = cbm_k_basin();
                if (cbm_k_readst()) break;

                switch(u8State)
                {
                    case 0:                 /* skip 2 bytes, next-BASIC-line pointer */
                    case 1:
                        u8State++;
                        break;
                    case 2:                 /* 16 bit line number/file size */
                        u16Size = byte;
                        if (u8newline)
                        {
                            screen_scroll();
                        }
                        u8State++;
                        break;
                    case 3:
                        u16Size |= byte<<8;
                        utoa(u16Size, snp_buf, 10);
                        cputs(snp_buf);cputc(' ');
                        u8State++;
                        break;
                    default:
                        if (byte == 0x12)   /* title line invers */
                        {
                            revers(1);
                            break;
                        }
                        if (byte == 0)      /* end of line */
                        {
                            revers(0);
                            u8State = 0;
                            if (wherey()==SCREEN_LINE_NUM-1) 
                            {
                                u8newline = 1;  /* screen end is reached, now scroll for every new line */
                            }
                            else
                            {
                                cput_newline();
                            }
                            break;
                        }
                        /* needed fo $=t output that uses last column */
                        if (wherex()<(SCREEN_LINE_LEN-1)) cputc(byte);
                }
            }
        }
    }
    cbm_close(lfn);
}

void changeDriveAddr(void)
{
    uint8_t u8NewAddress;
    uint8_t u8Ret;
    uint8_t *szCbmCmd;

    /*     <--------------------------------------><--------------------------------------> */
    cputs("this will change drive address tempo-   rarily in the drive's ram. make sure,   to choose a free drive address.\r\n\n");
    cputs("enter new drive address for drive ");cput_u16(snp_u8CurrentDevice);cput_newline();
    cgets(snp_buf, sizeof(snp_buf));
    u8NewAddress = atoi(snp_buf);
    cputs("changing to ");cput_u16(u8NewAddress);cputs("...\r\n");

    szCbmCmd = "u0>?"; 
    szCbmCmd[3] = u8NewAddress;

    if (u8Ret = dos_command(15, snp_u8CurrentDevice, CBM_COMMAND, szCbmCmd, NULL, 0) != 0)
    {
        printDOSErrorMsg(u8Ret);
    }
    else
    {
        cputs("done!\r\n");
    }
}

void printDriveStatus(void)
{
    #define MAX_CMD_STR_LEN      35
    uint8_t u8Tmp;
    uint8_t buf[64];   /* TBD: max bytes in cbm_read()? */
    
    u8Tmp = dos_command(15, snp_u8CurrentDevice, CBM_COMMAND, "", buf, sizeof(buf));
    if (!u8Tmp)
    {
        buf[MAX_CMD_STR_LEN]=0;      /* forcefully terminate string to match screen width TODO: screensize()-3 */
        for(u8Tmp=MAX_CMD_STR_LEN; u8Tmp!=0; u8Tmp--) if (buf[u8Tmp]=='\n') buf[u8Tmp]=0;   /* cputs already does LF */
        cputs(buf);
    }
    else
    {
        printDOSErrorMsg(u8Tmp);
    }
}

void sd2iec_cheats(void)
{
    /*     <--------------------------------------><--------------------------------------> */
    cputs("open1,8,15,\"...\":close1\r\n");
    cputs("    cd:foo   change to sub directory\r\n");
    cputs("    cd//     change to root directory\r\n");
    cputs("    cd"); cbm_k_bsout(0x5F); cputs("      change to parent directory\r\n");
    cputs("    u0>9     change drive address\r\n\n");
    cputs("load\"...\",8\r\n");
    cputs("    $=p      list partitions\r\n");
    cputs("    $!       dir of eeprom fs\r\n");
    cputs("    !:file   load from eeprom fs\r\n\n");
}

void create_d64(void)
{
    uint8_t key;
    uint8_t u8Ret;
    uint8_t imgname[16];
    uint8_t disclabel[16];
    uint8_t tmp1[16];

    /*     <----- 40 columns screen width --------><----- 40 columns screen width --------><----- 40 columns screen width --------><----- 40 columns screen width --------><----- 40 columns screen width --------><----- 40 columns screen width --------><----- 40 columns screen width --------><----- 40 columns screen width --------><----- 40 columns screen width -------->  */
    cputs("an empty d64 image file with 174848     bytes in size will be created. this willwork only on a real c64 with an sd2iec  attached. make sure enough free space isavailable and no d64 is currently moun- ted.\r\n\n");
    cputs("using drive "); cput_u16(snp_u8CurrentDevice);cputs("\r\n\n");
    cputs("continue? y/[n]\r\n\n");
    do {} while (0 == (key = cgetc()));
    if (key != 'y') return;
    cputs("name of image file [blank.d64]\r\n");
    cgets(imgname,sizeof(imgname));
    if (imgname[0]==0) memcpy(imgname,"blank.d64",sizeof(imgname));
    cputs("disc label [empty]\r\n");
    cgets(disclabel, sizeof(disclabel));
    if (disclabel[0]==0) memcpy(disclabel,"empty",sizeof(disclabel));
    cputs("\r\n");
    cputs("creating d64...\r\n");
    tmp1[0]=0; strcat(tmp1, imgname);strcat(tmp1, ",p,w");
    /* open1,8,3,"blank.d64,p,w" */
    if (u8Ret = cbm_open(1, snp_u8CurrentDevice, 3, tmp1) == 0)
    {
        //pBuf = "p123456"; pBuf[1]=3; pBuf[2]=255; pBuf[3]=170; pBuf[4]=2; pBuf[5]=0; 
        /* open2,8,15,"p"+chr$(3)+chr$(255)+chr$(170)+chr$(2) */
        if (u8Ret = cbm_open(2, snp_u8CurrentDevice, 15, d64_seek_array) == 0)
        {
            u8Ret = cbm_write(1, 0, 1);
            cbm_close(1);
            if (u8Ret == 0xff)      /* -1 */ 
            {
                printDOSErrorMsg(_oserror);
            }
            else
            {
                cputs("mounting...\r\n");
                tmp1[0]=0; strcat(tmp1, "cd:"); strcat(tmp1, imgname);
                /* print#2,"cd:test.d64" */
                u8Ret = cbm_write(2, tmp1, strlen(tmp1));
                if (u8Ret == 0xff)      /* -1 */ 
                {
                    printDOSErrorMsg(u8Ret);
                }
                else
                {
                    cputs("formatting...\r\n\n");
                    tmp1[0]=0;
                    strcat(tmp1, "n:");
                    strcat(tmp1, disclabel);
                    strcat(tmp1, ",00");
                    cbm_write(2, tmp1, strlen(tmp1));
                    cbm_printdir(1);
/*
                    tmp1[0]=0; strcat(tmp1, "cd"); tmp1[2] = 0x5f;
                    cputs("unmounting...\r\n");
                    cbm_write(2, tmp1, 3);
*/
                    cbm_close(2);
                }
            }
        }
        else
        {
            printDOSErrorMsg(u8Ret);
        }
    }
    else
    {
        printDOSErrorMsg(u8Ret);
    }
    cgetc();
    /* Thanks @unseen https://www.forum64.de/index.php?thread/43068-leeres-d64-image-erstellen/&postID=523837#post523837 */
}

void boot1st(void)
{
    uint8_t i;
    uint8_t* p;

    cputs("\r\n\n\nload\":*\",");cput_u16(snp_u8CurrentDevice);cputs(",1\r");
    gotoy(wherey()-2);
    p = (uint8_t*) 631; *p = 131;      /* put SHIFT+RUN/STOP to keyboard buffer */
    p = (uint8_t*) 198; *p = 1;        /* keyboard buffer contains one key */
    for(i=55;i>0;--i) waitvsync();  /* ~1 sec delay */
}

void rtc(void)
{
    uint8_t u8Tmp;
    /*                      01234567890123456890123   */
    uint8_t au8TimeCmd[] = "t-wiyyyy mm dd hh mm ss";
    /* We cannot use t-rd as it means to have \0 in the command string (for sunday or am) which cbm_open() cannot handle as sees this as end of string :-( */

    while(1)
    {
        cputs("t-ri:  ");
    
        u8Tmp = dos_command(15, snp_u8CurrentDevice, CBM_COMMAND, "t-ri", snp_buf, sizeof(snp_buf));
        if (!u8Tmp)
        {
            cputs(snp_buf);
            cput_newline();
            if (snp_buf[0] == '3' && snp_buf[1] == '0')
            {
                cputs("real time clock not available!");
            }
            else if (snp_buf[0] == '3' && snp_buf[1] == '1')
            {
                cputs("rtc is not set"); 
            }
            cput_newline();

            cputs("<s>et rtc   <m>ain menu\r\n\n");
            do {} while (0 == (u8Tmp = cgetc()));
            if (u8Tmp == 'm') return;
            if (u8Tmp == 's')
            {
                /*     <--------------------------------------><--------------------------------------> */
                //cputs("dow (0=sun 1=mon 2=tue 3=wed 4=thu 5=fri6=sat     :");cgets(snp_buf, sizeof(snp_buf));au8TimeCmd[4] = 1;//(uint8_t) atoi(snp_buf);
                cputs("year yyyy : ");cgets(&au8TimeCmd[4], 4); au8TimeCmd[8] = '-';
                cputs("month  mm : ");cgets(&au8TimeCmd[9], 2); au8TimeCmd[11] = '-';
                cputs("day    dd : ");cgets(&au8TimeCmd[12], 2); au8TimeCmd[14] = 't';
                cputs("hour   hh : ");cgets(&au8TimeCmd[15], 2); au8TimeCmd[17] = ':';
                cputs("minute mm : ");cgets(&au8TimeCmd[18], 2); au8TimeCmd[20] = ':';
                cputs("second ss : ");cgets(&au8TimeCmd[21], 2);

                cputs("\nsending ");cputs(au8TimeCmd);cput_newline();
                dos_command(15, snp_u8CurrentDevice, CBM_COMMAND, au8TimeCmd, NULL, 0);
                cputs("\npress any key");
                do {} while (0 == (u8Tmp = cgetc()));
            }
            clrscr();
        }
        else
        {
            printDOSErrorMsg(u8Tmp);
        }
    }
}

#define LOGO_SIZE_X     19
#define LOGO_SIZE_Y     13
#define SCREEN_SIZE_X   40
    
void printlogo(void)
{
    /* SD2IEC Logo */
    uint8_t u8ScreenLine = 0;
#if 1
    uint8_t col_idx = 0;
    uint8_t l,c;
    uint8_t*  pScreenRAM;
    uint8_t*  pColorRAM;

    pScreenRAM = SCREEN_RAM + SCREEN_POS_LOGO_X;
    pColorRAM = COLOR_RAM + SCREEN_POS_LOGO_X;
    
    for (l=0; l<LOGO_SIZE_Y; ++l)
    {
        col_idx=(LOGO_SIZE_X+1)*l;
        for (c=0; c<LOGO_SIZE_X; ++c)
        {
            *pScreenRAM = 160;
            *pColorRAM = logo_color[col_idx>>1]>>((col_idx&1)<<2);
            ++pScreenRAM;
            ++pColorRAM;
            ++col_idx;
        }
        pScreenRAM += SCREEN_SIZE_X-LOGO_SIZE_X;
        pColorRAM  += SCREEN_SIZE_X-LOGO_SIZE_X;
    }
    pScreenRAM = SCREEN_RAM+SCREEN_SIZE_X*0+SCREEN_POS_LOGO_X+LOGO_SIZE_X-2; *pScreenRAM = 0xDF;
    pScreenRAM = SCREEN_RAM+SCREEN_SIZE_X*0+SCREEN_POS_LOGO_X+LOGO_SIZE_X-1; *pScreenRAM = 0x20;
    pScreenRAM = SCREEN_RAM+SCREEN_SIZE_X*1+SCREEN_POS_LOGO_X+LOGO_SIZE_X-1; *pScreenRAM = 0xDF;
#else
    /* following code does the same but needs ~1000 bytes more ROM (cc65 -Os) */
    revers(1);
    /* gotox() + LF-strings will add an empty line and won't write to last column */
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs("                 \xDF");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs("  ");textcolor(COLOR_GREEN);cputs("  ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs("  ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs("  ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_ORANGE);cputs("   ");textcolor(COLOR_BLUE);cputs("  \xDF");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs(" ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("   ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("   ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("       ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs(" ");textcolor(COLOR_GREEN);cputs("   ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs("   ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_ORANGE);cputs("    ");textcolor(COLOR_BLUE);cputs(" ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs("   ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("         ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs(" ");textcolor(COLOR_GREEN);cputs("   ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs("  ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs("   ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_ORANGE);cputs("    ");textcolor(COLOR_BLUE);cputs(" ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs("                   ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs(" ");textcolor(COLOR_GREEN);cputs("   ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs("  ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs("  ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_ORANGE);cputs("    ");textcolor(COLOR_BLUE);cputs(" ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs("  ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("   ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("         ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs("  ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs("   ");textcolor(COLOR_BLUE);cputs(" ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("    ");textcolor(COLOR_ORANGE);cputs("    ");textcolor(COLOR_BLUE);cputs(" ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs("  ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("   ");textcolor(COLOR_GREEN);cputs(" ");textcolor(COLOR_BLUE);cputs("         ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs(" ");textcolor(COLOR_GREEN);cputs("   ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs("  ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_GREEN);cputs("  ");textcolor(COLOR_BLUE);cputs("  ");textcolor(COLOR_ORANGE);cputs("    ");textcolor(COLOR_BLUE);cputs(" ");
    gotoxy(SCREEN_POS_LOGO_X,u8ScreenLine++);cputs("                   ");
    revers(0);
#endif
}

void printmenu(void)
{
    uint8_t i;

    clrscr();

    printlogo();

    /* main menu */
    gotoxy(0,0);
    textcolor(COLOR_BLUE); cputs ("sd2iec snoop\r\n");
    textcolor(COLOR_GREEN);cputs ("v2.0 by exrom\r\n\n");

    for(i=0; i<(sizeof(menutxt)/sizeof(const uint8_t*)); i++)
    {
        textcolor(COLOR_GRAY3-((i&1)<<2));
        cputc('f');
        //cputs(menutxt[i][0]);
        cput_u16(i+1);
        cputc(' ');
        textcolor(COLOR_GRAY1);
        cputs(menutxt[i]);
        cput_newline();
        //if (i%4==3) cput_newline();
    }

    cput_newline();
    textcolor(COLOR_GRAY3);cputs("current drive: ");cput_u16(snp_u8CurrentDevice);cput_newline();cput_newline();
    printDriveStatus();
}

int main()
{
    uint8_t u8Char;

    bordercolor(COLOR_GRAY2);bgcolor(COLOR_GRAY2);
    cbm_k_bsout(0x8E);      // upper case + symbols display as CC65 switches to lower 
    clrscr();printDOSVersions();cgetc();
    while(1)
    {
        printmenu();
        u8Char=cgetc();
        switch(u8Char)
        {
            case CH_F1:
                clrscr();printDOSVersions();cgetc();break;
            case CH_F2:
                clrscr();sd2iec_cheats();cgetc();break;
            case CH_F3:
                clrscr();rtc();break;
            case CH_F4:
                clrscr();create_d64();break;
            case CH_F5:
                clrscr();changeDriveAddr();cgetc();break;
            case CH_F6:
                boot1st();bgcolor(COLOR_GRAY2);exit(0);
            case CH_F7:
                clrscr();cbm_printdir(1);cgetc();break;
            case CH_F8:
                clrscr();bgcolor(COLOR_GRAY2);exit(0);
            default: break;
        }
    }
}
