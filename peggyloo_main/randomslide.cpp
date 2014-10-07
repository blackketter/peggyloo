
#include "Arduino.h"
#include <PeggyLoo.h>
#include <Time.h>
#include "main.h"
#include "randomslide.h"

void randomslide_setup() {
//	frame.Clear(0);
};

int randomslide_iter = 0;
uint8_t randomslide_direction = PEGGYLOO_UP;

boolean randomslide_loop(boolean skip) {

	// if the button is held for less than 1000ms, move on to the next mode
	unsigned long br = frame.ButtonReleased();

	if (skip || ((br > 0) && (br < 1000)) ) {
		return false;
	}

	if (random(100) == 1) {
		switch (randomslide_direction) {
			case (PEGGYLOO_UP):
				randomslide_direction = PEGGYLOO_RIGHT;
				break;
			case (PEGGYLOO_RIGHT):
				randomslide_direction = PEGGYLOO_DOWN;
				break;
			case (PEGGYLOO_DOWN):
				randomslide_direction = PEGGYLOO_LEFT;
				break;
			case (PEGGYLOO_LEFT):
				randomslide_direction = PEGGYLOO_UP;
				break;
		}
//		randomslide_direction = random(4);
	}

	if (!frame.Button() && (randomslide_iter % 5) == 0) {
		// two shifts, to preserve color
		frame.Shift(0,randomslide_direction);
		frame.Shift(0,randomslide_direction);


		switch (randomslide_direction) {
			case (PEGGYLOO_UP):
				frame.WriteRow(0,PEGGYLOO_HEIGHT-2, random() & random() & random() & random() & 0x55555555);
				break;

			case (PEGGYLOO_DOWN):
				frame.WriteRow(0,0, random() & random() & random() & random() & 0x55555555);
				break;

			case (PEGGYLOO_LEFT):
				frame.WriteColumn(0, PEGGYLOO_WIDTH-2, random() & random() & random() & random() & 0x55555555);
				break;

			case (PEGGYLOO_RIGHT):
				frame.WriteColumn(0, 0, random() & random() & random() & random() & 0x55555555);
				break;

		}

	}
	randomslide_iter++;
	frame.Refresh(0,10);
	return true;

};
