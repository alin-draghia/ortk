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
			HogFeatureExtractor(cv::Size winSize, cv::Size blockSize, cv::Size blockStride, cv::Size cellSize, int nBins);
			virtual ~HogFeatureExtractor();

		public:
			virtual boost::shared_ptr<FeatureExtractor> Clone() const;

		public:
			virtual core::FeatureVector Compute(const core::Matrix& image) const;
			virtual void ComputeMulti(std::vector<core::Matrix> const& images, core::Matrix& features) const;
			virtual int Lenght() const;

		public:
			cv::Size winSize_;
			cv::Size blockSize_;
			cv::Size blockStride_;
			cv::Size cellSize_;
			int nBins_;

		private:
			cv::HOGDescriptor hog_;

		private:
			DECLARE_SERIALIZABLE();
		};


	}
}

#pragma warning(pop)

#endif // HOG_FEATURE_EXTRACTOR_H_