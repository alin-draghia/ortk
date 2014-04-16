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


		std::vector<cv::Mat> FloatImagePyramid::Build(cv::Mat image) const
		{
			std::vector<cv::Mat> pyramid;


			return std::move(pyramid);
		}


		cv::Rect FloatImagePyramid::ForwardTransform(const cv::Rect& box, size_t level) const
		{
			throw std::exception("not implemented");
		}


		cv::Rect FloatImagePyramid::InverseTransform(const cv::Rect& box, size_t level) const
		{
			throw std::exception("not implemented");
		}

	}
}