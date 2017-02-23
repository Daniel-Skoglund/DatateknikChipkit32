//time4io.c
#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"




int getsw(void)
{
	int sw; 
	sw = ((PORTD >> 8) & 0x0000F);
	return sw;
	
}

int getbtns(void)
{
	int btn;
	btn = ((PORTD >> 5) & 0x00007);
	return btn;
}