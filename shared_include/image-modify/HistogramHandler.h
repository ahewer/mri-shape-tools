#ifndef __HISTOGRAM_HANDLER_H__
#define __HISTOGRAM_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class HistogramHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];
    const std::string type = options["type"].asString();

    if      (type == "equalize") { equalize(image); }
    else if (type == "local enhancement") { local_enhancement(image, options); }

  }

  void equalize(Image& image) {

    image.histogram().equalize();

  }

  void local_enhancement(Image& image, Json::Value& options) {

    const int& radius = options["radius"].asInt();
    const double& multiplier = options["multiplier"].asDouble();
    const double& lowerBound = options["lowerBound"].asDouble();
    const double& upperBound = options["upperBound"].asDouble();

    image.histogram().local_enhancement(radius, multiplier, lowerBound, upperBound);

  }

};
#endif
