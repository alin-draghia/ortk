#include <object-recognition-toolkit/classification/classifier.h>

namespace object_recognition_toolkit
{
	namespace classification
	{


		Classifier::~Classifier() { }


		void Classifier::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}


		void Classifier::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

	}
}

