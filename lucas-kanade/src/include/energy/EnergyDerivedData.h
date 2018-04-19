#ifndef __LUCAS_KANADE_ENERGY_DERIVED_DATA__
#define __LUCAS_KANADE_ENERGY_DERIVED_DATA__

#include <vector>

#include <armadillo>

#include "matrix/Transformation.h"
#include "ZeroNormalizedCrossCorrelation.h"

namespace lucasKanade{

  class EnergyDerivedData{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyDerivedData(
                      const Image& target,
                      const std::vector<arma::vec>& originalLocations) :

      deformedTemplate(target, originalLocations),
      incrementallyDeformedTemplate(target, originalLocations, deformedTemplate) {

    }

    /*--------------------------------------------------------------------------*/

  };
}
#endif
