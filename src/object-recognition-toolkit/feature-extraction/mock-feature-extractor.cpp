#include <memory>

#include <opencv2/opencv.hpp>

#include <object-recognition-toolkit/feature-extraction/mock-feature-extractor.h>

namespace object_recognition_toolkit {
	namespace feature_extraction {


		MockFeatureExtractor::MockFeatureExtractor( )
		{
		}


		MockFeatureExtractor::~MockFeatureExtractor( ) 
		{
		}


		const std::string& MockFeatureExtractor::name( ) const
		{
			return name_;
		}


		std::vector<float> MockFeatureExtractor::compute(const cv::Mat& image) const
		{
			return std::vector<float>( );
		}


	}
}