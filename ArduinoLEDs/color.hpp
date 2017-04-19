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

#ifndef COLOR_H
#define COLOR_H

#include <inttypes.h>

namespace Alice {

	// 24-bit color class
	class Color { // 16777216 colors
	public:
	  // 24-bit color
	  uint8_t r, g, b;

	  Color(): r(0), g(0), b(0) {}
	  Color(const uint8_t& _r, const uint8_t& _g, const uint8_t& _b):
		r(_r), g(_g), b(_b)
	  { }

	  Color(const uint8_t& val) :
		r(val), g(val), b(val)
	  { }

	  Color& invert(){
		r = 255 - r;
		g = 255 - g;
		b = 255 - b;
		return *this;
	  }

	  Color& brighten(const float& multiplier){
		r *= multiplier;
		g *= multiplier;
		b *= multiplier;
		return *this;
	  }

	  void set(uint8_t red, uint8_t green, uint8_t blue){
		r = red;
		g = green;
		b = blue;
	  }

	  void set(uint8_t val)
		{ r = g = b = val; }
	};

	// POD digital color class
	class DigitalColor {

	public:
	  // 3-bit color
	  bool r : 1, g : 1, b : 1;

	  DigitalColor(): r(LOW), g(LOW), b(LOW) {}
	  DigitalColor(bool red, bool green, bool blue):
		r(red), g(green), b(blue)
	  { }
	  DigitalColor(bool value):
		r(value), g(value), b(value)
	  { }

	  DigitalColor& invert(){
		r = !r;
		g = !g;
		b = !b;
		return *this;
	  }

	  void set(bool red, bool green, bool blue){
		r = red;
		g = green;
		b = blue;
	  }

	  void set(bool val)
		{ r = g = b = val; }

	};
	typedef DigitalColor Dclr;
	namespace color {

	  inline Color& invert(Color& c){
		c.r = 255 - c.r;
		c.g = 255 - c.g;
		c.b = 255 - c.b;
		return c;
	  }

	  // cycle between 3 values/colors
	  inline void cycle3(uint8_t& v0, uint8_t& v1, uint8_t& v2, uint8_t& curHi){
		if (curHi == 0) {
		  v0--; v1++;
		} else if (curHi == 1) {
		  v1--; v2++;
		} else if (curHi == 2) {
		  v2--; v0++;
		}

		if (v0 <= 0 && curHi == 0)
		  curHi = 1;
		else if (v1 <= 0 && curHi == 1)
		  curHi = 2;
		else if (v2 <= 0 && curHi == 2)
		  curHi = 0;
	  }

	  // reciprocate between 2 values
	  inline void cycle2(uint8_t& v0, uint8_t& v1, bool& curHi){
		if (v0 == 0)
		  curHi = 1;
		else if (v1 == 0)
		  curHi = 0;

		if (curHi == 0) {
		  v0--; v1++;
		} else {
		  v1--; v0++;
		}

	  }

	  // reciprocate between 2 values
	  inline void cycle2(uint8_t& v0, uint8_t& v1, bool& curHi, uint8_t incr){
		while (incr--) {
		  if (v0 == 0)
		    curHi = 1;
		  else if (v1 == 0)
		    curHi = 0;

		  if (curHi == 0) {
		    v0--; v1++;
		  } else {
		    v1--; v0++;
		  }
		}
	  }
	}

	/*	these macro names are too generic...
		commenting these out just in case



	// base colors
	#define COLOR_RED Color(255, 0, 0)
	#define COLOR_YELLOW Color(255, 255, 0)
	#define COLOR_GREEN Color(0, 255, 0)
	#define COLOR_CYAN Color(0, 255, 255)
	#define COLOR_BLUE Color(0, 0, 255)
	#define COLOR_PURPLE Color(255, 0, 255)
	#define COLOR_WHITE Color(255, 255, 255)
	#define COLOR_OFF Color(0, 0, 0)
	#define COLOR_BLACK COLOR_OFF

	// base digital colors
	#define D_COLOR_RED DigitalColor(HIGH, LOW, LOW)
	#define D_COLOR_YELLOW DigitalColor(HIGH, HIGH, LOW)
	#define D_COLOR_GREEN DigitalColor(LOW, HIGH, LOW)
	#define D_COLOR_CYAN DigitalColor(LOW, HIGH, HIGH)
	#define D_COLOR_BLUE DigitalColor(LOW, LOW, HIGH)
	#define D_COLOR_PURPLE DigitalColor(HIGH, LOW, HIGH)
	#define D_COLOR_WHITE DigitalColor(HIGH, HIGH, HIGH)
	#define D_COLOR_OFF DigitalColor(LOW, LOW, LOW)
	#define D_COLOR_BLACK D_COLOR_OFF

	*/
}
#endif
