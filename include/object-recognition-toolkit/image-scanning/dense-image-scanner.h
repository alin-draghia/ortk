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
			virtual core::Clonable* Clone();

		public:
			virtual std::vector<Window> compute(const core::Matrix& image) const;

		public:
			core::Size GetWindowSize() const;
			core::Size GetWindowStep() const;
			core::Size GetPadding() const;

		private:
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