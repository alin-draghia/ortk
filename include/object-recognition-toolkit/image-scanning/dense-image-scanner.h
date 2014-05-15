#pragma once
#ifndef DENSE_IMAGE_SCANNER_H_
#define DENSE_IMAGE_SCANNER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "./image-scanner.h"

namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		class PUBLIC_API DenseImageScanner
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

		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};

	}
}

#pragma warning(pop)

#endif