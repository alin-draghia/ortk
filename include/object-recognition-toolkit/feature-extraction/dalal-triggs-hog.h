#pragma once
#ifndef DALAL_TRIGGS_HOG_H_
#define DALAL_TRIGGS_HOG_H_

#include "object-recognition-toolkit/feature-extraction/feature-extractor.h"

#include <opencv2/opencv.hpp>

namespace object_recognition_toolkit {
	namespace feature_extraction {

		class DalalTriggsHog
			: public FeatureExtractor {
		public:
			DalalTriggsHog();
			virtual ~DalalTriggsHog();

			virtual const std::string& name( ) const;
			virtual std::vector<float> compute(const cv::Mat& image) const;

		private:
			const std::string name_ = "DalalTriggsHog";
			cv::HOGDescriptor hog_;
			cv::Size winSize_;
			cv::Size blockSize_;
			cv::Size blockStride_;
			cv::Size cellSize_;
			int nBins_;
		};

	}
}

#endif DALAL_TRIGGS_HOG_H_