#include <object-recognition-toolkit/image-scanning/image-scanner.h>


namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		ImageScanner::~ImageScanner()
		{
		}

		void ImageScanner::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
		
		void ImageScanner::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
	}
}