/*
  PeggyLoo.h - Peggy 2.0 LED Matrix library for Arduino
  LIBRARY VERSION: 0.30b, DATED 7/08/2008



  Copyright (c) 2008 Windell H Oskay.  All right reserved.
  Significant modifications by Dean Blackketter, 2010

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modifications by Michael Yin, Copyright (c) 2008. All rights reserved.

  Line drawing code by Michaël Zancan & Julien 'v3ga' Gachadoat, Websites:
	http://www.2roqs.com
	http://www.zancan.fr
	http://www.v3ga.net

*/


#ifndef PeggyLoo_h
#define PeggyLoo_h

#include <inttypes.h>
#include "Arduino.h"

// utility function
int Round(float x);

// some constants

#define PEGGYLOO_WIDTH (24)
#define PEGGYLOO_HEIGHT (32)
#define PEGGYLOO_COLOR_WIDTH (PEGGYLOO_WIDTH/2)
#define PEGGYLOO_COLOR_HEIGHT (PEGGYLOO_HEIGHT/2)
#define FRAMEBUFFERS (4)
#define BRIGHTNESS_LEVELS (16)
#define MAX_BRIGHTNESS (BRIGHTNESS_LEVELS - 1)

#define PEGGYLOO_UP  (0)
#define PEGGYLOO_DOWN (1)
#define PEGGYLOO_LEFT (2)
#define PEGGYLOO_RIGHT (3)

#define WHITE_MASK_1 (0x55555555)
#define WHITE_MASK_2 (0x00000000)

#define BLUE_MASK_1   (0xAAAAAAAA)
#define BLUE_MASK_2   (0x00000000)

#define RED_MASK_1 (0x00000000)
#define RED_MASK_2 (0x55555555)

#define GREEN_MASK_1  (0x00000000)
#define GREEN_MASK_2  (0xAAAAAAAA)

class PeggyLoo
{
  public:
    PeggyLoo();

    // Onetime hardware initialization, don't need to run it for every frame.
    void HardwareInit();

    // Refresh this frame, refreshNum is how long to wait on each line; normally
	// use a refresh value of 1-10.
    void Refresh(uint8_t frame, unsigned int refreshNum, boolean fast = false);


	// Refresh this frame, refreshNum is the number of times this frame
    // will be refreshed in this call.

    // Clears out the FrameBuffer (all LEDs set to OFF)
    void Clear(uint8_t frame);

    // Fills the FrameBuffer (all LEDs set to ON)
    void Fill(uint8_t frame);

    // Turn point on or off logically
    // You can send true or false
    // or non-zero and zero
    void WritePoint(uint8_t frame, uint8_t xPos, uint8_t yPos, uint8_t Value);

    // Write over the entire column, each bit from position 0,24 represents
    // the LED position in the column.
    // example: WriteColumn(0, 32);
    // would set the 6th LED on the top Column (5th position)
    // 1 0 0 0 0 0  <- bit representation of 32
    // 5 4 3 2 1 0  <- power of 2
    // This function is a bit more complex to understand and is useful only
    // if you think you can set the bits faster than WritePoint would.
    void WriteColumn(uint8_t frame, uint8_t xPos, uint32_t column);
    void WriteRow(uint8_t frame, uint8_t yPos, uint32_t row);

	void Shift(uint8_t frame, uint8_t direction);

    // Turn a pixel on
    void SetPoint(uint8_t frame, uint8_t xPos, uint8_t yPos);

    // Turn a pixel off
    void ClearPoint(uint8_t frame, uint8_t xPos, uint8_t yPos);

	// Determine if a pixel is on or off
	uint8_t GetPoint(uint8_t frame, uint8_t xPos, uint8_t yPos);

    // Same as WriteColumn except it will only turn on bits that are 1, instead
    // of just writing over all bits. This is useful if you want to add new
    // lights to your frame buffer while keeping previous lights on.
    // By default, SetColumn(xPos) will set the entire column on.
    void SetColumn(uint8_t frame, uint8_t xPos, uint32_t column);

	//Draw a line from (x1,y1) to (x2,y2)
	void Line(uint8_t frame, int8_t x1, int8_t y1, int8_t x2, int8_t y2);

	//Set current cursor position to (xPos,yPos)
	void MoveTo(uint8_t frame, int8_t xPos, int8_t yPos);

	//Draw a line from current cursor position to (xPos,yPos)
	void LineTo(uint8_t frame, int8_t xPos, int8_t yPos);

	// greyscale functions
	void SetPointGrey (int x, int y, int grey);
	void RefreshGrey();
	void ClearGrey();
	void FillGrey(int grey);
	void InitGrey();
	void ShiftGrey(uint8_t direction);
	void ShiftColor(uint8_t direction);

	void SetPointColor(int x, int y, int r, int g, int b, int w);

    // return the time the button was held in milliseconds.  returns zero if it's still held down.
    unsigned long ButtonReleased();

    // return the time the button was released in milliseconds. returnd zero if it's still up.
	unsigned long ButtonPressed();

	// return the amount of time the button has been held in milliseconds
	unsigned long ButtonHeld();

	// last time the button transitioned (as detected by ButtonReleased or ButtonPressed)
	unsigned long LastButtonTime();

	// return the button state
	boolean Button();

    uint32_t* buffer;

    void Darken(boolean darken) { dark = darken; }
    boolean Night();

  private:
    void SPI_TX(char);
    uint8_t _Xcursor;
	uint8_t _Ycursor;
	unsigned long lastButtonTime;
	boolean lastButton;
	int intensity[FRAMEBUFFERS];   // Stores LED intensity for greyscale
	boolean dark;
};


extern PeggyLoo frame;

// utilities

static char sintable[60] = {
	0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -10, -10, -11, -11, -11, -11, -11, -10, -10, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 11, 11, 11, 11, 11, 10, 10, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
};

static char costable[60] = {
	-11, -11, -11, -10, -10, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 11, 11, 11, 11, 11, 10, 10, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -10, -10, -11, -11
};

// 32 bit random number generator
uint32_t random32();

#endif

