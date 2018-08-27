#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <algorithm>
#include <map>
#include <string>

#include "flags/FlagSingle.h"
#include "flags/FlagNone.h"
#include "flags/FlagList.h"
#include "flags/FlagsParser.h"
#include "optimization/fitmodel/MinimizerSettings.h"
#include "optimization/fitmodel/EnergySettings.h"

class Settings {

public:

  // input and output
  std::string model;
  std::string inputFileName;
  std::string outputFileName;

  std::string fixedSpeakerWeights = "";

  std::string subset;

  bool fixSpeaker = false;

  std::string meshModifications;

  fitModel::MinimizerSettings minimizerSettings;
  fitModel::EnergySettings energySettings;

  double speakerWeight = 1;
  double phonemeWeight = 1;

  double meanBiasWeight = 0;

  Settings(int argc, char* argv[]) {

    // input and output
    FlagSingle<std::string> inputFlag("input", this->inputFileName);
    FlagSingle<std::string> modelFlag("model", this->model);
    FlagSingle<std::string> outputFlag("output", this->outputFileName);
    FlagSingle<std::string> fixedSpeakerWeightsFlag("fixedSpeakerWeights", this->fixedSpeakerWeights, true);
    FlagSingle<std::string> subsetFlag("subset", this->subset);

    // mesh modifications
    FlagSingle<std::string> meshModificationsFlag("meshModifications", this->meshModifications);

    // smoothness for speaker and phoneme mode
    FlagSingle<double> speakerWeightFlag("speakerWeight", this->speakerWeight, true);
    FlagSingle<double> phonemeWeightFlag("phonemeWeight", this->phonemeWeight, true);
    FlagSingle<double> meanBiasWeightFlag("meanBiasWeight", this->meanBiasWeight, true);

    /////////////////////////////////////////////////////////////////////////

    // minimizer settings
    FlagSingle<double> priorSizeFlag(
      "priorSize", this->minimizerSettings.priorSize, true);

    FlagSingle<int> convergenceFactorFlag(
      "convergenceFactor",
      this->minimizerSettings.convergenceFactor, true);

    FlagSingle<double> projectedGradientToleranceFlag(
      "projectedGradientTolerance",
      this->minimizerSettings.projectedGradientTolerance, true);

    FlagSingle<int> maxFunctionEvalsFlag(
      "maxFunctionEvals",
      this->minimizerSettings.maxFunctionEvals, true);

    FlagSingle<int> frameIterationAmountFlag(
                                         "frameIterationAmount",
                                         this->minimizerSettings.iterationAmount, true);


    /////////////////////////////////////////////////////////////////////////


    FlagsParser parser(argv[0]);

    // input and output
    parser.define_flag(&inputFlag);
    parser.define_flag(&modelFlag);
    parser.define_flag(&outputFlag);
    parser.define_flag(&fixedSpeakerWeightsFlag);
    parser.define_flag(&subsetFlag);

    // mesh modifications
    parser.define_flag(&meshModificationsFlag);

    // smoothness for speaker and phoneme mode
    parser.define_flag(&speakerWeightFlag);
    parser.define_flag(&phonemeWeightFlag);

    parser.define_flag(&meanBiasWeightFlag);

    // minimizer settings
    parser.define_flag(&priorSizeFlag);
    parser.define_flag(&convergenceFactorFlag);
    parser.define_flag(&projectedGradientToleranceFlag);
    parser.define_flag(&maxFunctionEvalsFlag);
    parser.define_flag(&frameIterationAmountFlag);

    parser.parse_from_command_line(argc, argv);

    // use fixed correspondences
    this->energySettings.searchStrategy =
      fitModel::EnergySettings::SearchStrategy::BASIC;

    // set weights
    this->energySettings.weights["speakerSmoothnessTerm"] = speakerWeight;
    this->energySettings.weights["phonemeSmoothnessTerm"] = phonemeWeight;
    this->energySettings.weights["meanBiasTerm"] = meanBiasWeight;

    this->fixSpeaker = fixedSpeakerWeightsFlag.is_present();

  }

};
#endif
