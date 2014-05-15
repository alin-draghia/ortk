#include <object-recognition-toolkit/image-pyramid/image-pyramid.h>

namespace object_recognition_toolkit
{
	namespace pyramid
	{

		ImagePyramid::~ImagePyramid()
		{
		}

		void ImagePyramid::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}


		void ImagePyramid::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
	}
}