/* mipslabwork.c

   This file written 2015 by F Lundevall

   This file should be changed by YOU! So add something here:

   This file modified 2015-12-24 by Ture Teknolog 

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */


/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
	PR2 = 31250; // 100ms / (80 000 000Hz / 256prescale )
	T2CONSET = 0x70;
	TMR2 = 0;
	TRISD = TRISD | 0xfe0; // [1111,1110,0000]
	volatile int* trise = (volatile int*) 0xbf886100;
	*trise = *trise & 0x00;
	T2CONSET = 0x8000;
	
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  while(1)
	{
		
	int buttox = getbtns();
	int switchez = getsw();
	
	if(buttox == 1 || buttox == 3 || buttox == 5 || buttox == 7)
		{
			mytime = (mytime & 0xff0f) | (switchez << 4);
		}
	if(buttox == 2 || buttox == 3 || buttox == 6 || buttox == 7)
		{
			mytime = (mytime & 0xf0ff) | (switchez << 8);
		}
	if(buttox == 4 || buttox == 5 || buttox == 6 || buttox == 7)
		{
			mytime = (mytime & 0x0fff) | (switchez << 12);
		}
  
	volatile int* porte = (volatile int*) 0xbf886110;
  
	//delay( 1000 );
	
	// in case of time out event.
	if(IFS(0) & 0x100)
	{
	IFSCLR(0) = 0x100;
	timeoutcount++;
	}

	if(timeoutcount == 10)
	{
	time2string( textstring, mytime );
	display_string( 3, textstring );
	display_update();
	tick( &mytime );
	(*porte)++;
	timeoutcount = 0x00;
	}
	}
}