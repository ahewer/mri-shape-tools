#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "flags/FlagSingle.h"
#include "flags/FlagList.h"
#include "flags/FlagsParser.h"

#include <string>

class Settings {

public:

  // input and output
  std::string input;
  std::string output;

  std::string coil;
  std::vector<double> timeStamps;

  Settings(int argc, char* argv[]) {

    FlagSingle<std::string> inputFlag("input", this->input);
    FlagSingle<std::string> outputFlag("output", this->output);
    FlagSingle<std::string> coilFlag("coil", this->coil);
    FlagList<double> timeStampsFlag("timeStamps", this->timeStamps);

    FlagsParser parser(argv[0]);
    parser.define_flag(&inputFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&coilFlag);
    parser.define_flag(&timeStampsFlag);

    parser.parse_from_command_line(argc, argv);
  }

};

#endif
