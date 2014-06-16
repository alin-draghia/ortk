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
		public:
			DenseImageScanner();
			DenseImageScanner(core::Size windowSize, core::Size windowStep, core::Size padding);
			~DenseImageScanner();

			virtual boost::shared_ptr<ImageScanner> Clone() const;

		public:
			virtual std::vector<Window> compute(const core::Matrix& image) const;

		public:
			core::Size GetWindowSize() const;
			core::Size GetWindowStep() const;
			core::Size GetPadding() const;

		public:
			core::Size windowSize_;
			core::Size windowStep_;
			core::Size padding_;

		private:
			DECLARE_SERIALIZABLE();
		};

	}
}

#pragma warning(pop)

#endif