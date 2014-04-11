#pragma once
#ifndef _ORT_FEATURE_EXTRACTOR_HEADER_INCLUDED_
#define _ORT_FEATURE_EXTRACTOR_HEADER_INCLUDED_

#include "object-recognition-toolkit\core\algorithm.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		class FeatureExtractor
			: public object_recognition_toolkit::core::Algorithm {
		public:
			FeatureExtractor( );
			virtual ~FeatureExtractor( ) = 0;
		};

	}
}

#endif //_ORT_FEATURE_EXTRACTOR_HEADER_INCLUDED_