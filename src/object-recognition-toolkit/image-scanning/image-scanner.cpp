#include <object-recognition-toolkit/image-scanning/image-scanner.h>


namespace object_recognition_toolkit
{
	namespace image_scanning
	{

		
		ImageScanner::ImageScanner(const std::string& name) :
			object_recognition_toolkit::core::Algorithm{ name }
		{
		}


		ImageScanner::~ImageScanner()
		{
		}


	}
}