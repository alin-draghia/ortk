#pragma once
#ifndef IMAGE_SCANNER_H_
#define IMAGE_SCANNER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <vector>

#include "../core/public-api.h"
#include "../core/core.h"
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
			virtual void ScanImage(const core::Matrix& image, std::vector<core::Matrix>& windows, std::vector<core::Box>& boxes) const = 0;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};

	}
}

#pragma warning(pop)

#endif