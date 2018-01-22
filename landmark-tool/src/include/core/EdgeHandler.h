#ifndef __EDGE_HANDLER_H__
#define __EDGE_HANDLER_H__

#include <json/json.h>

#include "Handler.h"
#include "image/Image.h"

class EdgeHandler : public Handler {

public:

  virtual void handle(Image& image, Json::Value& modification) {

    Json::Value& options = modification["options"];

    const std::string type = options["type"].asString();

    if      (type == "laplacian zero crossings") { laplacian_zero_crossings(image); }
    else if (type == "gradient magnitude") { gradient_magnitude(image); }

  }

  void laplacian_zero_crossings(Image& image) {

    image.edge().laplacian_zero_crossings();

  }

  void gradient_magnitude(Image& image) {

    image.edge().gradient_magnitude();

  }


};
#endif
