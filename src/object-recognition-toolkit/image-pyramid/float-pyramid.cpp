#include <object-recognition-toolkit/image-pyramid/float-pyramid.h>

namespace object_recognition_toolkit
{
	namespace pyramid
	{

		FloatImagePyramid::FloatImagePyramid(double scaleFactor, cv::Size minSize, cv::Size maxSize) :
			ImagePyramid{ "FloatImagePyramid" },
			scaleFactor_{ scaleFactor },
			minSize_{ minSize },
			maxSize_{ maxSize }
		{
		}


		FloatImagePyramid::~FloatImagePyramid()
		{
		}

		
		double FloatImagePyramid::GetScaleFactor() const
		{
			return scaleFactor_;
		}


		cv::Size FloatImagePyramid::GetMinSize() const
		{
			return minSize_;
		}


		cv::Size FloatImagePyramid::GetMaxSize() const
		{
			return maxSize_;
		}


		std::vector<PyramidLevel> FloatImagePyramid::Build(cv::Mat image) const
		{
			std::vector<PyramidLevel> pyramid;
			

			for (int i = 0; true; i++) {
				double scale = 1.0 / std::pow(scaleFactor_, i);
				int width = cvRound(image.cols * scale);
				int height = cvRound(image.rows * scale);

				if (maxSize_.width != 0 && maxSize_.height != 0) {
					if (width > maxSize_.width || height > maxSize_.height) {
						continue;
					}
				}

				if (width < minSize_.width || height < minSize_.height) {
					break;
				}

				cv::Mat image0;
				cv::resize(image, image0, cv::Size(), scale, scale, cv::INTER_LINEAR);

				pyramid.emplace_back(image0, scale);
			}
			
			return std::move(pyramid);
		}

		void FloatImagePyramid::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<ImagePyramid>(*this);
			ar >> scaleFactor_; 
			ar >> minSize_;
			ar >> maxSize_;
		}

		void FloatImagePyramid::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<ImagePyramid>(*this);
			ar << scaleFactor_;
			ar << minSize_;
			ar << maxSize_;
		}

	}
}