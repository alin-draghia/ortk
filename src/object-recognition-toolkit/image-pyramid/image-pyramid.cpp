#include <object-recognition-toolkit/image-pyramid/image-pyramid.h>

namespace object_recognition_toolkit
{
	namespace pyramid
	{

		ImagePyramid::ImagePyramid(const std::string& name)
			: Algorithm(name)
		{
		}


		ImagePyramid::~ImagePyramid()
		{
		}

		void ImagePyramid::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Algorithm>(*this);
		}


		void ImagePyramid::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<Algorithm>(*this);
		}
	}
}