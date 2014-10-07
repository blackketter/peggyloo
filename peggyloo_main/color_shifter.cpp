/*
  color shifting light panel
 */

#include "Arduino.h"
#include <PeggyLoo.h>
#include "main.h"
#include "color_shifter.h"

short intensity = 0;
short delta = 1;
int   framecounter = 0;

#define UPDATE_INTERVAL (5)

void color_shifter_setup()
{
	frame.FillGrey(intensity);
}


boolean color_shifter_loop(boolean skip)                     // run over and over again
{

  // Draw each frame for its intensity level
  frame.RefreshGrey();

  // if the button is held for less than 1000ms, move on to the next mode
  unsigned long br = frame.ButtonReleased();

  if ( (br > 0) && (br < 1000) ) {
    return false;
  }

  if (skip) {
  	return false;
  }

  framecounter++;

  if (framecounter > UPDATE_INTERVAL) {
	  framecounter = 0;
	  intensity += delta;
	  if ((intensity >= MAX_BRIGHTNESS) || (intensity <= 0)) {
		delta = -delta;
	  }
	  color_shifter_setup();
  }
  return true;
}
















