#ifndef __LUCAS_KANADE_ENERGY_SETTINGS_H__
#define __LUCAS_KANADE_ENERGY_SETTINGS_H__

#include "PenalizerTypeEnum.h"

namespace lucasKanade{

  class EnergySettings{

  public:

    EnergySettings() {

      this->epsilon = 0.00001;

      this->penalizerType = ZNCC;

    }

    double epsilon;

    int penalizerType;

  };

}

#endif
