#pragma once
#ifndef IMAGE_PYRAMID_H_
#define IMAGE_PYRAMID_H_

#include <object-recognition-toolkit/core/algorithm.h>
#include <object-recognition-toolkit/image-pyramid/pyramid-level.h>

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		class ORT_API ImagePyramid
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			ImagePyramid(const std::string& name);
			virtual ~ImagePyramid();

			virtual std::vector<PyramidLevel> Build(cv::Mat image) const = 0;
			
		};
	}
}

#endif // IMAGE_PYRAMID_H_