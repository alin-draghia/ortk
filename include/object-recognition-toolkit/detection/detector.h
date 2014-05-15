#pragma once
#ifndef DETECTOR_H_INCLUDED_
#define DETECTOR_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)


#include "../core/public-api.h"
#include "../core/algorithm.h"

namespace object_recognition_toolkit
{
	namespace detection
	{
		class PUBLIC_API Detector
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			virtual ~Detector() = 0;

			virtual void detect(const cv::Mat& image, std::vector<cv::Rect>& detections, std::vector<double>& confidences, double treshold=0.0) const = 0;

		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};
	}
}

#pragma warning(pop)

#endif // DETECTOR_H_INCLUDED_