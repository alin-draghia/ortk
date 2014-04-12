#include <Poco/ClassLibrary.h>

#include "object-recognition-toolkit/feature-extraction/feature-extractor.h"
#include "pass-through.h"
#include "dallal-trigs-hog.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		FeatureExtractor::~FeatureExtractor( ) { }
	}
}

namespace {
	using namespace object_recognition_toolkit::feature_extraction;

	POCO_BEGIN_NAMED_MANIFEST(feature_extraction, FeatureExtractor)
		POCO_EXPORT_CLASS(PassThrough)
		POCO_EXPORT_CLASS(DallalTriggsHog)
	POCO_END_MANIFEST
}