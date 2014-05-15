#pragma once
#ifndef FEATURE_EXTRACTOR_H_
#define FEATURE_EXTRACTOR_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <vector>

#include <opencv2/opencv.hpp>

#include "../core/public-api.h"
#include "../core/named.h"
#include "../core/serialization.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {


		class PUBLIC_API FeatureExtractor
			: public core::Named
		{
		public:
			virtual ~FeatureExtractor( ) = 0;
			
		public:
			virtual std::vector<float> compute(const cv::Mat& image) const = 0;
			virtual int lenght() const = 0;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};

		
	}
}

#pragma warning(pop)

#endif // FEATURE_EXTRACTOR_H_