#include <fstream>
#include <json/json.h>
#include <armadillo>

#include "ema/Ema.h"

#include "settings.h"

int main(int argc, char* argv[]) {

  Settings settings(argc, argv);

  Ema ema;
  ema.read().from(settings.input);

  std::ifstream inFile(settings.timeStampsFile); 
  if( inFile.is_open() == false ) {
  
    throw std::runtime_error("Cannot open timestamps file.");
  
  }

  Json::Value timeStamps;

  inFile >> timeStamps;
  inFile.close();
 
  Json::Value root(Json::arrayValue);

  for(const Json::Value& timeStamp: timeStamps) {

    const arma::vec position = ema.coil(settings.coil).interpolate().position_at_time(timeStamp.asDouble());

    Json::Value entry(Json::objectValue);

    entry["x"] = position(0);
    entry["y"] = position(1);
    entry["z"] = position(2);

    root.append(entry);

  }

  std::ofstream outFile(settings.output);
  outFile << root << std::endl;
  outFile.close();

  return 0;

}
