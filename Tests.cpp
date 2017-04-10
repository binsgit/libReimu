//
// Created by root on 17-4-10.
//

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