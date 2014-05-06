#pragma once
#ifndef IMAGE_PYRAMID_H_
#define IMAGE_PYRAMID_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "../core/algorithm.h"
#include "./pyramid-level.h"

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		class PUBLIC_API ImagePyramid
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			ImagePyramid(const std::string& name);
			virtual ~ImagePyramid();

			virtual std::vector<PyramidLevel> Build(cv::Mat image) const = 0;
			
		};
	}
}

#pragma warning(pop)

#endif // IMAGE_PYRAMID_H_