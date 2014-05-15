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

		void ImageScanner::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Algorithm>(*this);
		}
		
		void ImageScanner::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<Algorithm>(*this);
		}
	}
}