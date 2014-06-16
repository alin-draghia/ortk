#include "precomp.h"
#include <object-recognition-toolkit/classification/classifier.h>

namespace object_recognition_toolkit
{
	namespace classification
	{


		Classifier::~Classifier() 
		{
		}


		boost::shared_ptr<Classifier> Classifier::Clone() const
		{
			throw core::NotImplementedError();
		}


		double Classifier::Predict(const core::FeatureVector& x) const
		{
			throw core::NotImplementedError();
		}

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

