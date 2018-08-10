#ifndef __ROTATE_HANDLER_H__
#define __ROTATE_HANDLER_H__

#include "mesh/Mesh.h"
#include "matrix/Transformation.h"

#include "Handler.h"

class RotateHandler : public Handler {

public:

  virtual void handle(Mesh& mesh, Json::Value& modification) {

    Json::Value& options = modification["options"];

    // read axis
    const std::string axis = options["axis"].asString();

    // read angle
    double angle = options["angle"].asDouble();

    // convert angle to radians if needed
    if( options["angleUnit"].empty() == false) {

      if( options["angleUnit"].asString() == "degrees") {

        angle = angle / 180 * M_PI;

      }

    }

    // construct transformation depending on chosen rotation
    // axis
    Transformation transform;

    if( axis == "x" ) {

      transform = Transformation(angle, 0, 0);

    }
    else if ( axis == "y" ) {

      transform = Transformation(0, angle, 0);

    }
    else if( axis == "z" ) {

      transform = Transformation(0, 0, angle);

    }

    // apply transformation to all vertices
    for( arma::vec& vertex: mesh.get_vertices() ) {

      vertex = transform.apply_matrix(vertex);

    }

  }

};

#endif
