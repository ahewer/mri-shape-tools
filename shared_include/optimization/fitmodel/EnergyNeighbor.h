#ifndef __FIT_MODEL_ENERGY_NEIGHBOR_H__
#define __FIT_MODEL_ENERGY_NEIGHBOR_H__

#include "neighborsearch/NeighborSearch.h"

#include "optimization/fitmodel/EnergyData.h"
#include "optimization/fitmodel/EnergyDerivedData.h"
#include "optimization/fitmodel/EnergySettings.h"

namespace fitModel{

  /* class for finding neighbors of the source vertices in the target vertices */
  class EnergyNeighbor{

  public:

    /*--------------------------------------------------------------------------*/

    EnergyNeighbor(
      EnergyData& energyData,
      EnergyDerivedData& energyDerivedData,
      EnergySettings& energySettings)
      : energyData(energyData),
        energyDerivedData(energyDerivedData),
        energySettings(energySettings) {


      if(this->energySettings.useTargetAsSource == false) {

        // set target and construct kd-tree if present
        if(this->energyData.target.get_vertices().size() > 0) {
          this->neighborSearch.set_target(this->energyData.target);
        }

      }
      else {

        // special case: only set source
        this->neighborSearch.set_source(this->energyData.target);

      }

      this->areFixed = false;

      // configure search strategy
      this->neighborSearch.set_max_angle(this->energySettings.maxAngle);
      this->neighborSearch.set_max_distance(this->energySettings.maxDistance);

      // select wanted search strategy
      switch(this->energySettings.searchStrategy) {

      case EnergySettings::SearchStrategy::BASIC:
        this->searchStrategy = &(this->neighborSearch.basic());
        this->needNormals = false;
        break;

      case EnergySettings::SearchStrategy::NORMAL_PLANE:
        this->searchStrategy = &(this->neighborSearch.normal_plane());
        this->needNormals = true;
        break;

      case EnergySettings::SearchStrategy::ADAPTIVE:
        this->searchStrategy = &(this->neighborSearch.adaptive());
        this->neighborSearch.set_search_radius(this->energySettings.searchRadius);
        this->needNormals = true;
        break;

      case EnergySettings::SearchStrategy::FIXED:
        this->searchStrategy = &(this->neighborSearch.fixed_correspondences());
        this->needNormals = false;
        this->areFixed = true;
        break;

      case EnergySettings::SearchStrategy::PRESET:
        this->searchStrategy = &(this->neighborSearch.preset_correspondences());
        this->neighborSearch.set_preset_source(this->energySettings.presetSource);
        this->neighborSearch.set_preset_target(this->energySettings.presetTarget);
        this->needNormals = false;
        this->areFixed = true;
        break;

      default:
        this->searchStrategy = &(this->neighborSearch.basic());
        this->needNormals = false;
        break;
      } // end switch

    }

    /*--------------------------------------------------------------------------*/

    /* compute neighbors according to chosen search strategy */
    void compute() {

      std::vector<int> sourceIndices;
      std::vector<int> targetIndices;

      this->energyDerivedData.sourceIndices.clear();
      this->energyDerivedData.targetIndices.clear();

      if(this->energySettings.useTargetAsSource == false) {

        // update source
        this->neighborSearch.set_source(this->energyDerivedData.source);

        // find neighbors
        this->searchStrategy->find_neighbors(
          sourceIndices,
          targetIndices
          );

      } // end if
      else {

        // special case: rebuild kd-tree, current state of model
        // is used as target
        this->neighborSearch.set_target(this->energyDerivedData.source);

        // important: swap roles of target and source
        this->searchStrategy->find_neighbors(
          targetIndices,
          sourceIndices
          );

      } // end else

      // remove indices that belong to landmarks
      for(size_t i = 0; i < sourceIndices.size(); ++i) {

        this->energyDerivedData.sourceIndices.push_back(sourceIndices.at(i));
        this->energyDerivedData.targetIndices.push_back(targetIndices.at(i));

      }

    }

    /*--------------------------------------------------------------------------*/

    /* helper function for updating the search structure if the target mesh
     * was changed
     */
    void update_for_target() {
      this->neighborSearch.set_target(this->energyData.target, ! this->areFixed);
    }

    /*--------------------------------------------------------------------------*/

    bool need_normals() const {
      return this->needNormals;
    }

    /*--------------------------------------------------------------------------*/

    bool are_fixed() const {
      return this->areFixed;
    }

    /*--------------------------------------------------------------------------*/

  private:

    /*--------------------------------------------------------------------------*/

    const SearchProto* searchStrategy;

    /*--------------------------------------------------------------------------*/

    bool areFixed;
    bool needNormals;
    EnergyData& energyData;
    EnergyDerivedData& energyDerivedData;
    EnergySettings& energySettings;
    NeighborSearch neighborSearch;

    /*--------------------------------------------------------------------------*/
  };

}
#endif
