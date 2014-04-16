#include <object-recognition-toolkit/feature-extraction/hog-feature-extractor.h>


namespace object_recognition_toolkit {
	namespace feature_extraction {


		HogFeatureExtractor::HogFeatureExtractor() : 
			FeatureExtractor("HogFeatureExtractor"), 
			winSize_{ 64, 128 }, 
			blockSize_{ 16, 16 }, 
			blockStride_{ 8, 8 }, 
			nBins_{ 9 }
		{
		}


		HogFeatureExtractor::~HogFeatureExtractor()
		{
		}


		std::vector<float> HogFeatureExtractor::compute(const cv::Mat& image) const
		{
			std::vector<float> feats;
			hog_.compute(image, feats);
			return std::move(feats);
		}


	}
}