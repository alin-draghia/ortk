#include "dallal-trigs-hog.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		DallalTriggsHog::DallalTriggsHog( ) { }

		DallalTriggsHog::~DallalTriggsHog( ) { }
		
		const std::string& DallalTriggsHog::name() const
		{
			return name_;
		}

	}
}