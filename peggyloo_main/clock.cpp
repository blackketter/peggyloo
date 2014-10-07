
#include "Arduino.h"
#include <PeggyLoo.h>
#include <Time.h>
#include "main.h"
#include "clock.h"


time_t lastDrawn = 0;

void clock_setup() {
	frame.ClearGrey();
	lastDrawn = 0;
};


void clock_draw() {

	// draw this at brightness level 8 or 1 depending on if it's night
	int dimframe = frame.Night() ? 0 : 3;

	int center_x = PEGGYLOO_WIDTH / 2;
	int center_y = PEGGYLOO_HEIGHT / 2;

	// seconds
	int s = second();

	frame.ClearGrey();

	// draw the second dot
	frame.SetPoint(dimframe, sintable[s]+center_x, costable[s]+center_y);

	// minutes
	int m = minute();
	frame.Line(dimframe, center_x, center_y, sintable[m]+center_x, costable[m]+center_y);

	// hour hand (fractional)
	int h = hourFormat12();

	if (h==12) { h=0; } // zero-base hours too

	// convert to 0 to 60 for rendering
	h = h*5 + minute() / 12;

	frame.Line(dimframe, center_x, center_y, sintable[h]*2/3 + center_x, costable[h]*2/3 + center_y);

	// draw a dot in the corner if it's PM
	if (isPM()) {
		frame.SetPoint(dimframe,0,0);
		frame.SetPoint(dimframe,0,1);
		frame.SetPoint(dimframe,1,0);
		frame.SetPoint(dimframe,1,1);
	}
};

boolean clock_loop(boolean skip) {

	// if the button is held for less than 1000ms, move on to the next mode
	unsigned long br = frame.ButtonReleased();

	if (skip || ((br > 0) && (br < 1000)) ) {
		// we're done
		return false;
	}
	long holdTime = frame.ButtonHeld();
	if (holdTime > 1000) {
		// set the clock forward by 1 second
		adjustTime(1);
	}

	if (holdTime > 15000) {
		adjustTime(30);
	}

	if (now() != lastDrawn) {
		clock_draw();
		lastDrawn = now();
	}

	frame.RefreshGrey();
	return true;

};
