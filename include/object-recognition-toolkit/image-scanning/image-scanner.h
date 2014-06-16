#pragma once
#ifndef IMAGE_SCANNER_H_
#define IMAGE_SCANNER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <vector>

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/clonable.h"
#include "../core/serialization.h"
#include "window.h"

#include <boost/shared_ptr.hpp>

namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		class PUBLIC_API ImageScanner
		{
		public:			
			virtual ~ImageScanner();

			virtual boost::shared_ptr<ImageScanner> Clone() const;

		public:			
			virtual std::vector<Window> compute(core::Matrix const& image) const;

		private:
			DECLARE_SERIALIZABLE();
		};

	}
}

#pragma warning(pop)

#endif