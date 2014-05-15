#pragma once
#ifndef DENSE_IMAGE_SCANNER_H_
#define DENSE_IMAGE_SCANNER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "./image-scanner.h"

namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		class PUBLIC_API DenseImageScanner
			: public ImageScanner
		{
			DenseImageScanner();

		public:
			DenseImageScanner(core::Size windowSize, core::Size windowStep, core::Size padding);
			~DenseImageScanner();

		public:
			virtual const std::string& name() const;

		public:
			virtual void ScanImage(const core::Matrix& image, std::vector<core::Matrix>& windows, std::vector<core::Box>& boxes) const;

		public:
			core::Size GetWindowSize() const;
			core::Size GetWindowStep() const;
			core::Size GetPadding() const;

		private:
			core::Size windowSize_;
			core::Size windowStep_;
			core::Size padding_;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};

	}
}

#pragma warning(pop)

#endif