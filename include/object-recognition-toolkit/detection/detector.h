#pragma once
#ifndef DETECTOR_H_INCLUDED_
#define DETECTOR_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)


#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/clonable.h"
#include "../core/serialization.h"
#include "../image-pyramid/pyramid_builder.h"
#include "../image-scanning/image-scanner.h"
#include "../feature-extraction/feature-extractor.h"
#include "../classification/classifier.h"
#include "../non_maxima_suppression/non_maxima_suppressor.h"

#include <boost/shared_ptr.hpp>

namespace object_recognition_toolkit
{
	namespace detection
	{
		class PUBLIC_API Detector
		{
		public:
			virtual ~Detector();

			virtual boost::shared_ptr<Detector> Clone() const;

		public:
			virtual void Detect(core::Matrix const& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold = 0.0) const;

		private:
			DECLARE_SERIALIZABLE();
		};
	}
}

#pragma warning(pop)

#endif // DETECTOR_H_INCLUDED_