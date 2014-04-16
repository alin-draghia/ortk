#pragma once
#ifndef IMAGE_SCANNER_H_
#define IMAGE_SCANNER_H_

#include <cinttypes>
#include <vector>
#include <tuple>

#include <object-recognition-toolkit/core/core.h>
#include <object-recognition-toolkit/core/algorithm.h>

namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		class ORT_API ImageScanner
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			ImageScanner(const std::string& name);
			virtual ~ImageScanner();		
	
			virtual void ScanImage(const cv::Mat& image, std::vector<cv::Mat>& windows, std::vector<cv::Rect>& boxes) const = 0;
		};

	}
}

#endif