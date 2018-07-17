#ifndef __MESH_SEQUENCE_BUILDER_H__
#define __MESH_SEQUENCE_BUILDER_H__

#include <string>

#include "ema/Ema.h"
#include "model/Model.h"
#include "RegisteredEma.h"
#include "MeshSequence.h"

class MeshSequenceBuilder{

private:

  Ema headMotion;
  Ema originalHeadMotion;

  RegisteredEma registeredEma;

  Model tongueModel;

  std::string globalTransformation;

  double startTime;
  double endTime;

  double samplingRate;
  double scaleFactor;

  bool headMotionSet = false;
  bool originalHeadMotionSet = false;
  bool registeredEmaSet = false;
  bool tongueModelSet = false;
  bool globalTransformationSet = false;
  bool startTimeSet = false;
  bool endTimeSet = false;
  bool samplingRateSet = false;
  bool scaleFactorSet =false;

public:

  MeshSequence build() {

    if( is_valid() == false) {

      throw std::logic_error("Not all parameters are set.");

    }

    return MeshSequence(
                        this->headMotion,
                        this->originalHeadMotion,
                        this->registeredEma,
                        this->tongueModel,
                        this->globalTransformation,
                        this->startTime,
                        this->endTime,
                        this->samplingRate,
                        this->scaleFactor
                        );

  }

  // setters
  MeshSequenceBuilder& set_head_motion(const Ema& headMotion) {

    this->headMotion = headMotion;
    this->headMotionSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_original_head_motion(const Ema& originalHeadMotion) {

    this->originalHeadMotion = originalHeadMotion;
    this->originalHeadMotionSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_registered_ema(const RegisteredEma& registeredEma) {

    this->registeredEma = registeredEma;
    this->registeredEmaSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_tongue_model(const Model& tongueModel) {

    this->tongueModel = tongueModel;
    this->tongueModelSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_global_transformation(const std::string& globalTransformation) {

    this->globalTransformation = globalTransformation;
    this->globalTransformationSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_start_time(const double& startTime) {

    this->startTime = startTime;
    this->startTimeSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_end_time(const double& endTime) {

    this->endTime = endTime;
    this->endTimeSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_sampling_rate(const double& samplingRate) {

    this->samplingRate = samplingRate;
    this->samplingRateSet = true;

    return *this;

  }

  MeshSequenceBuilder& set_scale_factor(const double& scaleFactor) {

    this->scaleFactor = scaleFactor;
    this->scaleFactorSet = true;

    return *this;

  }

private:

  bool is_valid() const {

    return (

            headMotionSet &&
            originalHeadMotionSet &&
            registeredEmaSet &&
            tongueModelSet &&
            globalTransformationSet &&
            startTimeSet &&
            endTimeSet &&
            samplingRateSet &&
            scaleFactorSet

            );

  }

};
#endif
