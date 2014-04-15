#include <object-recognition-toolkit/feature-extraction/mock-feature-extractor.h>

#include <memory>
#include <opencv2/opencv.hpp>

namespace object_recognition_toolkit {
	namespace feature_extraction {

		MockFeatureExtractor::MockFeatureExtractor( )
		{
			addParam("n", 100, "help: int param ");
			addParam("nstr", std::string("1234"), "help: string param");
			addParam("pt", cv::Point( ), "help: cv::Point param");
			addParam("sz", cv::Size( ), "help: cv::Size param");
			addParam("mat", cv::Mat( ), "help: cv::Mat param");
			addParam("vec", std::vector<double>( ), "help: std::vector<double>");
			addParam("alg", std::shared_ptr<object_recognition_toolkit::core::Algorithm>(nullptr), "help: algorithm param");
		}

		MockFeatureExtractor::~MockFeatureExtractor( ) { }

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