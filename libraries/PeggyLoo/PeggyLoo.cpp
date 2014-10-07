/*
  PeggyLoo.cpp - PeggyLoo LED Matrix library for Arduino
  LIBRARY VERSION: 0.30b, DATED 7/08/2008

  
  
  Copyright (c) 2008 Windell H Oskay.  All right reserved.

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
  
  Line code contributed by Michaël Zancan & Julien 'v3ga' Gachadoat
  websites: http://www.2roqs.com    http://www.zancan.fr    http://www.v3ga.net
  
*/


extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h> 
  #include <avr/io.h>
}


//#include "WProgram.h"
#include <Time.h>
#include "PeggyLoo.h" 

#define PEGGYLOO_FRAMESIZE (PEGGYLOO_WIDTH*sizeof(uint32_t))
#define PEGGYLOO_FRAMEWORDS (PEGGYLOO_WIDTH)
#define PEGGYLOO_BUFFERWORDS (PEGGYLOO_WIDTH * FRAMEBUFFERS)
#define PEGGYLOO_BUFFERSIZE (PEGGYLOO_FRAMESIZE * FRAMEBUFFERS)

#define PEGGY_COLUMN_OFF 0

#define PEGGY_COLUMN_ON (0xffffffff) 


// Constructors ////////////////////////////////////////////////////////////////
PeggyLoo::PeggyLoo()
{
  buffer = (uint32_t*)calloc(PEGGYLOO_FRAMESIZE, FRAMEBUFFERS);
}

 
void PeggyLoo::SPI_TX(char cData)
{
  SPDR = cData;
  //Wait for transmission complete:
  while (!(SPSR & _BV(SPIF))) ;
} 


void PeggyLoo::HardwareInit()
{
  //  Hardware Initialization:
  
  PORTD = 0U;
  DDRD = 255U;
  
  ////SET MOSI, SCK Output, all other SPI as input: 
  DDRB |= _BV(5) | _BV(3) | _BV(2) | _BV(1);
  
  //ENABLE SPI, MASTER, CLOCK RATE fck/4:  
  SPCR =  _BV(SPE) |  _BV(MSTR) ;
  
  //  Flush SPI LED drivers::
  SPI_TX(0);
  SPI_TX(0);
  SPI_TX(0);
  SPI_TX(0);

  PORTB |= 2;    //Latch Pulse 
  PORTB &= 253;  
  
  DDRB = 254;		//All outputs except B0, the "OFF/SELECT" button.
  PORTB |= 1;            // Turn on PortB Pin 0 pull up!

  lastButtonTime = 0;
  lastButton = false;
  
  InitGrey();
}

unsigned long PeggyLoo::ButtonReleased() {
	boolean nowButton = Button();
	
	if (nowButton == lastButton) {
		return 0;	
	}

	unsigned long nowButtonTime = millis();
	
	if (nowButton) {
		lastButton = true;
		lastButtonTime = nowButtonTime;
		return 0;
	} else {
	    lastButton = false;
	    unsigned long delta = nowButtonTime - lastButtonTime;
	    lastButtonTime = nowButtonTime;
	    return delta;
	}		
}

unsigned long PeggyLoo::ButtonHeld() {
	boolean nowButton = Button();
	
	if (!nowButton) {
		return 0;	
	}

	unsigned long nowButtonTime = millis();
	
	return (nowButtonTime - lastButtonTime);
}

unsigned long PeggyLoo::ButtonPressed() {
	boolean nowButton = Button();
	
	if (nowButton == lastButton) {
		return 0;	
	}

	unsigned long nowButtonTime = millis();
	
	if (nowButton) {
		lastButton = true;
	    unsigned long delta = nowButtonTime - lastButtonTime;
	    lastButtonTime = nowButtonTime;
	    return delta;
	} else {
	    lastButton = false;
		lastButtonTime = nowButtonTime;
		return 0;
	}		
}

unsigned long PeggyLoo::LastButtonTime() {
	return lastButtonTime;
}
boolean PeggyLoo::Button() {
	boolean state = ((PINB & 1) == 0);
	return state;
}


void delay()
{
unsigned int delayvar;
	delayvar = 0; 
	while (delayvar <=  6000U)		
		{ 
			asm("nop");  
			delayvar++;
		}
}

void PeggyLoo::Refresh(uint8_t frame, unsigned int refreshNum, boolean fast)
{
  unsigned int k;
  unsigned char j;
  
  union mix_t {
    unsigned long atemp; 
    unsigned char c[4];
  } mix;
  
  if (dark) {
  	return;
  }
  
  k = 0;
  
  int offset = frame*PEGGYLOO_FRAMEWORDS;

  while (k != refreshNum)   
  {
    k++;
    j = 0;
    while (j <= (PEGGYLOO_WIDTH)) 
    {

      if (j == 0)
        PORTD = 160;
      else if (j < 16)
        PORTD = j;
      else
        PORTD = (j - 15) << 4;  

	  if (!fast) {
		  unsigned char i = 0;
		  while (i < 50)
		  {
		  asm("nop"); 
		  i++;
		  }
      }  

  	  mix.atemp = buffer[j+offset];
	  
      SPI_TX(mix.c[3]);
      SPI_TX(mix.c[2]);
      SPI_TX(mix.c[1]);
	  
      PORTD = 0;  // Turn displays off
 	  
      SPI_TX(mix.c[0]); 
      PORTB |= 2U;    //Latch Pulse 
      PORTB &= 253U;
      
      j++;
    }
  }
}

void PeggyLoo::Clear(uint8_t frame) 
{
  memset((uint8_t*)buffer+(frame*PEGGYLOO_FRAMESIZE), 0, PEGGYLOO_FRAMESIZE);
}

void PeggyLoo::Fill(uint8_t frame)
{
  memset((uint8_t*)buffer+(frame*PEGGYLOO_FRAMESIZE), 0xff, PEGGYLOO_FRAMESIZE);
}

void PeggyLoo::WritePoint(uint8_t frame, uint8_t xPos, uint8_t yPos, uint8_t Value)
{
  if (Value)
    buffer[xPos+frame*PEGGYLOO_FRAMEWORDS] |= (uint32_t) 1 << yPos;
  else
    buffer[xPos+frame*PEGGYLOO_FRAMEWORDS] &= ~((uint32_t) 1 << yPos);
}

void PeggyLoo::WriteColumn(uint8_t frame, uint8_t xPos, uint32_t column)
{
  buffer[xPos+frame*PEGGYLOO_FRAMEWORDS] = column;
}

void PeggyLoo::WriteRow(uint8_t frame, uint8_t yPos, uint32_t row) 
{
	uint32_t write_mask = (uint32_t)1 << yPos;
	uint32_t read_mask = 1;
	
	for (int i = 0; i < PEGGYLOO_WIDTH; i++) {
		if (read_mask & row) {
			buffer[frame*PEGGYLOO_FRAMEWORDS + i] |= write_mask;
		} else {
		    buffer[frame*PEGGYLOO_FRAMEWORDS + i] &= ~write_mask;
		}
		read_mask <<= 1;
	}
}

void PeggyLoo::SetColumn(uint8_t frame, uint8_t xPos, uint32_t column = PEGGY_COLUMN_ON)
{
  buffer[xPos+frame*PEGGYLOO_FRAMEWORDS] |= column;
}

void PeggyLoo::Shift(uint8_t frame, uint8_t direction)
{
	int i;
	switch (direction) {
		case PEGGYLOO_DOWN:
			for (i = 0; i<PEGGYLOO_FRAMEWORDS; i++) {
				buffer[i+frame*PEGGYLOO_FRAMEWORDS] <<= 1;
			}
			break;
		case PEGGYLOO_UP:
			for (i = 0; i<PEGGYLOO_FRAMEWORDS; i++) {
				buffer[i+frame*PEGGYLOO_FRAMEWORDS] >>= 1;
			}		
			break;
		case PEGGYLOO_LEFT:
			for (i = 0; i<PEGGYLOO_FRAMEWORDS-1; i++) {
				buffer[i+frame*PEGGYLOO_FRAMEWORDS] = buffer[i+frame*PEGGYLOO_FRAMEWORDS + 1];
			}
			buffer[i+frame*PEGGYLOO_FRAMEWORDS] = 0;
			break;
		case PEGGYLOO_RIGHT:
			for (i = PEGGYLOO_FRAMEWORDS - 2; i >= 0; i--) {
				buffer[frame*PEGGYLOO_FRAMEWORDS + i + 1] = buffer[frame*PEGGYLOO_FRAMEWORDS + i];
			}
			buffer[frame*PEGGYLOO_FRAMEWORDS] = 0;
			break;
	}
}

void PeggyLoo::ShiftGrey(uint8_t direction) {

	int i;
	switch (direction) {
		case PEGGYLOO_DOWN:
			for (i == 0; i<PEGGYLOO_BUFFERWORDS; i++) {
				buffer[i] <<= 1;
			}
			break;
		case PEGGYLOO_UP:
			for (i == 0; i<PEGGYLOO_BUFFERWORDS; i++) {
				buffer[i] >>= 1;
			}		
			break;
		case PEGGYLOO_LEFT:
		case PEGGYLOO_RIGHT:
			// todo: optimize this
			for (i = 0; i < FRAMEBUFFERS; i++) {
				Shift(i,direction);
			}
			break;
	}
}

void PeggyLoo::ShiftColor(uint8_t direction) {

	int i;
	switch (direction) {
		case PEGGYLOO_DOWN:
			for (i == 0; i<PEGGYLOO_BUFFERWORDS; i++) {
				buffer[i] <<= 2;
			}
			break;
		case PEGGYLOO_UP:
			for (i == 0; i<PEGGYLOO_BUFFERWORDS; i++) {
				buffer[i] >>= 2;
			}		
			break;
		case PEGGYLOO_LEFT:
		case PEGGYLOO_RIGHT:
			// todo: optimize this
			for (i = 0; i < FRAMEBUFFERS; i++) {
				Shift(i,direction);
				Shift(i,direction);
			}
			break;
	}
}
// should ClearColumn be implemented? if you only want to clear portions of the column, 
// you'd need to send masks. Might be overly complex to document.

// Turn a pixel on
void PeggyLoo::SetPoint(uint8_t frame, uint8_t xPos, uint8_t yPos)
{
  buffer[xPos+frame*PEGGYLOO_FRAMEWORDS] |= (uint32_t) 1 << yPos; 
}

// Turn a pixel off
void PeggyLoo::ClearPoint(uint8_t frame, uint8_t xPos, uint8_t yPos)
{
  buffer[xPos+frame*PEGGYLOO_FRAMEWORDS] &= ~((uint32_t) 1 << yPos);
}

// Determine if a pixel is on or off
uint8_t PeggyLoo::GetPoint(uint8_t frame, uint8_t xPos, uint8_t yPos)
{
  return ((buffer[xPos+frame*PEGGYLOO_FRAMEWORDS] & (uint32_t) 1 << yPos) > 0); 
}


void PeggyLoo::Line(uint8_t frame, int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
  if ( (x1>=(PEGGYLOO_WIDTH) && x2>=(PEGGYLOO_WIDTH)) || (y1>=(PEGGYLOO_HEIGHT) && y2>=(PEGGYLOO_HEIGHT)) ) return;
  int8_t dx = abs(x2 -x1);
  int8_t dy = abs(y2 -y1);

  int8_t p1x,p1y,p2x,p2y,i;

  if (dx > dy)
  {
    if (x2>x1) {
      p1x=x1;
      p1y=y1;
      p2x=x2;
      p2y=y2;
    } 
    else {
      p1x=x2;
      p1y=y2;
      p2x=x1;
      p2y=y1;
    }
	
    int8_t y = p1y;
    int8_t x = p1x;
    int8_t count = 0;
    int8_t increment = p2y > p1y ? 1 : -1;
    for (i=0; i<=dx; i++)
    {	
      count += dy;
      if (count > dx)
      {
        count -= dx; 
        y+= increment;
      }				
      if (y>=0 && y<(PEGGYLOO_HEIGHT) && x>=0 && x<(PEGGYLOO_HEIGHT)) 
		SetPoint(frame,x,y);
      
		x++; 
      if (x>=(PEGGYLOO_WIDTH)) 
		break;
    }

  }
  else
  {
    if (y2>y1) {
      p1x=x1;
      p1y=y1;
      p2x=x2;
      p2y=y2;
    } 
    else {
      p1x=x2;
      p1y=y2;
      p2x=x1;
      p2y=y1;
    }
    int8_t y = p1y;
    int8_t x = p1x;
    int8_t count = 0;
    int8_t increment = p2x > p1x ? 1 : -1;
    for (i=0; i<=dy; i++)
    {	
      count += dx;
      if (count > dy)
      {
        count -= dy; 
        x+= increment;
      }				
      if (y>=0 && y<(PEGGYLOO_HEIGHT) && x>=0 && x<(PEGGYLOO_WIDTH)) SetPoint(frame,x,y);
      y+=1; 
      if (y>=(PEGGYLOO_HEIGHT)) break;
    }

  }
 
}
	
	 
	  

//Set current cursor position to (xPos,yPos)
void PeggyLoo::MoveTo(uint8_t frame, int8_t xPos, int8_t yPos)
{
 _Xcursor = xPos;
 _Ycursor = yPos;
}

//Draw a line from current cursor position to (xPos,yPos)
void PeggyLoo::LineTo(uint8_t frame, int8_t xPos, int8_t yPos)
{
  Line(frame, _Xcursor, _Ycursor, xPos, yPos);
 _Xcursor = xPos;
 _Ycursor = yPos;
}
	    


// Another quick utility method to round 

int Round(float x)
{
  return ((int)(x > 0.0 ? x + 0.5 : x- 0.5));
}

// This procedure implements the levels of greyscale by
// drawing the stars onto each frame accordingly, as per this
// chart
/*
        Greyscale
Frame   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
    0   -  x  -  x  -  x  -  x  -  x  -  x  -  x  -  x
    1   -  -  x  x  -  -  x  x  -  -  x  x  -  -  x  x
    2   -  -  -  -  x  x  x  x  -  -  -  -  x  x  x  x
    3   -  -  -  -  -  -  -  -  x  x  x  x  x  x  x  x
    
    and so on.  32 levels of greyscale uses 5 frames (0-4).
*/


void PeggyLoo::SetPointGrey (int x, int y, int grey)
{
  for (int i=0; i<FRAMEBUFFERS; i++)
  {
    if ((grey & intensity[i]) == intensity[i])
    {
      frame.SetPoint(i,x,y); 
  	} else {
  	  frame.ClearPoint(i,x,y);
  	}
  }
}

void PeggyLoo::FillGrey (int grey)
{
  for (int i=0; i<FRAMEBUFFERS; i++)
  {
    if ((grey & intensity[i]) == intensity[i])
    {
      frame.Fill(i);
  	} else {
  	  frame.Clear(i);
  	}
  }
}

void PeggyLoo::SetPointColor (int x, int y, int r, int g, int b, int w)
{  
  int x2 = x*2;
  int y2 = y*2;
  
  for (int i=0; i<FRAMEBUFFERS; i++)
  {
    int intense = intensity[i];
 
    if ((w & intense) == intense)
    {
      frame.SetPoint(i,x2,y2); 
  	} else {
  	  frame.ClearPoint(i,x2,y2);
  	}
    if ((r & intense) == intense)
    {
      frame.SetPoint(i,x2+1,y2); 
  	} else {
  	  frame.ClearPoint(i,x2+1,y2);
  	}
    if ((b & intense) == intense)
    {
      frame.SetPoint(i,x2,y2+1); 
  	} else {
  	  frame.ClearPoint(i,x2,y2+1);
  	}
    if ((g & intense) == intense)
    {
      frame.SetPoint(i,x2+1,y2+1); 
  	} else {
  	  frame.ClearPoint(i,x2+1,y2+1);
  	}
  }
}

// Redraws frame buffers to create greyscale 
void PeggyLoo::RefreshGrey()
{
   for (int i=0; i<FRAMEBUFFERS; i++)
    Refresh(i,intensity[i], 1);
}
  
void PeggyLoo::ClearGrey() 
{
  memset(buffer, 0, PEGGYLOO_BUFFERSIZE);
}

void PeggyLoo::InitGrey()
{
 for (int i=0; i<FRAMEBUFFERS; i++)  {
    intensity[i] = Round(pow (2.f, (float)i));
  }  
  
}

boolean PeggyLoo::Night() {
 return (hour() > 22 || 
		     hour() < 7 );
}

// utility functions

// random() only provides, apparently, 31 bits of randomness, shift and xor with self to get one more bit.  not accurate, but works
uint32_t random32() {
	uint32_t rand = random();
   	return rand ^ (rand << 1);
}