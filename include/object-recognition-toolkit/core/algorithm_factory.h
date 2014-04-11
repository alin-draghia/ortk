#pragma once
#ifndef algorithm_factory_header_333AE7CD_A011_4744_9E9A_115251BDDB9C
#define algorithm_factory_header_333AE7CD_A011_4744_9E9A_115251BDDB9C

#include "algorithm.h"
#include "object-recognition-toolkit\feature-extraction\feature-extractor.h"
#include <Poco\AutoPtr.h>
#include <Poco\Instantiator.h>
#include <Poco\DynamicFactory.h>

namespace object_recognition_toolkit
{
	namespace core
	{
#if(0)
		class AlgorithmFactory
		{
		public:

			using FeatureExtractorInstanciator = Poco::AbstractInstantiator<object_recognition_toolkit::feature_extraction::FeatureExtractor>;

			AlgorithmFactory();
			~AlgorithmFactory();

			Poco::AutoPtr<object_recognition_toolkit::feature_extraction::FeatureExtractor>
			createFeatureExtractor(const std::string& featureExtractorName);

		private:
			void registerBuiltins();


		};
#endif
	}
}

#endif algorithm_factory_header_333AE7CD_A011_4744_9E9A_115251BDDB9C

