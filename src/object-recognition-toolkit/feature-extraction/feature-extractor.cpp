#include <string>

#include <object-recognition-toolkit/feature-extraction/feature-extractor.h>


namespace object_recognition_toolkit {
	namespace feature_extraction {


		FeatureExtractor::FeatureExtractor(const std::string& name) : 
			object_recognition_toolkit::core::Algorithm(name)
		{
		}


		FeatureExtractor::~FeatureExtractor( ) 
		{
		}

		void FeatureExtractor::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Algorithm>(*this);
		}

		void FeatureExtractor::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<Algorithm>(*this);
		}

	}
}