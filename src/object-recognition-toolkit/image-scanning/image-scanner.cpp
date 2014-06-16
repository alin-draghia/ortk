#include "precomp.h"
#include <object-recognition-toolkit/image-scanning/image-scanner.h>


namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		ImageScanner::~ImageScanner()
		{
		}

		boost::shared_ptr<ImageScanner> ImageScanner::Clone() const
		{
			throw core::NotImplementedError();
		}

		std::vector<Window> ImageScanner::compute(const core::Matrix& image) const
		{
			throw core::NotImplementedError();
		}


		void ImageScanner::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
		
		void ImageScanner::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
	}
}