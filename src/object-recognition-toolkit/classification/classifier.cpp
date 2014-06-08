#include "precomp.h"
#include <object-recognition-toolkit/classification/classifier.h>

namespace object_recognition_toolkit
{
	namespace classification
	{


		Classifier::~Classifier() {
			std::cerr << __FUNCTION__ << "name=" << this->name() << std::endl;
		}

		double Classifier::Predict(const core::FeatureVector& instance) const
		{
			throw std::runtime_error("not implemented, [~pure_virtual]");
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

