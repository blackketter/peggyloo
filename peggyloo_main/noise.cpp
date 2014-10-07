/*
  noise light panel
 */

#include "Arduino.h"
#include <PeggyLoo.h>
#include "main.h"
#include "noise.h"

int noisemode = 1;

void noise_setup()
{
	frame.ClearGrey();
 }


boolean noise_loop(boolean skip)                     // run over and over again
{

  // Draw each frame for its intensity level
  frame.RefreshGrey();

  // if the button is held for less than 1000ms, move on to the next mode
  unsigned long br = frame.ButtonReleased();

  if ( skip || ((br > 0) && (br < 1000)) ) {
  	noisemode++;
  	noise_setup();
  	if (noisemode > 15) {
  		noisemode = 1;
  		return false;
  	}
  }

  switch (noisemode) {
  		case 1:
  			frame.SetPointColor(random(PEGGYLOO_COLOR_WIDTH), random(PEGGYLOO_COLOR_HEIGHT), 0,0,0,random(BRIGHTNESS_LEVELS));
  			break;
  		case 2:
 			frame.SetPointColor(random(PEGGYLOO_COLOR_WIDTH), random(PEGGYLOO_COLOR_HEIGHT), 0,0,random(BRIGHTNESS_LEVELS),0);
   			break;
  		case 3:
 			frame.SetPointColor(random(PEGGYLOO_COLOR_WIDTH), random(PEGGYLOO_COLOR_HEIGHT), 0,random(BRIGHTNESS_LEVELS),0,0);
  			break;
  		case 4:
 			frame.SetPointColor(random(PEGGYLOO_COLOR_WIDTH), random(PEGGYLOO_COLOR_HEIGHT), random(BRIGHTNESS_LEVELS),0,0,0);
   			break;
  		case 5:
  			for (int i = 0; i < PEGGYLOO_COLOR_HEIGHT; i++)
  				frame.SetPointColor(random(PEGGYLOO_COLOR_WIDTH), i, 0,0,0,random(BRIGHTNESS_LEVELS));
  			break;
  		case 6:
  			for (int i = 0; i < PEGGYLOO_COLOR_HEIGHT; i++)
	 			frame.SetPointColor(random(PEGGYLOO_COLOR_WIDTH), i, 0,0,random(BRIGHTNESS_LEVELS),0);
   			break;
  		case 7:
  			for (int i = 0; i < PEGGYLOO_COLOR_HEIGHT; i++)
	 			frame.SetPointColor(random(PEGGYLOO_COLOR_WIDTH), i, 0,random(BRIGHTNESS_LEVELS),0,0);
  			break;
  		case 8:
  			for (int i = 0; i < PEGGYLOO_COLOR_HEIGHT; i++)
	 			frame.SetPointColor(random(PEGGYLOO_COLOR_WIDTH), i, random(BRIGHTNESS_LEVELS),0,0,0);
   			break;
  		case 9:
 			frame.SetPointGrey(random(PEGGYLOO_WIDTH), random(PEGGYLOO_HEIGHT), random(BRIGHTNESS_LEVELS));
   			break;
  		case 10:
  			for (int i = 0; i < PEGGYLOO_HEIGHT; i++)
	 			frame.SetPointGrey(random(PEGGYLOO_WIDTH), i, random(BRIGHTNESS_LEVELS));
   			break;
   		case 11:
   			// write a whole column at a time.
   			for (int i = 0; i < PEGGYLOO_WIDTH*4; i++) {
   				// random only provides 31 bits of randomness, xoring with a left-shifted version of value is good enough
   				frame.WriteColumn(0, i, random32());
   			}
   			break;
   		case 12:
   			// write a whole column at a time, but just the white pixels.
   			for (int i = 0; i < PEGGYLOO_WIDTH*4; i+=2) {
   				frame.WriteColumn(0, i, random32() & WHITE_MASK_1);
   			}
			break;
   		case 13:
   			// write a whole column at a time, but with the blue pixels.
   			for (int i = 0; i < PEGGYLOO_WIDTH*4; i+=2) {
   				frame.WriteColumn(0, i, random32() & BLUE_MASK_1);
   			}
   			break;
   		case 14:
   			// write a whole column at a time, but with the green pixels.
   			for (int i = 1; i < PEGGYLOO_WIDTH*4; i+=2) {
   				frame.WriteColumn(0, i, random32() & GREEN_MASK_2);
   			}
   			break;
   		case 15:
   			// write a whole column at a time, but with the blue pixels.
   			for (int i = 1; i < PEGGYLOO_WIDTH*4; i+=2) {
   				frame.WriteColumn(0, i, random32() & RED_MASK_2);
   			}
   			break;
  }

  return true;

}
















