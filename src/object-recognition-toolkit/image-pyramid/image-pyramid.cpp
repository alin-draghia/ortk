#include <object-recognition-toolkit/image-pyramid/image-pyramid.h>

namespace object_recognition_toolkit
{
	namespace pyramid
	{

		ImagePyramid::~ImagePyramid()
		{
		}

		void ImagePyramid::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}


		void ImagePyramid::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
	}
}