#include <object-recognition-toolkit/classification/classifier.h>

#include <boost/serialization/base_object.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/export.hpp>

namespace object_recognition_toolkit
{
	namespace classification
	{

		Classifier::Classifier()
		{
		}


		Classifier::Classifier(const std::string& name)
			: Algorithm(name)
		{
		}

		
		Classifier::~Classifier()
		{
		}

		double Classifier::PredictConf(const std::vector<float>& instance) const
		{
			throw std::runtime_error("not implemented");
		}

		void Classifier::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{			
			ar >> boost::serialization::base_object<Algorithm>(*this);
		}


		void Classifier::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<Algorithm>(*this);
		}

	}
}

