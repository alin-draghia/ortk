#include <string>

#include <object-recognition-toolkit/feature-extraction/feature-extractor.h>


namespace object_recognition_toolkit {
	namespace feature_extraction {


		FeatureExtractor::~FeatureExtractor( ) 
		{
		}

		void FeatureExtractor::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

		void FeatureExtractor::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

	}
}