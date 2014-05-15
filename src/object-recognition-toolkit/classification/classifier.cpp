#include <object-recognition-toolkit/classification/classifier.h>

#include <boost/serialization/base_object.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/export.hpp>

namespace object_recognition_toolkit
{
	namespace classification
	{


		Classifier::~Classifier() { }


		void Classifier::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}


		void Classifier::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

	}
}

