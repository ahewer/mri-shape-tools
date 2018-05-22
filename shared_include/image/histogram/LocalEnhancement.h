#ifndef __HISTOGRAM_LOCAL_ENHANCEMENT_H__
#define __HISTOGRAM_LOCAL_ENHANCEMENT_H__

class HistogramLocalEnhancement{

private:

  ImageData& imageData;
  ImageAccess imageAccess;

  const int& radius;
  const double& multiplier;
  const double& lowerBound;
  const double& upperBound;

public:

  HistogramLocalEnhancement(
                            ImageData& imageData,
                            const int& radius,
                            const double& multiplier,
                            const double& lowerBound,
                            const double& upperBound
                            ) :
    imageData(imageData),
    imageAccess(imageData),
    radius(radius),
    multiplier(multiplier),
    lowerBound(lowerBound),
    upperBound(upperBound) {

    ImageStatistics statistics(this->imageData);

    this->globalMean = statistics.mean();
    this->globalStandardDeviation = statistics.standard_deviation();

  }

  void apply() {

    ImageData copy = this->imageData;
    ImageBoundary(this->imageData).change(this->radius, this->radius, this->radius);
    ImageMirror(imageData).all();
    ImageAccess access(copy);

    for(int i = 0; i < copy.nx; ++i) {
      for(int j = 0; j < copy.ny; ++j) {
        for(int k = 0; k < copy.nz; ++k) {

          double localMean, localStandardDeviation;

          compute_local_statistics(i, j, k, localMean, localStandardDeviation);

          if( (localMean < this->globalMean) &&
              (lowerBound * this->globalStandardDeviation <= this->localStandardDeviation ) &&
              (localStandardDeviation <= this->upperBound * this->globalStandardDeviation ) ) {

            access.at_grid(i, j, k) = this->multiplier * access.at_grid(i, j, k);

          } // end if

        } // end for k

      } // end for j

    } // end for i

    this->imageData = copy;

  }

private:

  void compute_local_statistics(
                                // INPUT
                                const int& x,
                                const int& y,
                                const int& z,
                                // OUTPUT
                                double& mean,
                                double& standardDeviation) const {

    mean = 0;

    for(int i = -this->radius; i <= this->radius; ++i) {
      for(int j = -this->radius; j <= this->radius; ++j) {
        for(int k = -this->radius; k <= this->radius; ++k) {

          mean += this->imageAccess.at_grid(x + i, y + j, z + k);

        } // end k

      } // end j

    } // end i

    mean /= this->imageData.size();

    standardDeviation = 0;

    for(int i = -this->radius; i <= this->radius; ++i) {
      for(int j = -this->radius; j <= this->radius; ++j) {
        for(int k = -this->radius; k <= this->radius; ++k) {

          standardDeviation += pow(this->imageAccess.at_grid(x + i, y + j, z + k) - mean, 2);

        } // end k

      } // end j

    } // end i

    standardDeviation = sqrt(standardDeviation / this->imageData.values.size());
  }

};
#endif
