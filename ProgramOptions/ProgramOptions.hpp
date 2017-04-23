//
// Created by root on 17-4-23.
//

#ifndef LIBREIMU_PROGRAMOPTIONS_HPP
#define LIBREIMU_PROGRAMOPTIONS_HPP


#include "../libReimu.hpp"

namespace Reimu {
    class ProgramOptions {
    public:
	std::map<std::string, char *> OptionMapping;
	std::set<std::string> Flags;
	std::vector<std::string> Trailing;

	ProgramOptions(int argc, char **argv);

	bool Flag(std::string f);
	char *OptArg(std::string f);
    };
}

#endif //LIBREIMU_PROGRAMOPTIONS_HPP
