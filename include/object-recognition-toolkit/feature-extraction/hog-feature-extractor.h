#pragma once
#ifndef HOG_FEATURE_EXTRACTOR_H_
#define HOG_FEATURE_EXTRACTOR_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <opencv2/opencv.hpp>

#include "./feature-extractor.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {


		class PUBLIC_API HogFeatureExtractor
			: public FeatureExtractor 
		{
		public:
			HogFeatureExtractor();
			virtual ~HogFeatureExtractor();

		public:
			virtual const std::string& name() const;

		public:
			virtual core::Clonable* Clone();

		public:
			virtual core::FeatureVector compute(const core::Matrix& image) const;
			virtual int lenght() const;

		private:
			cv::HOGDescriptor hog_;
			cv::Size winSize_;
			cv::Size blockSize_;
			cv::Size blockStride_;
			cv::Size cellSize_;
			int nBins_;

		private:
			DECLARE_SERIALIZABLE();
		};


	}
}

#pragma warning(pop)

#endif // HOG_FEATURE_EXTRACTOR_H_