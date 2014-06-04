#pragma once
#ifndef OBJECT_RECOGNITION_TOOLKIT_HEADER_INCLUDED_
#define OBJECT_RECOGNITION_TOOLKIT_HEADER_INCLUDED_

#include "core/core.h"
#include "core/public-api.h"
#include "core/named.h"
#include "core/clonable.h"
#include "core/serialization.h"

#include "dataset/dataset.h"

#include "feature-extraction/feature-extractor.h"
#include "feature-extraction/hog-feature-extractor.h"

#include "image-pyramid/pyramid.h"
#include "image-pyramid/pyramid-level.h"
#include "image-pyramid/pyramid_builder.h"
#include "image-pyramid/float_pyramid_builder.h"

#include "image-scanning/window.h"
#include "image-scanning/image-scanner.h"
#include "image-scanning/dense-image-scanner.h"

#include "non_maxima_suppression/non_maxima_suppressor.h"
#include "non_maxima_suppression/pass_through_nms.h"

#include "classification/classifier.h"
#include "classification/trainer.h"
#include "classification/linear-svc.h"
#include "classification/linear-svc-trainer.h"
#include "classification/mock-person-classifier.h"

#include "detection/detector.h"
#include "detection/detector_base.h"
#include "detection/detector_base_mt.h"
#include "detection/detector_trainer.h"
#include "detection/dallal_triggs_detector_trainer.h"
#include "detection/detector_builder.h"


namespace object_recognition_toolkit
{
}

#endif // OBJECT_RECOGNITION_TOOLKIT_HEADER_INCLUDED_