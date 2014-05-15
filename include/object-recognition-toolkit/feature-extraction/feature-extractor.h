#pragma once
#ifndef FEATURE_EXTRACTOR_H_
#define FEATURE_EXTRACTOR_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <vector>

#include <opencv2/opencv.hpp>

#include "../core/public-api.h"
#include "../core/algorithm.h"
#include "../core/serialization.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {


		class PUBLIC_API FeatureExtractor
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			FeatureExtractor(const std::string& name);
			virtual ~FeatureExtractor( );
			
			virtual std::vector<float> compute(const cv::Mat& image) const = 0;
			virtual int lenght() const = 0;
		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};

		
	}
}

#pragma warning(pop)

#endif // FEATURE_EXTRACTOR_H_