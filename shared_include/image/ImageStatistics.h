#ifndef __IMAGE_STATISTICS_H__
#define __IMAGE_STATISTICS_H__

#include <algorithm>
#include <cmath>

#include "image/ImageAccess.h"
#include "image/ImageData.h"

class ImageStatistics{

public:

  ImageStatistics(ImageData& imageData) :
    imageData(imageData),
    imageAccess(imageData) {
  }

  double percentile(const double& percentage) const {

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    std::vector<double> values;

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {

          values.push_back(this->imageAccess.at_grid(i, j ,k));

        }
      }
    }

    std::sort(values.begin(), values.end());

    const int index = ( percentage / 100 ) * values.size() - 1;

    return values.at(index);

  }

  double mean() const {

    double result = 0;

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {

          const double& value = this->imageAccess.at_grid(i, j, k);

          result += value;

        }

      }

    }

    return value / ( nx * ny * nz );

  }

  double standard_deviation() const {

    const double imageMean = mean();

    double result = 0;

    const int& nx = this->imageData.nx;
    const int& ny = this->imageData.ny;
    const int& nz = this->imageData.nz;

    for(int i = 0; i < nx; ++i) {
      for(int j = 0; j < ny; ++j) {
        for(int k = 0; k < nz; ++k) {

          const double& value = this->imageAccess.at_grid(i, j, k);

          result += pow(value - imageMean, 2);

        }

      }

    }

    return sqrt( result / ( nx * ny * nz ) );

  }

private:

  ImageData& imageData;
  const ImageAccess imageAccess;

};

#endif
