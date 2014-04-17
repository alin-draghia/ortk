#pragma once
#ifndef FLOAT_PYRAMID_H_
#define FLOAT_PYRAMID_H_

#include <object-recognition-toolkit/image-pyramid/image-pyramid.h>

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		class ORT_API FloatImagePyramid
			: public ImagePyramid
		{
		public:
			FloatImagePyramid(double scaleFactor, cv::Size minSize, cv::Size maxSize);
			virtual ~FloatImagePyramid();

			double GetScaleFactor() const;
			cv::Size GetMinSize() const;
			cv::Size GetMaxSize() const;

			virtual std::vector<PyramidLevel> Build(cv::Mat image) const;



		private:
			double scaleFactor_;
			cv::Size minSize_;
			cv::Size maxSize_;
		};
	}
}

#endif // FLOAT_PYRAMID_H_
