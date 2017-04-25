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
#include <cinttypes>

#include "Base64/Base64.hpp"
#include "ProgramOptions/ProgramOptions.hpp"
#include "UniversalType/UniversalType.hpp"
#include "SQLAutomator/SQLAutomator.hpp"

using namespace std;
using namespace Reimu;

void Test_Base64(){
	cout << "Testing Base64:\n";
	Base64 encoder;
	string raw = "The  expected  form of the (initial portion of the) string is optional leading white space as recognized by isspace(3), an optio";
	cout << "Raw = " << raw << "\n";
	string enced = encoder.Encode(raw);
	cout << "Encoded = " << enced << "\n";
	std::vector<uint8_t> deced = encoder.Decode(enced);
	printf("Decoded = %s\n", &deced[0]);

	if (memcmp(raw.c_str(), &deced[0], raw.size())) {
		cout << "Test Failed!!!\n";
		abort();
	}
	cout << "Base64 Test OK.\n\n";
}

void Test_ProgramOptions(){
	cout << "Testing ProgramOptions:\n";

	char *myargv[10];

	myargv[0] = "miao";
	myargv[1] = "-c";
	myargv[2] = "/etc/miao.cf";
	myargv[3] = "-v";
	myargv[4] = "--moe";
	myargv[5] = "-a";
	myargv[6] = "OaO QwQ";
	myargv[7] = "--";
	myargv[8] = "喵喵喵";
	myargv[9] = "哒哒哒";

	cout << "Command: ";

	for (int i=0; i<(sizeof(myargv)/sizeof(char *)); i++){
		cout << myargv[i] << " ";
	}

	cout << "\n";

	ProgramOptions po((sizeof(myargv)/sizeof(char *)), (char **)myargv);

	cout << "Option -c = " << po.OptArg("c") << "\n";
	printf("Flag -v found: %s\n", po.Flag("v") ? "true" : "false");
	printf("Flag --moe found: %s\n", po.Flag("moe") ? "true" : "false");
	printf("Flag -x found: %s\n", po.Flag("x") ? "true" : "false");
	cout << "Option -a = " << po.OptArg("a") << "\n";

	cout << "Trailing:\n";

	for (auto &tt : po.Trailing)
		cout << tt << "\n";

	cout << "ProgramOptions Test OK.\n";
}

void Test_UniversalType(){
	cout << "Testing UniversalType:\n";

	int64_t myd64;
	void *myptr;

	printf("myut_u64 orig = %" PRIu16 "\n", 65535);
	UniversalType myut_u16((uint16_t)65535);

	printf("myut_u16 type = %d\n", myut_u16.Type);


	myd64 = myut_u16;

	printf("myut_u16 u64 = %" PRIu64 "\n", myd64);
	printf("myut_u16 s64 = %" PRId64 "\n", myd64);
	printf("myut_u16 auto to string = %s\n", myut_u16.operator std::string().c_str());

	puts("");


	printf("myut_u64 orig = %" PRIu64 "\n", 18446744073709551615UL);
	UniversalType myut_u64(18446744073709551615UL);

	printf("myut_u64 type = %d\n", myut_u64.Type);

	myd64 = myut_u64;

	printf("myut_u64 u64 = %" PRIu64 "\n", myd64);
	printf("myut_u64 s64 = %" PRId64 "\n", myd64);
	printf("myut_u64 auto to string = %s\n", myut_u64.operator std::string().c_str());

	puts("");

	printf("myut_s64 orig = %" PRId64 "\n", -9223372036854775807L);
	UniversalType myut_s64(-9223372036854775807L);

	printf("myut_s64 type = %d\n", myut_s64.Type);

	myd64 = myut_s64;

	printf("myut_s64 u64 = %" PRIu64 "\n", myd64);
	printf("myut_s64 s64 = %" PRId64 "\n", myd64);
	printf("myut_s64 auto to string = %s\n", myut_s64.operator std::string().c_str());

	puts("");

	void *someptr = malloc(0);

	printf("myut_ptr orig = %p\n", someptr);
	UniversalType myut_ptr(someptr);

	printf("myut_ptr type = %d\n", myut_ptr.Type);

	myptr = myut_ptr;

	printf("myut_ptr ptr = %p\n", myptr);
	printf("myut_ptr auto to string = %s\n", myut_ptr.operator std::string().c_str());

	cout << "UniversalType Test OK.\n\n";

	puts("");

	string somestr = "18446744073709551615";

	printf("myut_stru64max orig = \"%s\"\n", somestr.c_str());
	UniversalType myut_stru64max(somestr);

	printf("myut_stru64max type = %d\n", myut_stru64max.Type);

	myd64 = myut_stru64max;

	printf("myut_stru64max u64 = %" PRIu64 "\n", myd64);
	printf("myut_stru64max auto to string = %s\n", myut_stru64max.operator std::string().c_str());

	cout << "UniversalType Test OK.\n\n";
}

SQLAutomator::SQLite3 *SQLite3wrapper(){
	SQLAutomator::SQLite3 *ret = new SQLAutomator::SQLite3(":memory:");

	return ret;
}

void Test_SQLite3(){
	SQLAutomator::SQLite3 s3 = *SQLite3wrapper();

	cerr << "Got SQlite3 ptr!!\n";
}

int main(){
	Test_Base64();
	Test_UniversalType();
	Test_ProgramOptions();
	Test_SQLite3();
}