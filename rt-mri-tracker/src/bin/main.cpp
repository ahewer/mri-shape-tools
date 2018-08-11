#include "image/Image.h"

#include "settings.h"
#include "Tracker.h"
#include "mesh/MeshIO.h"
#include "mesh/Mesh.h"
#include "model/ModelReader.h"
#include "utility/CreatePointCloud.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Image image;

  image.read().from(settings.inputFileName);

  ModelReader modelReader(settings.model);
  Model model = modelReader.get_model();

  TrackerData trackerData(model, settings);
  trackerData.sourceIds = settings.sourceIds;

  Tracker tracker(trackerData);

  DiffusionSettings diffusionSettings;

  const int& nx = image.info().get_nx();
  const int& ny = image.info().get_ny();
  const int& nz = image.info().get_nz();

  const double& hz = image.info().get_hz();


  for(int i = 0; i < image.info().get_nz(); ++i) {

    // extract slice of segmented sequence
    Image copy = image;
    copy.crop().to(0, 0, i, nx, ny, i);

    // build point cloud
    Mesh cloud = CreatePointCloud(copy.data(), diffusionSettings).create_point_cloud();


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
