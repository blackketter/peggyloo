

// ------------------------------------------------------------
// inclusion
// ------------------------------------------------------------
#include <PeggyLoo.h>
#include <Time.h>
#include "main.h"
#include "cube.h"
#include "qix.h"
#include "solid.h"
#include "color_shifter.h"
#include "color_wave.h"
#include "clock.h"
#include "shower.h"
#include "noise.h"
#include "strings.h"
#include "bounce.h"
#include "randomslide.h"

// ------------------------------------------------------------
// Objects
// ------------------------------------------------------------
PeggyLoo frame;

typedef enum {
	INIT_MODE = (-1),
	FIRST_MODE = (0),
	
	CLOCK_MODE = FIRST_MODE,
	RANDOMSLIDE_MODE,
	STRINGS_MODE,
	QIX_MODE,
	NOISE_MODE,
	SHOWER_MODE,
	CUBE_MODE,
	COLOR_WAVE_MODE,
	SOLID_MODE,
	BOUNCE_MODE,
	
	LAST_MODE = BOUNCE_MODE,
	
	// disabled modes
	COLOR_SHIFTER_MODE,

} Mode;

Mode peggyloo_mode = INIT_MODE;

unsigned long lastAutoShift = 0;

// autoshift modes every 5 minutes
#define AUTOSHIFT_INTERVAL (60*1000L)

void setup()
{
  	frame.HardwareInit();   // Call this once to init the hardware. 
};


// ------------------------------------------------------------
// loop
// ------------------------------------------------------------
void loop()
{
	boolean running = false;
	boolean skip = false;
	
	unsigned long now = millis();
	
	if (frame.LastButtonTime() > lastAutoShift) {
		lastAutoShift = frame.LastButtonTime();
	}
	
	// auto shift to the next mode, if the button is up and we've timed out
	if (!frame.Button() && ((now - lastAutoShift) > AUTOSHIFT_INTERVAL)) {
			skip = true;
			lastAutoShift = now;
	}

	switch (peggyloo_mode) {
		case  CLOCK_MODE:
			running = clock_loop(skip);
			break;
		case  BOUNCE_MODE:
			running = bounce_loop(skip);
			break;
		case  RANDOMSLIDE_MODE:
			running = randomslide_loop(skip);
			break;
		case  SHOWER_MODE:
			running = shower_loop(skip);
			break;
		case  STRINGS_MODE:
			running = strings_loop(skip);
			break;
		case  NOISE_MODE:
			running = noise_loop(skip);
			break;
		case  CUBE_MODE:
			running = cube_loop(skip);
			break;
		case QIX_MODE:
			running = qix_loop(skip);
			break;
		case SOLID_MODE:
			running = solid_loop(skip);
			break;
		case COLOR_SHIFTER_MODE:
			running = color_shifter_loop(skip);
			break;
		case COLOR_WAVE_MODE:
			running = color_wave_loop(skip);
			break;
		default:
			break;
	}

	if (skip) {
		// autoshift timer kicked in, pick a random mode
		// it's ok if it's the same mode as before
		// unless it's between 11pm and 6:30am, in which case we turn into a clock
		if (frame.Night()) {
				peggyloo_mode = CLOCK_MODE;
		} else {
				peggyloo_mode = (Mode)random(LAST_MODE+1);
	    }
	}
	
	// shift to the next mode if the mode has stopped running (or the autoshift has kicked in)
	if (!running) { 

		// mode exited on its own due to a button press, go on to the next mode
		if (!skip)
			peggyloo_mode = (Mode)((int)peggyloo_mode + 1);
			
		if (peggyloo_mode > LAST_MODE) {
			peggyloo_mode = FIRST_MODE;
		}
		
		switch (peggyloo_mode) {
			case  CUBE_MODE:
				cube_setup();
				break;
			case  CLOCK_MODE:
				clock_setup();
				break;
			case  RANDOMSLIDE_MODE:
				randomslide_setup();
				break;
			case  SHOWER_MODE:
				shower_setup();
				break;
			case  BOUNCE_MODE:
				bounce_setup();
				break;
			case  STRINGS_MODE:
				strings_setup();
				break;
			case  NOISE_MODE:
				noise_setup();
				break;
			case QIX_MODE:
				qix_setup();
				break;
			case SOLID_MODE:
				solid_setup();
				break;
			case COLOR_SHIFTER_MODE:
				color_shifter_setup();
				break;
			case COLOR_WAVE_MODE:
				color_wave_setup();
				break;
		}
	}

};

