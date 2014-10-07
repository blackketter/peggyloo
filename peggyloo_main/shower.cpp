
#include "Arduino.h"
#include <PeggyLoo.h>
#include <Time.h>
#include "main.h"
#include "shower.h"

void shower_setup() {
	frame.ClearGrey();
};

int iter = 0;
int showermode = 1;

uint8_t lastrow[PEGGYLOO_COLOR_WIDTH];

boolean shower_loop(boolean skip) {

	// if the button is held for less than 1000ms, move on to the next mode
	unsigned long br = frame.ButtonReleased();

	if (skip || ((br > 0) && (br < 1000)) ) {
		showermode++;
		shower_setup();
		if (showermode > 4) {
			showermode = 1;
			return false;
		}
	}

	if (!frame.Button() && (iter > 10)) {
		// two shifts, to preserve color
		frame.ShiftColor(PEGGYLOO_DOWN);

		for (int i = 0; i < PEGGYLOO_COLOR_WIDTH; i++) {
			if (lastrow[i] > 0)
				lastrow[i] /= 2;
		}

		lastrow[random(PEGGYLOO_COLOR_WIDTH)] = MAX_BRIGHTNESS;

		switch (showermode) {
			case (1):
				for (int i = 0; i < PEGGYLOO_COLOR_WIDTH; i++) {
					frame.SetPointColor(i,0,0,0,0,lastrow[i]);
				}
				break;
			case (2):
				for (int i = 0; i < PEGGYLOO_COLOR_WIDTH; i++) {
					frame.SetPointColor(i,0,0,0,lastrow[i],0);
				}
				break;
			case (3):
				for (int i = 0; i < PEGGYLOO_COLOR_WIDTH; i++) {
					frame.SetPointColor(i,0,0,lastrow[i],0,0);
				}
				break;
			case (4):
				for (int i = 0; i < PEGGYLOO_COLOR_WIDTH; i++) {
					frame.SetPointColor(i,0,lastrow[i],0,0,0);
				}
				break;

		}

		iter = 0;
	}
	iter++;
	frame.RefreshGrey();
	return true;

};
