#pragma once
#ifndef HOG_FEATURE_EXTRACTOR_H_
#define HOG_FEATURE_EXTRACTOR_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <opencv2/opencv.hpp>

#include "../core/public-api.h"
#include "./feature-extractor.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {


		class PUBLIC_API HogFeatureExtractor
			: public FeatureExtractor {
		public:
			HogFeatureExtractor();
			virtual ~HogFeatureExtractor();

			virtual std::vector<float> compute(const cv::Mat& image) const;

		private:
			cv::HOGDescriptor hog_;
			cv::Size winSize_;
			cv::Size blockSize_;
			cv::Size blockStride_;
			cv::Size cellSize_;
			int nBins_;
		};


	}
}

#pragma warning(pop)

#endif // HOG_FEATURE_EXTRACTOR_H_