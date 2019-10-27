#ifndef __IMAGE_MASKED_ACCESS_H__
#define __IMAGE_MASKED_ACCESS_H__

#include "image/ImageAccess.h"

class ImageMaskedAccess: public ImageAccess{

public:

  ImageMaskedAccess(ImageData& imageData, ImageData imageMask): ImageAccess(imageData), imageMask(imageMask), copyData(imageData) {
    // TODO: check consistency between imageData and imageMask
  }

  virtual double& at_grid(
                          const int& x,
                          const int& y,
                          const int& z) {

    const int& bx = this->imageData.bx;
    const int& by = this->imageData.by;
    const int& bz = this->imageData.bz;

    int index = ImageAccess::compute_index(bx + x, by + y, bz + z);

    if(voxel_is_masked(index)) {
      // a masked voxel can not be changed
      return reference_to_copy_of_original_value(index);
    }

    return this->imageData.values.at(index);

  }

private:

  bool voxel_is_masked(const int& index) {
    return this->imageMask.values.at(index) != 0;
  }

  double& reference_to_copy_of_original_value(const int& index) {
    this->copyData.values.at(index) = this->imageData.values.at(index);
    return this->copyData.values.at(index);
  }

  // no reference
  ImageData imageMask;
  ImageData copyData;

};

#endif
