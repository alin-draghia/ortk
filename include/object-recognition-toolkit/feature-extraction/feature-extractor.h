#pragma once
#ifndef FEATURE_EXTRACTOR_H_
#define FEATURE_EXTRACTOR_H_

#include <string>

#include <Poco/ClassLoader.h>

namespace object_recognition_toolkit {
	namespace feature_extraction {

		class FeatureExtractor
		{
		public:
			virtual ~FeatureExtractor( ) = 0;
			virtual const std::string& name( ) const = 0;
		};

		using FeatureExtractorLoader = Poco::ClassLoader<FeatureExtractor>;

		const std::string Manifest = "feature_extraction";

		

	}
}

#endif // FEATURE_EXTRACTOR_H_