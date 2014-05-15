#pragma once
#ifndef IMAGE_SCANNER_H_
#define IMAGE_SCANNER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <vector>

#include <opencv2/opencv.hpp>

#include "../core/public-api.h"
#include "../core/named.h"
#include "../core/serialization.h"

namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		class PUBLIC_API ImageScanner
			: public core::Named
		{
		public:			
			virtual ~ImageScanner() = 0;		
	
		public:
			virtual void ScanImage(const cv::Mat& image, std::vector<cv::Mat>& windows, std::vector<cv::Rect>& boxes) const = 0;

		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};

	}
}

#pragma warning(pop)

#endif