#include <Poco/ClassLibrary.h>

#include "object-recognition-toolkit/feature-extraction/feature-extractor.h"
#include <object-recognition-toolkit/feature-extraction/pass-through.h>
#include <object-recognition-toolkit/feature-extraction/dalal-triggs-hog.h>
#include <object-recognition-toolkit/feature-extraction/mock-feature-extractor.h>

namespace object_recognition_toolkit {
	namespace feature_extraction {

		FeatureExtractor::~FeatureExtractor( ) { }
	}
}

namespace {
#if(0)
	using namespace object_recognition_toolkit::feature_extraction;

	POCO_BEGIN_NAMED_MANIFEST(feature_extraction, FeatureExtractor)
		POCO_EXPORT_CLASS(PassThrough)
		POCO_EXPORT_CLASS(DalalTriggsHog)
		POCO_EXPORT_CLASS(MockFeatureExtractor)
	POCO_END_MANIFEST
#endif
}