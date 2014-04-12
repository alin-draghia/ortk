#include "dallal-trigs-hog.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		DallalTriggsHog::DallalTriggsHog( )
			: winSize_ { 64, 128 }, blockSize_ { 16, 16 },
			blockStride_ { 8, 8 }, nBins_ { 9 }
		{

		}

		DallalTriggsHog::~DallalTriggsHog( ) { }

		const std::string& DallalTriggsHog::name( ) const
		{
			return name_;
		}

		std::vector<float> DallalTriggsHog::compute(const cv::Mat& image) const
		{
			std::vector<float> feats;
			hog_.compute(image, feats);
			return std::move(feats);
		}

	}
}