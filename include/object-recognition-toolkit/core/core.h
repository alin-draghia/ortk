#pragma once
#ifndef CORE_H_
#define CORE_H_

#include <opencv2/opencv.hpp>

#include "./public-api.h"

namespace object_recognition_toolkit
{
	namespace core
	{
		using Matrix = cv::Mat;
		using Box = cv::Rect;
		using Size = cv::Size;
		using FeatureVector = cv::Mat;

		Matrix PUBLIC_API imread(const std::string& filename, bool color = true);
		void PUBLIC_API imwrite(const std::string& filename, const Matrix& image);

		void PUBLIC_API imshow(const Matrix& image, const std::string& name = "disp");
		int PUBLIC_API show(int delay=0);
	}
}

#endif // CORE_H_