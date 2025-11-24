#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <c64.h>
#include <cbm.h>
#include <errno.h>      // _oserror

#define SCREEN_POS_LOGO_X       (21)
#define SCREEN_COLUMN_OFFSET_MENU       (3)

#define SCREEN_RAM          ((unsigned char*)0x400)
#define SCREEN_RAM_SIZE     (1000u)
#define SCREEN_LINE_LEN     (40u)
#define SCREEN_LINE_NUM     (25u)

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;

uint8_t snp_buf[128];

void cput_u16(const unsigned int u16Number)
{
    utoa(u16Number, snp_buf, 10);
    cputs(snp_buf);
}


int main()
{
    uint8_t u8YShift = 3;
    signed char i8ScrollSpeed = -1;
    uint8_t u8Delay = 100;
    uint8_t u8Tmp = 0;
    uint8_t i;

    /* Y Scroll default == 3 
        slow = 1 pixel:  3,2,1,0,7(m),6,5,4,3
        med  = 2 pixel:  3,1,7(m),5,3
        fast = 4 pixel:  3,7(m),3
    */
    cput_u16(VIC.ctrl1 & (7));cputs("\r\n");
    gotoy(SCREEN_LINE_NUM -3);

    while(1)
    {
        for (i = 0; i<50; ++i) 
            waitvsync();
        if ((SCREEN_LINE_NUM-1) == wherey())
        {
            u8YShift = (u8YShift+i8ScrollSpeed) & 7;
            VIC.ctrl1 = (VIC.ctrl1 & (~7)) | u8YShift;
            if (u8YShift == 7)
            {
                /* 0 -> 7 == jump down --> copy screen contents one line up */
                memcpy(SCREEN_RAM, SCREEN_RAM+SCREEN_LINE_LEN, SCREEN_RAM_SIZE-SCREEN_LINE_LEN);
                memset(SCREEN_RAM+SCREEN_RAM_SIZE-SCREEN_LINE_LEN, ' ', SCREEN_LINE_LEN);
            }
            else if (u8YShift == 3)
            {
                /* 0 -> 7 == jump down --> copy screen contents one line up */
                gotoy(wherey()-1);
            }
        }
        else
        {
            cput_u16(u8Tmp);cputs("\r\n");
        }
        ++u8Tmp;
    }
}

