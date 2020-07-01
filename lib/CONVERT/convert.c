/*
 * convert.c
 *
 *  Created on: 23.05.2020
 *      Author: tom
 */

#include <lib/CONVERT/convert.h>

void floatToString__toCheckAndRewrite(float f, char *s) {
	f = f + 0, 05;
	if (f < 0) {
		f = -f + 0, 1;
		*(s++) = '-';
	}
	char mini3[20];
	char *mini;
	mini = mini3;

	float t = f;
	int tt = t;
	itoa(tt, mini, 10);
	while (*mini != 0)
		*(s++) = *(mini++);
	tt = ((int) (t * 10)) % 10;
	itoa(tt, mini, 10);
	*(s++) = '.';
	while (*mini)
		*(s++) = *(mini++);
	*s = 0;
}
