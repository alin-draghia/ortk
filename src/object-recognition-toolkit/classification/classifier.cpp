
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
		void Classifier::save(Archive& ar, const unsigned int version) const
		{
			ar << boost::serialization::base_object<const Algorithm>(*this);
			
		}


		template<class Archive>
		void Classifier::load(Archive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Algorithm>(*this);
		}
	}
}

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
BOOST_CLASS_EXPORT_IMPLEMENT(object_recognition_toolkit::classification::Classifier);