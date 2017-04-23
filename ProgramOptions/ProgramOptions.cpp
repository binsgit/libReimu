//
// Created by root on 17-4-23.
//

#include "ProgramOptions.hpp"

#define ITEM_STATE_WAITNEXT	0
#define ITEM_STATE_OPT		1
#define ITEM_STATE_ARG		2
#define ITEM_STATE_TRAILING	3

Reimu::ProgramOptions::ProgramOptions(int argc, char **argv) {

	int state = ITEM_STATE_WAITNEXT;
	int argj = 1;
	size_t thisoptlen;
	char *s_st, *s_ed;
	std::string thisopt;

	while ( argj < argc ) {
		if (state == ITEM_STATE_TRAILING) {
			Trailing.push_back(std::string(argv[argj]));
			argj++;
			continue;
		}

		thisoptlen = strlen(argv[argj]);
		if (thisoptlen > 1) {
			if (argv[argj][0] == '-') {
				if (argv[argj][1] == '-') {
					if (thisoptlen > 2) {
						s_st = argv[argj] + 2;
						if (state == ITEM_STATE_OPT) {
							Flags.insert(thisopt);
							thisopt.clear();
						}
						thisopt += s_st;
						state = ITEM_STATE_OPT;
					} else {
						state = ITEM_STATE_TRAILING;
						argj++;
						continue;
					}
				} else {
					s_st = argv[argj] + 1;
					if (state == ITEM_STATE_OPT) {
						Flags.insert(thisopt);
						thisopt.clear();
					}
					thisopt += s_st;
					state = ITEM_STATE_OPT;
				}
			} else {
				if (state == ITEM_STATE_OPT) {
					OptionMapping[thisopt] = argv[argj];
					thisopt.clear();
				} else
					Trailing.push_back(std::string(argv[argj]));

				state = ITEM_STATE_WAITNEXT;
			}

		} else {
			if (state == ITEM_STATE_OPT) {
				OptionMapping[thisopt] = argv[argj];
				thisopt.clear();
			} else
				Trailing.push_back(std::string(argv[argj]));

			state = ITEM_STATE_WAITNEXT;
		}

		argj++;
	}

	if (state == ITEM_STATE_OPT)
		Flags.insert(thisopt);


}

bool Reimu::ProgramOptions::Flag(std::string f) {
	auto tgt = Flags.find(f);

	return tgt != Flags.end();
}

char *Reimu::ProgramOptions::OptArg(std::string f) {
	auto tgt = OptionMapping.find(f);

	if (tgt == OptionMapping.end())
		return NULL;
	else
		return tgt->second;
}
