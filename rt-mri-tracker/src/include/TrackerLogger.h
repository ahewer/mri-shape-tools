#ifndef __TRACKER_LOGGER_H__
#define __TRACKER_LOGGER_H__

#include <vector>
#include <json/json.h>

#include "TrackerData.h"
#include "TrackerState.h"

class TrackerLogger{

public:

  /*--------------------------------------------------------------------------*/

  TrackerLogger(
    const TrackerData& trackerData,
    const TrackerState& trackerState
    ) : trackerData(trackerData), trackerState(trackerState) {

    this->loggedData = Json::Value(Json::arrayValue);

  }

  /*--------------------------------------------------------------------------*/

  Json::Value get_logged_data() const {
    return this->loggedData;
  }

  /*--------------------------------------------------------------------------*/

  void log() {

    Json::Value logData(Json::objectValue);

    log_time(logData);
    log_energy(logData);
    log_weights(logData);

    this->loggedData.append(logData);

  }

  /*--------------------------------------------------------------------------*/


private:

  /*--------------------------------------------------------------------------*/

  void log_time(Json::Value& logData) const {

    logData["timeStamp"] = this->trackerData.currentTime;

  }

  /*--------------------------------------------------------------------------*/

  void log_energy(Json::Value& logData) const {
    logData["energy"] = this->trackerData.energy;
  }

  /*--------------------------------------------------------------------------*/

  void log_weights(Json::Value& logData) const {

    // select right speaker weights
    arma::vec speakerWeights =
      (this->trackerState.isPCA) ? this->trackerData.fixedSpeakerWeights : this->trackerData.speakerWeight;

    arma::vec phonemeWeights = this->trackerData.phonemeWeight;

    arma::vec speakerVariations = speakerWeights;
    arma::vec phonemeVariations = phonemeWeights;

    this->trackerData.originalModel.convert().to_variations(
      speakerVariations,
      phonemeVariations
      );

    Json::Value jsonSpeakerWeights(Json::arrayValue);
    Json::Value jsonSpeakerVariations(Json::arrayValue);

    Json::Value jsonPhonemeWeights(Json::arrayValue);
    Json::Value jsonPhonemeVariations(Json::arrayValue);

    for(unsigned int i = 0; i < speakerWeights.n_elem; ++i) {

      jsonSpeakerWeights.append(speakerWeights(i));
      jsonSpeakerVariations.append(speakerVariations(i));

    }

    for(unsigned int i = 0; i < phonemeWeights.n_elem; ++i) {

      jsonPhonemeWeights.append(phonemeWeights(i));
      jsonPhonemeVariations.append(phonemeVariations(i));

    }

    logData["speakerWeights"] = jsonSpeakerWeights;
    logData["speakerVariations"] = jsonSpeakerVariations;

    logData["phonemeWeights"] = jsonPhonemeWeights;
    logData["phonemeVariations"] = jsonPhonemeVariations;

  }

  /*--------------------------------------------------------------------------*/

  const TrackerData& trackerData;
  const TrackerState& trackerState;

  Json::Value loggedData;

  /*--------------------------------------------------------------------------*/

};

#endif
