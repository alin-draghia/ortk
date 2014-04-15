#pragma once
#ifndef DUMMY_FEATURE_EXTRACTOR_H_
#define DUMMY_FEATURE_EXTRACTOR_H_

#include <string>
#include <vector>

#include <object-recognition-toolkit/core/core.h>
#include <object-recognition-toolkit/feature-extraction/feature-extractor.h>

namespace object_recognition_toolkit {
	namespace feature_extraction {


		class ORT_API MockFeatureExtractor
			: public FeatureExtractor
		{
		public:
			MockFeatureExtractor();
			virtual ~MockFeatureExtractor();

			virtual const std::string& name() const;
			virtual std::vector<float> compute(const cv::Mat& image) const;

		private:
			const std::string name_ = "MockFeatureExtractor";
		};


	}
}

#endif