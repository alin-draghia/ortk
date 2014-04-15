#include <object-recognition-toolkit/feature-extraction/pass-through.h>

namespace object_recognition_toolkit {
	namespace feature_extraction {


		PassThrough::PassThrough() 
		{
		}


		PassThrough::~PassThrough() 
		{
		}


		const std::string& PassThrough::name( ) const
		{
			return name_;
		}

		
		std::vector<float> PassThrough::compute(const cv::Mat& image) const
		{
			std::vector<float> feats = image;

			return std::move(feats);
		}


	}
}