#include <boost/serialization/base_object.hpp>

#include <object-recognition-toolkit/classification/classifier.h>


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


		template<class Archive>
		void Classifier::save(Archive & ar, const unsigned int version) const
		{
			ar << boost::serialization::base_object<Algorithm>(*this);
		}


		template<class Archive>
		void Classifier::load(Archive & ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Algorithm>(*this);
		}
		
	}
}

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_IMPLEMENT(object_recognition_toolkit::classification::Classifier);



namespace object_recognition_toolkit {
	namespace classification {
		//template<> ORT_API void Classifier::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);		
		//template<> ORT_API void Classifier::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		template ORT_API void Classifier::load(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
		template ORT_API void Classifier::save(boost::archive::polymorphic_oarchive& ar, const unsigned int version) const;
	}
}
