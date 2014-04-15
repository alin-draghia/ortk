#pragma once
#ifndef FEATURE_EXTRACTOR_H_
#define FEATURE_EXTRACTOR_H_

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include <Poco/ClassLoader.h>

#include <object-recognition-toolkit/core/core.h>
#include <object-recognition-toolkit/core/algorithm.h>

namespace object_recognition_toolkit {
	namespace feature_extraction {

		class ORT_API FeatureExtractor
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			virtual ~FeatureExtractor( ) = 0;
			
			virtual std::vector<float> compute(const cv::Mat& image) const = 0;
		};


		#if(0)
		using FeatureExtractorLoader = Poco::ClassLoader<FeatureExtractor>;
		const std::string Manifest = "feature_extraction";
		#endif
		

	}
}

#endif // FEATURE_EXTRACTOR_H_