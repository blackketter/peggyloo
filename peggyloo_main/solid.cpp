/*
  solid light panel
 */

#include "Arduino.h"
#include <PeggyLoo.h>
#include "main.h"
#include "solid.h"

float white_intensity = 0;
float green_intensity = 0;
float blue_intensity = 0;
float red_intensity = 15;
float red_inc = 0.0111;
float green_inc = 0.0171;
float blue_inc = 0.0201;
float white_inc = 0.0223;
int colormode = 0;
long solid_frame = 0;
long solidcolumn = 0;


void solid_setup()
{
	for (int i=0; i < PEGGYLOO_COLOR_WIDTH; i++) {
		for (int j=0; j < PEGGYLOO_COLOR_HEIGHT; j++) {
			frame.SetPointColor(i,j,red_intensity,green_intensity,blue_intensity,white_intensity);
		}
	}
}


boolean solid_loop(boolean skip)                     // run over and over again
{
	// Draw each frame for its intensity level
	frame.RefreshGrey();

	// if the button is held for less than 1000ms, move on to the next mode
	unsigned long br = frame.ButtonReleased();

	if ( skip || ((br > 0) && (br < 1000)) ) {
	colormode++;
	switch (colormode) {
		case 0:
			white_intensity = 0;
			red_intensity = 15;
			green_intensity = 0;
			blue_intensity = 0;
			break;
		case 1:
			white_intensity = 0;
			red_intensity = 0;
			green_intensity = 15;
			blue_intensity = 0;
			break;
		case 2:
			white_intensity = 0;
			red_intensity = 0;
			green_intensity = 0;
			blue_intensity = 15;
			break;
		case 3:
			white_intensity = 15;
			red_intensity = 0;
			green_intensity = 0;
			blue_intensity = 0;
			break;
		case 4:
			white_intensity = 15;
			red_intensity = 15;
			green_intensity = 15;
			blue_intensity = 15;
			break;
		case 5:
		case 6:
			white_intensity = 8;
			red_intensity = 8;
			green_intensity = 8;
			blue_intensity = 8;
			break;
		// we've gone through all the colors, move on to the next screen mode.
		default:
			colormode = 0;
			return false;
	}

	solid_setup();
	}

	// draw a column
	for (int j=0; j <= PEGGYLOO_COLOR_HEIGHT; j++) {
		frame.SetPointColor(solidcolumn,j, red_intensity,green_intensity,blue_intensity,white_intensity);
	}

    // draw a single column every time through
	solidcolumn++;
	if (solidcolumn >= PEGGYLOO_COLOR_WIDTH)
	{
		solidcolumn = 0;
		solid_frame++;
	}

	// adjust the color every 100 frames
	if (solid_frame > 100) {
		solid_frame = 0;
	}

   switch (colormode) {
    	case (5):
			red_intensity = random(16);
			green_intensity = random(16);
			blue_intensity = random(16);
			white_intensity = random(16);
			break;
		case (6):
			red_intensity += red_inc;
			green_intensity += green_inc;
			blue_intensity += blue_inc;
			white_intensity += white_inc;
			if (red_intensity > 15) { red_intensity = 15; red_inc = -red_inc; }
			if (green_intensity > 15) { green_intensity = 15; green_inc = -green_inc; }
			if (blue_intensity > 15) { blue_intensity = 15; blue_inc = -blue_inc; }
			if (white_intensity > 15) { white_intensity = 15; white_inc = -white_inc; }
			if (red_intensity < 0) { red_intensity = 0; red_inc = -red_inc; }
			if (green_intensity < 0) { green_intensity = 0; green_inc = -green_inc; }
			if (blue_intensity < 0) { blue_intensity = 0; blue_inc = -blue_inc; }
			if (white_intensity < 0) { white_intensity = 0; white_inc = -white_inc; }
			break;
		default:
			break;

	}

	return true;
}
















