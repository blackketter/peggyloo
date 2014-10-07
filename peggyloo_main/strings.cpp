
#include "Arduino.h"
#include <PeggyLoo.h>
#include <Time.h>
#include "main.h"
#include "strings.h"

void strings_setup() {
	frame.ClearGrey();
};

int stringsiter = 0;
int stringsmode = 1;

float redpos = 0;
float greenpos = 0;
float bluepos = 0;
float whitepos = 0;

float redinc = 2.0;
float blueinc = 1.99;
float greeninc = 1.98;
float whiteinc = 1.97;

float maxinc = 5;
int maxloops = 10;

#define MAXMODES (8)

boolean strings_loop(boolean skip) {

	// if the button is held for less than 1000ms, move on to the next mode
	unsigned long br = frame.ButtonReleased();

	if (skip || ((br > 0) && (br < 1000)) ) {
		stringsmode++;

		if (stringsmode > MAXMODES) {
			stringsmode = 1;
			return false;
		}

		switch (stringsmode) {
			case (5):
				redinc = 2.0;
				blueinc = 1.99;
				greeninc = 1.98;
				whiteinc = 1.97;
				break;
			case (6):
				redinc = 1.0;
				blueinc = 1.3;
				greeninc = 1.6;
				whiteinc = 1.9;
				break;
			case (7):
				redinc = 0.5;
				blueinc = 0.91;
				greeninc = 0.29;
				whiteinc = 0.1;
				break;
			case (8):
				redinc = random(20)/10.0;
				blueinc = random(20)/10.0;
				greeninc = random(20)/10.0;
				whiteinc = random(20)/10.0;
				break;
			default:
				redinc = 2.0;
				blueinc = 1.99;
				greeninc = 1.98;
				whiteinc = 1.97;
		}

		strings_setup();
	}

	if (!frame.Button() && (stringsiter > maxloops)) {
		// two shifts, to preserve color
		frame.ShiftColor(PEGGYLOO_DOWN);

		switch (stringsmode) {
			case (1):
				frame.SetPointColor(sintable[(int)redpos]/2+PEGGYLOO_COLOR_WIDTH/2,0,MAX_BRIGHTNESS,0,0,0);
				break;
			case (2):
				frame.SetPointColor(sintable[(int)greenpos]/2+PEGGYLOO_COLOR_WIDTH/2,0,0,MAX_BRIGHTNESS,0,0);
				break;
			case (3):
				frame.SetPointColor(sintable[(int)bluepos]/2+PEGGYLOO_COLOR_WIDTH/2,0,0,0,MAX_BRIGHTNESS,0);
				break;
			case (4):
				frame.SetPointColor(sintable[(int)whitepos]/2+PEGGYLOO_COLOR_WIDTH/2,0,0,0,0,MAX_BRIGHTNESS);
				break;
			default:
				// do these in grey, so the points can overlap
				frame.SetPointGrey(((sintable[(int)redpos]+PEGGYLOO_COLOR_WIDTH)/2)*2,0,MAX_BRIGHTNESS);
				frame.SetPointGrey(((sintable[(int)greenpos]+PEGGYLOO_COLOR_WIDTH)/2)*2+1,0,MAX_BRIGHTNESS);
				frame.SetPointGrey(((sintable[(int)bluepos]+PEGGYLOO_COLOR_WIDTH)/2)*2,1,MAX_BRIGHTNESS);
				frame.SetPointGrey(((sintable[(int)whitepos]+PEGGYLOO_COLOR_WIDTH)/2)*2+1,1,MAX_BRIGHTNESS);
				break;
		}

		stringsiter = 0;

		redpos += redinc;
		greenpos += greeninc;
		bluepos += blueinc;
		whitepos += whiteinc;

		if (redpos >= 60) { redpos -= 60; }
		if (greenpos >= 60) { greenpos -= 60; }
		if (bluepos >= 60) { bluepos -= 60; }
		if (whitepos >= 60) { whitepos -= 60; }

	}
	stringsiter++;

	frame.RefreshGrey();
	return true;

};
