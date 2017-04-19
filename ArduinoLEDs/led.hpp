/*
    This file is part of libReimu.
    Copyright (C) 2016-2017  Dustin Van Tate Testa <toast27@gmail.com>

    libReimu is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libReimu is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libReimu.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <inttypes.h>

#include "color.hpp"

namespace Alice {

	// tri-color PWM RGB LED class
	class TriLED {
	public:
	  Color color;
	  unsigned int pr : 7, pg : 7, pb : 7;

	  TriLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin):
		pr(redPin), pg(greenPin), pb(bluePin), color(0,0,0)
	  {
		pinMode(pr, OUTPUT);
		pinMode(pg, OUTPUT);
		pinMode(pb, OUTPUT);
	  }

	  TriLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, Color _clr):
		pr(redPin), pg(greenPin), pb(bluePin), color(_clr)
	  {
		pinMode(pr, OUTPUT);
		pinMode(pg, OUTPUT);
		pinMode(pb, OUTPUT);
	  }

	  // burn `color` into the led (apply the values)
	  void refresh(){
		analogWrite(pr, color.r);
		analogWrite(pg, color.g);
		analogWrite(pb, color.b);
	  }

	  // change and apply the color
	  void setColor(const Color& clr){
		color = clr;
		analogWrite(pr, color.r);
		analogWrite(pg, color.g);
		analogWrite(pb, color.b);
	  }

	  void setColor(DigitalColor clr){

		color.r = clr.r;
		color.g = clr.g;
		color.b = clr.b;

		digitalWrite(pr, clr.r);
		digitalWrite(pg, clr.g);
		digitalWrite(pb, clr.b);
	  }

	  void setColor()
		{ refresh(); }

	  void set(const Color& clr)
		{ setColor(clr); }

	  void set()
		{ refresh(); }

	  void set(uint8_t val){
		color.r = color.g = color.b = val;
		refresh();
	  }

	  void set(uint8_t r, uint8_t g, uint8_t b){
		color.r = r;
		color.g = g;
		color.b = b;
		refresh();
	  }

	  void set(DigitalColor clr)
		{ setColor(clr); }

	  // change the color without applying the changes
	  void push(const Color& clr)
		{ color = clr; }

	  void push(const DigitalColor clr){
		color.r = clr.r ? 255 : 0;
		color.g = clr.g ? 255 : 0;
		color.b = clr.b ? 255 : 0;
	  }

	  void push(uint8_t val)
		{ color.r = color.g = color.b = val; }

	  void push(uint8_t r, uint8_t g, uint8_t b){
		color.r = r;
		color.g = g;
		color.b = b;
	  }


	  // sets output to zero without clearing the values
	  void off(){
		digitalWrite(pr, 0);
		digitalWrite(pg, 0);
		digitalWrite(pb, 0);
	  }

	  // set values to 0 but don't call refresh()
	  void setNull()
		{ color = Color(0, 0, 0); }

	  // hardcore OOP users use this :
	  Color& getColor()
		{ return color; }


	  // uses a scheme string to determine the color pattern.
	  void colorCycle(const char order[4], uint8_t& curHi, uint8_t incr = 1){

		// I don't like this solution, but it works...
		// it's better than my 300+ SLOC branching solution.
		uint8_t *c1, *c2, *c3;

		if (*order == 'r')
		  c1 = &color.r;
		else if (*order == 'g')
		  c1 = &color.g;
		else if (*order == 'b')
		  c1 = &color.b;
		else
		  c1 = &color.r;

		order++; //next char

		if (*order == 'r')
		  c2 = &color.r;
		else if (*order == 'g')
		  c2 = &color.g;
		else if (*order == 'b')
		  c2 = &color.b;
		else
		  c2 = &color.g;

		order++; //next char

		if (*order == 'r')
		  c3 = &color.r;
		else if (*order == 'g')
		  c3 = &color.g;
		else if (*order == 'b')
		  c3 = &color.b;
		else
		  c3 = &color.b;

		while (incr-- > 0)
		  color::cycle3(*c1, *c2, *c3, curHi);

		/*
		// for debugging only (uses too much resources)
		 Serial.print("r:");
		 Serial.print(color.r, DEC);
		 Serial.print(" g:");
		 Serial.print(color.g, DEC);
		 Serial.print(" b:");
		 Serial.print(color.b, DEC);
		 Serial.print(" CurHi: ");
		 Serial.println(curHi, DEC);
		*/
		refresh();

	  }

	  // wrapper for color::cycle3
	  void colorCycle(const char order[4], uint8_t incr = 1){

		static uint8_t curHi = 0;

		colorCycle(order, curHi, incr);

	  }


	  // wrapper for color::cycle3
	  void inverseColorCycle(uint8_t incr = 1){

		static uint8_t curHi = 0;

		while (incr-- > 0)
		  color::cycle3(color.r, color.g, color.b, curHi);

		writeInverse();
	  }

	  // wrapper for color::cycle3
	  void colorCycle(uint8_t incr = 1){

		static uint8_t curHi = 0;

		while (incr-- > 0)
		  color::cycle3(color.r, color.g, color.b, curHi);

		refresh();
	  }

	  // wrapper for color::cycle3
	  void colorCycle(uint8_t& curHi, uint8_t incr = 1){
		while (incr-- > 0)
		  color::cycle3(color.r, color.g, color.b, curHi);

		refresh();
	  }

	  // sets the analog equivalent of the digital value
	  void digitalSetColor(const Color& clr){
		color.r = clr.r ? 255 : 0;
		color.g = clr.g ? 255 : 0;
		color.b = clr.b ? 255 : 0;
		refresh();
	  }
	  void digitalSetColor(DigitalColor clr){
		color.r = clr.r ? 255 : 0;
		color.g = clr.g ? 255 : 0;
		color.b = clr.b ? 255 : 0;
		refresh();
	  }


	  // uses digitalWrite instead of analogWrite
	  void digitalRefresh() {
		//color.r = color.r ? 255 : 0;
		//color.g = color.g ? 255 : 0;
		//color.b = color.b ? 255 : 0;
		digitalWrite(pr, color.r);
		digitalWrite(pg, color.g);
		digitalWrite(pb, color.b);
	  }

	  // same as in DigitalTriLED
	  void digitalColorCycle(char order[4]){
		char curHi = color.r?
		                'r' : color.g?
		                  'g' : color.b?
		                    'b': *order;


		uint8_t i = 0;
		while (curHi != *(order + i) && i <= 4)
		  i++;

		i++;

		curHi = (i < 3) ? *(order + i) : *order;

		switch (curHi) {
		case 'r':
		  color.r = HIGH;
		  color.g = LOW;
		  color.b = LOW;
		  break;

		case 'g':
		  color.r = LOW;
		  color.g = HIGH;
		  color.b = LOW;
		  break;

		case 'b':
		  color.r = LOW;
		  color.g = LOW;
		  color.b = HIGH;
		  break;
		}

		digitalRefresh();

	  }

	  // same as in DigitalTriLED
	  void digitalColorCycle(){

		char nextHi = color.g?
		                'b' : color.b?
		                  'r' : 'g';

		switch (nextHi) {
		case 'r':
		  color.r = HIGH;
		  color.g = LOW;
		  color.b = LOW;
		  break;

		case 'g':
		  color.r = LOW;
		  color.g = HIGH;
		  color.b = LOW;
		  break;

		case 'b':
		  color.r = LOW;
		  color.g = LOW;
		  color.b = HIGH;
		  break;
		}

		digitalRefresh();
	  }

	  // changes the color to its inverse
	  void invert(){
		color.r = 255 - color.r;
		color.g = 255 - color.g;
		color.b = 255 - color.b;
		refresh();
	  }

	  // writes the inverse without modifying the values
	  void writeInverse(){
		analogWrite(pr, 255 - color.r);
		analogWrite(pg, 255 - color.g);
		analogWrite(pb, 255 - color.b);
	  }

	  // this will never get used...
	  void swapPins(uint8_t redPin, uint8_t bluePin, uint8_t greenPin){
		pr = redPin;
		pg = greenPin;
		pb = bluePin;
		// set them as output
		pinMode(pr, OUTPUT);
		pinMode(pg, OUTPUT);
		pinMode(pb, OUTPUT);

		refresh();
	  }

	};

	// tri-color RGB LED class
	class DigitalTriLED {
	public:
	  bool r : 1, g : 1, b : 1;
	  unsigned int pr : 7, pg : 7, pb : 7;

	  DigitalTriLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin):
		pr(redPin), pg(greenPin), pb(bluePin), r(LOW), g(LOW), b(LOW)
	  {
		pinMode(pr, OUTPUT);
		pinMode(pg, OUTPUT);
		pinMode(pb, OUTPUT);
	  }

	  DigitalTriLED(uint8_t redPin, uint8_t greenPin, uint8_t bluePin, Color _clr):
		pr(redPin), pg(greenPin), pb(bluePin), r(_clr.r), g(_clr.g), b(_clr.b)
	  {
		pinMode(pr, OUTPUT);
		pinMode(pg, OUTPUT);
		pinMode(pb, OUTPUT);
	  }

	  // apply values
	  void refresh(){
		digitalWrite(pr, r);
		digitalWrite(pg, g);
		digitalWrite(pb, b);
	  }

	  // apply a color
	  void setColor(bool red, bool green, bool blue){
		r = red;
		g = green;
		b = blue;
		refresh();
	  }

	  void setColor(const Color& clr){
		r = clr.r;
		g = clr.g;
		b = clr.b;
		refresh();
	  }

	  void setColor(DigitalColor clr){
		r = clr.r;
		g = clr.g;
		b = clr.b;
		refresh();
	  }

	  void setColor()
		{ refresh(); }

	  void set()
		{ refresh(); }

	  void set(const Color& clr)
		{ setColor(clr); }

	  void set(DigitalColor clr)
		{ setColor(clr);}

	  void set(bool red, bool green, bool blue)
		{ return setColor(red, green, blue); }

	  void set(const bool val){
		r = val;
		g = val;
		b = val;
		refresh();
	  }

	  // pushes values but doesn't refresh
	  void push(const Color& clr){
		r = clr.r;
		g = clr.g;
		b = clr.g;
	  }
	  void push(DigitalColor clr){
		r = clr.r;
		g = clr.g;
		b = clr.g;
	  }

	  // sets output to zero without clearing the values
	  void off(){
		digitalWrite(pr, 0);
		digitalWrite(pg, 0);
		digitalWrite(pb, 0);
	  }

	  // set all values to 0 (no refresh)
	  void setNull(){
		r = LOW;
		g = LOW;
		b = LOW;
	  }

	  void colorCycle(char order[4]){
		// determine which pin is on HIGH
		char curHi = r ?
		    'r' : g ?
		      'g' : b?
		        'b': *order;

		// find fist occurance of current color (this is difficult to achieve)
		uint8_t i = 0;
		while (curHi != *(order + i) && i <= 4)
		  i++;

		i++; // we want the next color to change to

		// prevents errors
		curHi = (i < 3) ? *(order + i) : *order;

		// set correct color
		switch (curHi) {
		case 'r':
		  r = HIGH;
		  g = LOW;
		  b = LOW;
		  break;

		case 'g':
		  r = LOW;
		  g = HIGH;
		  b = LOW;
		  break;

		case 'b':
		  r = LOW;
		  g = LOW;
		  b = HIGH;
		  break;
		}


		refresh();

	  }

	  // follow a scheme string to cycle between color values
	  void colorCycle(char* order, uint8_t len){

		// determine curhi based on which color is currently HIGH
		char curHi = r?
		    'r' : g?
		      'g' : b?
		        'b': *order;



		// find position in string where we are currrently (very inaccurate)
		uint8_t i = 0;
		while (curHi != *(order + i) && i < len)
		  i++;

		i++; // we want the color after where we are..

		// just to prevent problems
		curHi = (i < len) ? *(order + i) : *order;

		// set in the correct value
		switch (curHi) {
		case 'r':
		  r = HIGH;
		  g = LOW;
		  b = LOW;
		  break;

		case 'g':
		  r = LOW;
		  g = HIGH;
		  b = LOW;
		  break;

		case 'b':
		  r = LOW;
		  g = LOW;
		  b = HIGH;
		  break;

		}

		refresh();

	  }

	  // cycle rgb
	  void colorCycle(){

		// determine nextHi based on which color is HIGH
		char nextHi = g ? 'b' : ( b? 'r': 'g');

		// set the LED correct color
		switch (nextHi) {
		case 'r':
		  r = HIGH;
		  g = LOW;
		  b = LOW;
		  break;

		case 'g':
		  r = LOW;
		  g = HIGH;
		  b = LOW;
		  break;

		case 'b':
		  r = LOW;
		  g = LOW;
		  b = HIGH;
		  break;
		}

		refresh();
	  }

	  // invert current color
	  void invert(){
		r = !r;
		g = !g;
		b = !b;
	  }

	  // this will never get used...
	  void swapPins(uint8_t redPin, uint8_t bluePin, uint8_t greenPin){
		pr = redPin;
		pg = greenPin;
		pb = bluePin;
		// set them as output
		pinMode(pr, OUTPUT);
		pinMode(pg, OUTPUT);
		pinMode(pb, OUTPUT);

		refresh();
	  }

	};



	// these will no longer get used...
	// but they're still useful...


	// 2 digital LEDs
	class BiLED {
	public:
	  unsigned int p0 : 7, p1 : 7;
	  bool v0 : 1, v1 : 1; // could be replaced by uint8_t for PWM output

	  BiLED(uint8_t pin0, uint8_t pin1):
		p0(pin0), p1(pin1), v0(0), v1(0)
	  {
		pinMode(p0, OUTPUT);
		pinMode(p1, OUTPUT);
	  }

	  BiLED(uint8_t pin0, uint8_t pin1, const bool val0, const bool val1):
		p0(pin0), p1(pin1), v0(val0), v1(val1)
	  {
		pinMode(p0, OUTPUT);
		pinMode(p1, OUTPUT);
	  }

	  // burn in values
	  void refresh(){
		digitalWrite(p0,v0);
		digitalWrite(p1,v1);
	  }

	  // push and apply values
	  void set(uint8_t val0, uint8_t val1){
		v0 = val0;
		v1 = val1;
		refresh();
	  }

	  // push and apply the same value for v0 & v1
	  void set(uint8_t val){
		v0 = val;
		v1 = val;
		refresh();
	  }

	  // sets output to zero without clearing the values
	  void off(){
		digitalWrite(p0, 0);
		digitalWrite(p1, 0);
	  }

	  // replace v0 and v1
	  void swap(){
		bool temp = v0;
		v0 = v1;
		v1 = temp;
		refresh();
	  }


	  // this will never get used...
	  void swapPins(const uint8_t pin0, const uint8_t pin1){
		// note: this doesn't set(0) before swapping the pins.

		// set them as output
		pinMode(pin0, OUTPUT);
		pinMode(pin1, OUTPUT);

		p0 = pin0;
		p1 = pin1;

		// burn in current color
		refresh();
	  }
	};

	// 2 LEDs with PWM capabilities
	class BiLED_pwm {
	public:
	  uint8_t p0, p1; // making this a bitfield wouldn't improve anything
	  uint8_t v0, v1;

	  BiLED_pwm(uint8_t pin0, uint8_t pin1):
		p0(pin0), p1(pin1), v0(0), v1(0)
	  {
		pinMode(p0, OUTPUT);
		pinMode(p1, OUTPUT);
	  }
	  BiLED_pwm(uint8_t pin0, uint8_t pin1, uint8_t val0, uint8_t val1):
		p0(pin0), p1(pin1), v0(val0), v1(val1)
	  {
		pinMode(p0, OUTPUT);
		pinMode(p1, OUTPUT);
	  }

	  // burns in values
	  void refresh(){
		analogWrite(p0, v0);
		analogWrite(p1, v1);
	  }

	  // sets values and applys
	  void set(uint8_t val0, uint8_t val1){
		v0 = val0;
		v1 = val1;
		refresh();
	  }

	  // sets output to zero without clearing the values
	  void off(){
		analogWrite(p0, 0);
		analogWrite(p1, 0);
	  }

	  // switch v0 and v1
	  void swap(){
		uint8_t temp = v0;
		v0 = v1;
		v1 = temp;
		refresh();
	  }

	  // reciprocate between v0 and v1
	  void seeSaw(uint8_t incr = 1){

		static bool curHi = 0;

		while (incr-- > 0) {

		  if (v0 == 0|| v1 == 0)
		    curHi = !curHi;
		  if (!curHi) {
		    v0--; v1++;
		  } else {
		    v1--; v0++;
		  }

		}
		refresh();
	  }
	  // reciprocate between v0 and v1
	  void seeSaw(bool& curHi, uint8_t incr = 1){

		while (incr-- > 0) {

		  // switch directions
		  if (v0 == 0 || v1 == 0)
		    curHi = !curHi;

		  if (!curHi) {
		    v0--; v1++;
		  } else {
		    v1--; v0++;
		  }

		}

		refresh();
	  }

	  // this will never get used...
	  void swapPins(uint8_t pin0, uint8_t pin1){
		// note: this doesn't set(0) before swapping the pins.

		p0 = pin0;
		p1 = pin1;

		// set them as output
		pinMode(p0, OUTPUT);
		pinMode(p1, OUTPUT);

		// burn in current color
		refresh();
	  }

	};

}


#endif
