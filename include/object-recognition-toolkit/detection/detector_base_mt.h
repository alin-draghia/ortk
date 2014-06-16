#pragma once
#ifndef MULTI_THREADED_DETECTOR_BASE_H_INCLUDED_
#define MULTI_THREADED_DETECTOR_BASE_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "detector_base.h"

namespace object_recognition_toolkit
{
	namespace detection
	{

		class PUBLIC_API DetectorBaseMt
			: public DetectorBase
		{
			using PyramidBuilder = pyramid::PyramidBuilder;
			using PyramidLevel = pyramid::PyramidLevel;
			using ImageScanner = image_scanning::ImageScanner;
			using FeatureExtractor = feature_extraction::FeatureExtractor;
			using Classifier = classification::Classifier;
			using NonMaximaSuppressor = non_maxima_suppression::NonMaximaSuppressor;

		public:
			DetectorBaseMt();
			virtual ~DetectorBaseMt();

			virtual boost::shared_ptr<Detector> Clone() const;

		public:
			virtual void Detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold = 0.0) const;


		private:
			DECLARE_SERIALIZABLE();

		private:
			friend class DetectorBaseMT_Builder;
		};

	}
}

#pragma warning(pop)

#endif // MULTI_THREADED_DETECTOR_BASE_H_INCLUDED_