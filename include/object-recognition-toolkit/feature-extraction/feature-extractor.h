#pragma once
#ifndef FEATURE_EXTRACTOR_H_
#define FEATURE_EXTRACTOR_H_

#include <string>

#include "object-recognition-toolkit\core\algorithm.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		class FeatureExtractor
		{
		public:
			virtual ~FeatureExtractor( ) = 0;
		};

	}
}

#endif // FEATURE_EXTRACTOR_H_