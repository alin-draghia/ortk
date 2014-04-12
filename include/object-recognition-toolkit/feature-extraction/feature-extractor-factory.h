#pragma once
#ifndef FEATURE_EXTRACTOR_FACTORY_H_
#define FEATURE_EXTRACTOR_FACTORY_H_

#include <string>
#include <memory>
#include <list>

#include <Poco/Instantiator.h>
#include <Poco/DynamicFactory.h>

#include "feature-extractor.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		class FeatureExtractorFactory
		{

		public:
			using Instantiator = Poco::AbstractInstantiator<FeatureExtractor>;

			FeatureExtractorFactory( );
			~FeatureExtractorFactory( );

			FeatureExtractor* create(const std::string& name) const;
			void registerFeatureExtractor(const std::string& name, std::unique_ptr<Instantiator> instantiator);
			void unregisterFeatureExtractor(const std::string& name);

		private:
			void registerBuiltins( );

			Poco::DynamicFactory<FeatureExtractor> factory_;
			std::list<std::string> classes_;
		};

	}
}
#endif
