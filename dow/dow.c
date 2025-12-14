#include <stdio.h>
#include <inttypes.h>

/* gcc -o dow dow.c && ./dow >dow_c.txt */

#if 0
/* from sd2iec doscmd.c - y is offset to 1900 */
uint8_t day_of_week(uint16_t y, uint8_t m, uint8_t d) {
    y += 1900;
    return (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;
  }
#endif

#if 1
/* from sd2iec doscmd.c - y is offset to 1900 - uint16_t patch */
uint8_t day_of_week(uint16_t y, uint16_t m, uint16_t d) {
    y += 1900;
    return (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;
  }
#endif

#if 0
int day_of_week(uint16_t year, uint8_t month, uint8_t day) {
    year += 1900;
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    uint16_t K = year % 100;
    uint16_t J = year / 100;
    uint16_t f = day + (13 * (month + 1)) / 5 + K + K / 4 + J / 4 + 5 * J;
    return f % 7;
}
#endif


int main(void)
{
	for(uint16_t y = 0; y<255; y++)
		for(uint8_t m = 1; m<=12; m++)
			for(uint8_t d = 1; d<=(30
									+((m<=7)*m&1) 
									+((m>=8)*!(m&1))
									-2*(m==2)+(m==2)*( (y+1900)%4 == 0 )
								   );d++) 
				printf("%d-%02d-%02d %d\n", y+1900, m, d, day_of_week(y, m, d));
}

	
