#ifndef __BRIGHTNESS_CONSTANCY_H__
#define __BRIGHTNESS_CONSTANCY_H__

namespace lucasKanade{

  class BrightnessConstancy{

  private:

    // derived data
    int voxelAmount;

    int transformationAmount;

    const std::vector<double>& originalValues;
    std::vector<double> deformedValues;
    const IncrementallyDeformedTemplate& incrementallyDeformedTemplate;

    // output
    double energy;

    arma::vec gradient;

  public:

    BrightnessConstancy(
                       const std::vector<double>& originalValues,
                       const DeformedTemplate& deformedTemplate,
                       const IncrementallyDeformedTemplate& incrementallyDeformedTemplate
                       ) :
      originalValues(originalValues),
      incrementallyDeformedTemplate(incrementallyDeformedTemplate) {

      this->voxelAmount = deformedTemplate.get_deformed_template().size();
      this->transformationAmount = 6;

      this->deformedValues =
        this->incrementallyDeformedTemplate.get_incrementally_deformed_template();

      compute_energy();

      compute_derivatives();

    }

    double get_energy() const{

      return this->energy;

    }

    arma::vec get_gradient() const {

      return this->gradient;

    }

  private:

    void compute_energy() {

      this->energy = 0;

      for(int i = 0; i < this->voxelAmount; ++i) {

        this->energy += pow(this->deformedValues[i] - this->originalValues[i], 2);

      }

    }

    void compute_derivatives() {

      arma::mat derivative = arma::zeros(this->transformationAmount).t();

      const std::vector<arma::mat>& imageGradientTimesJacobian =
        this->incrementallyDeformedTemplate.get_image_gradient_times_jacobian();

      for(int i = 0; i < this->voxelAmount; ++i) {

        derivative += imageGradientTimesJacobian[i] * ( this->deformedValues[i] - this->originalValues[i] );

      }

      this->gradient = derivative;

    }


  };

}

#endif
