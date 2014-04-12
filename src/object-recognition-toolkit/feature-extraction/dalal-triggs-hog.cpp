#include "dalal-triggs-hog.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		DalalTriggsHog::DalalTriggsHog()
			: winSize_ { 64, 128 }, blockSize_ { 16, 16 },
			blockStride_ { 8, 8 }, nBins_ { 9 }
		{

		}

		DalalTriggsHog::~DalalTriggsHog() { }

		const std::string& DalalTriggsHog::name() const
		{
			return name_;
		}

		std::vector<float> DalalTriggsHog::compute(const cv::Mat& image) const
		{
			std::vector<float> feats;
			hog_.compute(image, feats);
			return std::move(feats);
		}

	}
}