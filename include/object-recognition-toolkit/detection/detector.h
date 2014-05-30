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

namespace object_recognition_toolkit
{
	namespace detection
	{
		class PUBLIC_API Detector
			: public core::Named
			, public core::Clonable
		{
		public:
			virtual ~Detector() = 0;

		public:
			virtual void Detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold=0.0) const = 0;

		public:
			virtual pyramid::PyramidBuilder& GetPyramidBuilder() const = 0;
			virtual image_scanning::ImageScanner& GetImageScanner() const = 0;
			virtual feature_extraction::FeatureExtractor& GetFeatureExtracotr() const = 0;
			virtual classification::Classifier& GetClassifier() const = 0;
			virtual non_maxima_suppression::NonMaximaSuppressor& GetNonMaximaSuppressor() const = 0;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};
	}
}

#pragma warning(pop)

#endif // DETECTOR_H_INCLUDED_