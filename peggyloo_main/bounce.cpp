
#include "Arduino.h"
#include <PeggyLoo.h>
#include <Time.h>
#include "main.h"
#include "bounce.h"

float x = 0;
float y = 0;
float dx = 0.04;
float dy = 0;

byte currx = 0;
byte curry = 0;
byte lastx[16];
byte lasty[16];
byte samespot = 0;
byte bounceframe = 0;

void bounce_setup() {
	frame.ClearGrey();
	x = random(PEGGYLOO_COLOR_WIDTH);
	y = random(PEGGYLOO_COLOR_HEIGHT);
	dx = (random(240)-120)/10.0;
	dy = (random(320)-160)/10.0;
	for (int i = 0; i<16; i++) {
		lastx[i] = lasty[i] = 0;
	}
	samespot = 0;
};


boolean bounce_loop(boolean skip) {

	// if the button is held for less than 1000ms, move on to the next mode
	unsigned long br = frame.ButtonReleased();

	if (skip || ((br > 0) && (br < 1000)) ) {
		// we're done
		return false;
	}
	long holdTime = frame.ButtonHeld();
	if (holdTime > 1000) {
		// do something if we're holding
	} else {
		bounceframe++;

		if (bounceframe == 1) {
			bounceframe = 0;

			x = x + dx;
			y = y + dy;

			dy += 0.005;

			// bounce off walls
			if (x >= PEGGYLOO_COLOR_WIDTH) {
				x = 2*PEGGYLOO_COLOR_WIDTH - x;
				dx = -dx * 0.7;
			} else if (x < 0) {
				x = -x;
				dx = -dx * 0.7;
			}


			if (y >= PEGGYLOO_COLOR_HEIGHT) {
				y = 2*PEGGYLOO_COLOR_HEIGHT - y;
				dy = -dy * 0.8;
			} else if (y < 0) {
				y = -y;
				dy = -dy * 0.8;
			}

			for (int i = 15; i > 0; i--) {
				lastx[i] = lastx[i-1];
				lasty[i] = lasty[i-1];
				frame.SetPointColor(lastx[i], lasty[i], 0,0,0, 15-i);
			}

			currx = x;
			curry = y;

			lastx[0] = currx;
			lasty[0] = curry;

			frame.SetPointColor(currx,curry,0,0,0,15);

			if ((lastx[1] == currx) && (lasty[1] == curry)) {
				samespot++;
			} else {
				samespot = 0;
			}

			if (samespot == 50) {
				bounce_setup();
			}
		}
	}

	frame.RefreshGrey();
	return true;

};
