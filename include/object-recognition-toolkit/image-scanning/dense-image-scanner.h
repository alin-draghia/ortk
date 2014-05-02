#pragma once
#ifndef DENSE_IMAGE_SCANNER_H_
#define DENSE_IMAGE_SCANNER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <object-recognition-toolkit/image-scanning/image-scanner.h>

namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		class ORT_API DenseImageScanner
			: public ImageScanner
		{
		public:
			DenseImageScanner(cv::Size windowSize, cv::Size windowStep, cv::Size padding);
			~DenseImageScanner();

			cv::Size GetWindowSize() const;
			cv::Size GetWindowStep() const;
			cv::Size GetPadding() const;

			virtual void ScanImage(const cv::Mat& image, std::vector<cv::Mat>& windows, std::vector<cv::Rect>& boxes) const;

		private:
			cv::Size windowSize_;
			cv::Size windowStep_;
			cv::Size padding_;
		};

	}
}

#pragma warning(pop)

#endif