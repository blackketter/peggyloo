/*
  color shifting light panel
 */

#include "Arduino.h"
#include <PeggyLoo.h>
#include "main.h"
#include "color_wave.h"

int   waveframecounter = 0;
float xCtr,yCtr;
byte  r,g,b,w;
byte stepoffset;
byte color = 0;

#define UPDATE_INTERVAL (5)

void SetPxSeq(byte x,byte y, byte step, byte white)
{

	byte redTemp, greenTemp, blueTemp;

	if (step <= 15)
	{
		redTemp = step;
		greenTemp = 0;
		blueTemp = 15 - step;
	}
	else  if (step <= 31)
	{
		redTemp = 31 - step;
		greenTemp = step - 16;
		blueTemp = 0;
	}
	else  if (step <= 47)
	{
		redTemp = 0;
		greenTemp = 47 - step;
		blueTemp = step - 32;
	}
	switch (color) {
		case 0:
			frame.SetPointColor(x,y,redTemp,0,0,0);
			break;
		case 1:
			frame.SetPointColor(x,y,0,greenTemp,0,0);
			break;
		case 2:
			frame.SetPointColor(x,y,0,0,blueTemp,0);
			break;
		case 3:
			frame.SetPointColor(x,y,0,0,0,redTemp);
			break;
		case 4:
			frame.SetPointColor(x,y,redTemp,greenTemp,blueTemp,white);
			break;
	}
}

void color_wave_setup()
{
	frame.ClearGrey();
    stepoffset = 0;
    xCtr = 5.5;
    yCtr = 5.5;
}

byte i = 0;

boolean color_wave_loop(boolean skip)                     // run over and over again
{

	byte j,k;
	float temp;

	// Draw each frame for its intensity level
	frame.RefreshGrey();

	// if the button is held for less than 1000ms, move on to the next mode
	unsigned long br = frame.ButtonReleased();

	if ( skip || ((br > 0) && (br < 1000)) ) {
		color++;
		if (color == 5) {
			color = 0;
			return false;
		}
	}

		if (rand() & 1)
		{
			if (rand() & 1)
				xCtr += 0.1;
			else
				xCtr -= 0.1;

			 if (xCtr > 10)
				xCtr = 10;

			 if (xCtr < 2)
				xCtr = 2;
		}


		if (rand() & 1)
		{
			if (rand() & 1)
				yCtr += 0.1;
			else
				yCtr -= 0.1;

			if (yCtr > 10)
				yCtr = 10;

			if (yCtr < 2)
				yCtr = 2;
		}

	if (i == 0) {
		w = 0;

		if (stepoffset > 0)
			stepoffset--;
		else
			stepoffset = 47;

	}
	j = 0;
	while (j < 16)
	{
		 k = 3*sqrt((i-xCtr)*(i-xCtr)+(j-yCtr)*(j-yCtr)) + stepoffset;
		if (k > 47)
			k -= 47;

		SetPxSeq(i,j,k,w);

		j++;
	}

	i++;
	if (i >= 12) { i = 0; };

  	return true;

}

















