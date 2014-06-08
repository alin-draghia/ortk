#include "precomp.h"
#include "object-recognition-toolkit/image-pyramid/pyramid_builder.h"

namespace object_recognition_toolkit
{
	namespace pyramid
	{

		PyramidBuilder::~PyramidBuilder()
		{
		}

		void PyramidBuilder::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}


		void PyramidBuilder::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
	}
}