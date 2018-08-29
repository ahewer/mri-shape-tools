#include <fstream>
#include <json/json.h>
#include <stdexcept>


#include "image/Image.h"
#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "model/ModelReader.h"
#include "utility/CreatePointCloud.h"
#include "mesh-modify/ApplyModifications.h"

#include "settings.h"
#include "Tracker.h"
#include "Subset.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image image;

  image.read().from(settings.inputFileName);

  ModelReader modelReader(settings.model);
  Model model = modelReader.get_model();

  // only consider subset of vertices
  std::set<int> subset = Subset::read(settings.subset);
  model.truncate().vertex(subset);

  TrackerData trackerData(model, settings);

  Tracker tracker(trackerData);

  DiffusionSettings diffusionSettings;

  const int& nx = image.info().get_nx();
  const int& ny = image.info().get_ny();
  const int& nz = image.info().get_nz();

  const double& hz = image.info().get_hz();

  Json::Value meshModifications;
  std::ifstream modFile(settings.meshModifications);

  // throw exception if file can not be opened
  if( modFile.is_open() == false ) {

    throw std::runtime_error("Cannot open mesh modification file.");

  }

  modFile >> meshModifications;

  modFile.close();

  for(int i = 0; i < nz; ++i) {

    // extract slice of segmented sequence
    Image copy = image;
    copy.crop().to(0, 0, i, nx-1, ny-1, i);

    // build point cloud
    Mesh cloud = CreatePointCloud(copy.data(), diffusionSettings).create_point_cloud();

    // map cloud to tongue model
    meshModify::ApplyModifications(cloud).apply(meshModifications);

    // update data for current time frame
    tracker.data().target = cloud;
    tracker.data().currentTime = i * hz;

    tracker.fitting().fit();

  }

  Json::Value loggedData = tracker.logger().get_logged_data();

  std::ofstream outFile(settings.outputFileName);

  outFile << loggedData << std::endl;

  outFile.close();

  return 0;

}
