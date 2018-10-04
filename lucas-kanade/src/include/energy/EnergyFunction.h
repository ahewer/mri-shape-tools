#ifndef __LUCAS_KANADE_ENERGY_FUNCTION_H__
#define __LUCAS_KANADE_ENERGY_FUNCTION_H__

#include <vnl/vnl_vector.h>
#include <vnl/vnl_cost_function.h>

#include "energy/EnergyData.h"
#include "ZeroNormalizedCrossCorrelation.h"
#include "BrightnessConstancy.h"

#include "PenalizerTypeEnum.h"

namespace lucasKanade{

  // class used for minimizing the energy with ITK
  class EnergyFunction: public vnl_cost_function{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyFunction(
                   Energy& energy
                   ) :
      vnl_cost_function(energy.data().transformationAmount),
      energy(energy) {

    }

    /*--------------------------------------------------------------------------*/

    virtual void compute(
                         const vnl_vector<double>&, double *f, vnl_vector<double>* g){

      // make sure that energy and gradient are reset to 0
      *f = 0;

      for(int i = 0; i < this->energy.data().transformationAmount; ++i) {
        (*g)[i] = 0;
      }

      add_energy_and_gradient(*f, *g);

    }

    /*--------------------------------------------------------------------------*/

  private:

    void add_energy_and_gradient(
                                 double& energy,
                                 vnl_vector<double>& gradient) const {

      const int& transformationAmount = this->energy.data().transformationAmount;


      IncrementallyDeformedTemplate& incrementallyDeformedTemplate =
        this->energy.data().incrementallyDeformedTemplate;

      const arma::vec increment( {
          this->energy.data().transformationIncrement[TX],
            this->energy.data().transformationIncrement[TY],
            this->energy.data().transformationIncrement[TZ],
            this->energy.data().transformationIncrement[ALPHA],
            this->energy.data().transformationIncrement[BETA],
            this->energy.data().transformationIncrement[GAMMA]
            }
        );

      incrementallyDeformedTemplate.compute(increment);

      arma::vec localGradient;

      switch(this->energy.settings().penalizerType) {

      case BRIGHTNESS:

        {

          BrightnessConstancy constancy(
                                        this->energy.data().originalTemplate.get_values(),
                                        this->energy.data().deformedTemplate,
                                        incrementallyDeformedTemplate);

          energy = constancy.get_energy();

          localGradient = constancy.get_gradient();

        }

        break;

      default:

        {
          ZeroNormalizedCrossCorrelation zeroNormalizedCrossCorrelation(
                                                                        this->energy.data().originalTemplate.get_normalized_values(),
                                                                        this->energy.data().deformedTemplate,
                                                                        incrementallyDeformedTemplate
                                                                        );

          energy = - zeroNormalizedCrossCorrelation.get_correlation();

          localGradient = - zeroNormalizedCrossCorrelation.get_correlation_derivative();

        }

        break;

      }

      for(int j = 0; j < transformationAmount; ++j) {

        gradient[j] = localGradient(j);

      }

    }

    /*--------------------------------------------------------------------------*/

    Energy& energy;

    /*--------------------------------------------------------------------------*/

  };

}
#endif
