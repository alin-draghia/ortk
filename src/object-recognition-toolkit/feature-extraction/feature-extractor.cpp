#include "precomp.h"
#include <string>

#include <object-recognition-toolkit/feature-extraction/feature-extractor.h>


namespace object_recognition_toolkit {
	namespace feature_extraction {


		FeatureExtractor::~FeatureExtractor( ) 
		{
		}

		boost::shared_ptr<FeatureExtractor> FeatureExtractor::Clone() const
		{
			throw core::NotImplementedError();
		}

		core::FeatureVector FeatureExtractor::Compute(const core::Matrix& image) const
		{
			throw core::NotImplementedError();
		}

		void FeatureExtractor::ComputeMulti(std::vector<core::Matrix> const& images, core::Matrix& features) const
		{
			throw core::NotImplementedError();
		}

		int FeatureExtractor::Lenght() const
		{
			throw core::NotImplementedError();
		}


		void FeatureExtractor::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

		void FeatureExtractor::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

	}
}