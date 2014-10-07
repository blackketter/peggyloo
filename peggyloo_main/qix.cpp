/* Simple example code for Peggy 2.0, using the PeggyLoo library, version 0.2.

 Create a light that bounces off Paggy's "walls", and leaves a tail of 4 lights behind it of decreasing brightness

 Copyright (c) 2009 David S. Kauffman
 inspired by Peggy 2.0 example code from Windell Oskay

 This example is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 */

#include "Arduino.h"
#include <PeggyLoo.h>
#include "main.h"
#include "qix.h"

// if QIX is "1", acts like the QIX game, "0" and it acts as a wall bouncer. It's all in how the next position
// is calculated.  One day I want to add a toggle switch for run time control
#define QIX  0

// TAIL determines how long the echoes behind hte bouncing ball are. Since there are only 16 gray levels..
#define MAXTAILLENGTH 16
#define TAIL MAXTAILLENGTH

// xpos and ypos are arrays that keep track of the last few locations of the ball, so it can draw the tail
short xpos[TAIL], ypos[TAIL];

// these variable control whether the ball is moving up, down, left or right.
short xdir = 1;
short ydir = 1;


void qix_setup()                    // run once, when the sketch starts
{
  unsigned short i;

  // pick a random starting spot
  xpos[0] = random (PEGGYLOO_WIDTH);
  ypos[0] = random (PEGGYLOO_HEIGHT);
  // set the directions
  xdir = 1;
  ydir = 1;

  // fill in some initial values of the tail so we don't just light up the (0,0) lights.
  for (i = 1; i < TAIL; i++) {
    xpos [i] = xpos [i-1] + xdir;
    ypos [i] = ypos [i-1] + ydir;
  }
}  // End void setup()


boolean qix_loop(boolean skip)                     // run over and over again
{
  short i, j;
  short reps;

  // move the ball if the button is not down
  if (!frame.Button()) {

	// Clear frame buffer
	  frame.ClearGrey();

	  //this loop shuffles the tail so that it keeps the last few places the ball has been
	  // it throws the away the oldest value, the farthest end of the tail
	  for (i = TAIL-1; i > 0; i--) {
		xpos[i] = xpos [i-1];
		ypos[i] = ypos [i-1];
	  }

	  // Each of these 4 blocks moves the ball, then checks to see if it is hitting a wall. If so, change the direction
	  // The call to random controls the angle of the bounce, otherwise the bouncing gets very boring...
	  if (QIX)
		xpos [0] += random (3) - 1;    // randomly turn left, stay, or 1 right
	  else
		xpos [0] += xdir;              // stay the course

	  if (xpos [0] >= PEGGYLOO_WIDTH) {            // off the right edge
		xdir = - (random (2) + 1);    // pick a new random velocity to the left
		xpos [0] = PEGGYLOO_WIDTH-1;                // stay on board
	  }
	  if (xpos [0] < 0) {            // off the left edge
		xdir = random (2) + 1;
		xpos [0] = 0;
	  }

	  if (QIX)
		ypos [0] += random (3) - 1;    // randomly turn left, stay, or 1 right
	  else
		ypos [0] += ydir;              // stay the course

	  if (ypos [0] >= PEGGYLOO_HEIGHT) {
		ydir = - (random (2) + 1);
		ypos [0] = PEGGYLOO_HEIGHT-1;
	  }
	  if (ypos [0] < 0) {
		ydir = random (2) + 1;
		ypos [0] = 0;
	  }
	  // Set the new ball location, and its "echoes" behind it of where it has been
	  for (i = 0; i < TAIL; i++) {
		int col = MAX_BRIGHTNESS - i*(BRIGHTNESS_LEVELS/MAXTAILLENGTH);
		int xp = xpos [i];
		int yp = ypos [i];
		frame.SetPointGrey(xp,yp,col);
	  }
  }

  frame.RefreshGrey();

  // if the button is held for less than 1000ms, move on to the next mode
  unsigned long br = frame.ButtonReleased();

  if ( skip ||((br > 0) && (br < 1000)) ) {
  	return false;
  } else {
  	return true;
  }

}
