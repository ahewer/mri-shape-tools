#include <string>
#include <vector>
#include <stdexcept>

#include "settings.h"
#include "ProcessScan.h"
#include "ImageStack.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  ProcessScan process(settings.input, settings.discard);

  auto stack = process.get_image_stack();

  Cairo::RefPtr<Cairo::ImageSurface> image;

  if( (
        settings.xyOutput ||
        settings.xzOutput ||
        settings.yzOutput ) == false ) {

    throw std::runtime_error("No slices given for output.");
  }

  // prepare to output all slices if wanted
  if( settings.all == true ) {

    settings.xySlices.clear();
    settings.xzSlices.clear();
    settings.yzSlices.clear();

    for(size_t i = 0; i < stack->get_xy_slices().size(); ++i) {

      settings.xySlices.push_back(i);

    }

    for(size_t i = 0; i < stack->get_zx_slices().size(); ++i) {

      settings.xzSlices.push_back(i);

    }

    for(size_t i = 0; i < stack->get_zy_slices().size(); ++i) {

      settings.yzSlices.push_back(i);

    }

  }

  // output xy slices if wanted
  if( settings.xyOutput == true ) {

    const unsigned int digits = std::to_string(settings.xySlices.size()).size();

    for( size_t i = 0; i < settings.xySlices.size(); ++i) {

      const int sliceIndex = settings.xySlices.at(i);

      image = stack->get_xy_slices().at(sliceIndex);

      std::string outputName = settings.baseName + "_xy_";

      const unsigned int freeDigits = digits - std::to_string(i).size();
      outputName.insert(outputName.end(), freeDigits, '0');

      outputName += std::to_string(i) + ".png";

      image->write_to_png(outputName);
    }
  }

  // output xz slices if wanted
  if( settings.xzOutput == true ) {

    const unsigned int digits = std::to_string(settings.xzSlices.size()).size();

    for( size_t i = 0; i < settings.xzSlices.size(); ++i) {

      const int sliceIndex = settings.xzSlices.at(i);

      image = stack->get_zx_slices().at(sliceIndex);

      std::string outputName = settings.baseName + "_xz_";

      const unsigned int freeDigits = digits - std::to_string(i).size();
      outputName.insert(outputName.end(), freeDigits, '0');

      outputName += std::to_string(i) + ".png";

      image->write_to_png(outputName);
    }
  }

  // output yz slices if wanted
  if( settings.yzOutput == true ) {

    const unsigned int digits = std::to_string(settings.yzSlices.size()).size();

    for( size_t i = 0; i < settings.yzSlices.size(); ++i) {

      const int sliceIndex = settings.yzSlices.at(i);

      image = stack->get_zy_slices().at(sliceIndex);

      std::string outputName = settings.baseName + "_yz_";

      const unsigned int freeDigits = digits - std::to_string(i).size();

      outputName.insert(outputName.end(), freeDigits, '0');

      outputName += std::to_string(i) + ".png";

      image->write_to_png(outputName);
    }
  }


  return 0;

}
