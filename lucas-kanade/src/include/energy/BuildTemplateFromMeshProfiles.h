#ifndef __BUILD_TEMPLATE_FROM_MESH_PROFILES_H__
#define __BUILD_TEMPLATE_FROM_MESH_PROFILES_H__

#include <vector>
#include <algorithm>

#include <armadillo>

#include "mesh/Mesh.h"
#include "mesh/NormalEstimation.h"
#include "matrix/Transformation.h"

namespace lucasKanade{

  class BuildTemplateFromMeshProfiles{

  private:

    const Mesh mesh;
    const Image image;
    const double offset;
    const double length;

    Transformation matrix;

    std::vector<arma::vec> locations;

  public:

    /*-------------------------------------------------------------------------*/

    BuildTemplateFromMeshProfiles(
                                  const Mesh& mesh,
                                  const Image& image,
                                  const double& offset,
                                  const double& length
                                  ) :
      mesh(mesh), image(image), offset(offset), length(length) {

      estimate_normals();

      compute_profile_locations();

      compute_rotation_center();

    }

    /*-------------------------------------------------------------------------*/

    const std::vector<arma::vec>& get_locations() const {

      if( this->isBuilt == false) {

        throw std::logic_error("Template information was not built.");

      }

      return this->locations;

    }

    /*-------------------------------------------------------------------------*/

    const Transformation& get_transformation() const {

      if( this->isBuilt == false) {

        throw std::logic_error("Template information was not built.");

      }

      return this->transformation;

    }

    /*-------------------------------------------------------------------------*/

  private:

    /*-------------------------------------------------------------------------*/

    void estimate_normals() {

      this->mesh.set_vertex_normals( NormalEstimation(this->mesh).compute() );

    }

    /*-------------------------------------------------------------------------*/

    void compute_profile_locations() {

      this->locations.clear();

      for( size_t i = 0; i < mesh.get_vertices().size(); ++i ) {

        compute_locations_for_vertex(i);

      }

      std::unique( this->locations.begin(), this->locations.end() );

    }

    /*-------------------------------------------------------------------------*/

    void compute_rotation_center() {

      this->matrix.set_origin( this->mesh.get_center() );

    }

    /*-------------------------------------------------------------------------*/

    void compute_locations_for_vertex(size_t index) {

      const arma::vec& vertex = this->mesh.get_vertices().at(index);
      const arma::vec& normal = this->mesh.get_vertex_normals().at(index);

      for(size_t i = 0; i < this->length; ++i) {

        const arma::vec location = vertex + ( this->offset + i ) * normal;

        if( is_inside_domain(location) == true ) {

          this->locations.push_back(location);

        }

      }

    }

    /*-------------------------------------------------------------------------*/

    bool is_inside_domain(const arma::vec& location) const {

      const int& nx = this->image.info().get_nx();
      const int& ny = this->image.info().get_ny();
      const int& nz = this->image.info().get_nz();

      const double& hx = this->image.info().get_hx();
      const double& hy = this->image.info().get_hy();
      const double& hz = this->image.info().get_hz();


      return

        ( location(0) >= 0 ) && ( location(0) < nx * hx ) &&
        ( location(1) >= 0 ) && ( location(1) < ny * hy ) &&
        ( location(2) >= 0 ) && ( location(2) < nz * hz );

    }

    /*-------------------------------------------------------------------------*/

  };

}
#endif
