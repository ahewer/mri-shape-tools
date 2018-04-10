#ifndef __FILTER_ACTION_H__
#define __FILTER_ACTION_H__

#include <json/json.h>

#include "action/Action.h"

class FilterAction : public Action {

protected:

  Json::Value add_landmarks_if_needed(const std::string& originalString) const {

    Json::Value description;
    Json::Reader reader;

    reader.parse(originalString, description);

    for(Json::Value& entry: description) {

      const std::string action = entry["action"].asString();
      Json::Value& options = entry["options"];

      if( action == "segment" && options["type"].asString() == "with landmarks") {

        Json::Value landmarks(Json::arrayValue);

        for(const auto& mark: LandmarkPool::get_instance()->get_all_landmarks() ) {

          Json::Value landmark(Json::objectValue);

          landmark["x"] = Json::Value(mark->get_position().get_canonical_x());
          landmark["y"] = Json::Value(mark->get_position().get_canonical_y());
          landmark["z"] = Json::Value(mark->get_position().get_canonical_z());

          landmarks.append(landmark);

        }

        options["landmarks"] = landmarks;

      }

    }

    return description;

  }

};
#endif
