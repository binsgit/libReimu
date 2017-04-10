/*
    This file is part of libReimu.
    Copyright (C) 2016-2017  CloudyReimu <cloudyreimu@gmail.com>

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

    Tests.cpp - Unit testing.
*/

#include <iostream>

#include "Base64/Base64.hpp"

using namespace std;
using namespace Reimu;

void Test_Base64(){
	cout << "Testing Base64:\n";
	Base64 encoder;
	string raw = "我就眼睁睁地看着一个个新人小妹妹被这群乌合之众蛊惑把心态弄得差到不行。妻管严…女人的事情，能叫妻管严么！知乎上充斥着快活的空气。";
	cout << "Raw = " << raw << "\n";
	string enced = encoder.Encode(raw);
	cout << "Encoded = " << enced << "\n";
	std::vector<uint8_t> deced = encoder.Decode(enced);
	printf("Decoded = %s\n", &deced[0]);

	if (memcmp(raw.c_str(), &deced[0], raw.size())) {
		cout << "Test Failed!!!\n";
		abort();
	}
	cout << "Base64 Test OK.\n";
}

int main(){
	Test_Base64();

}