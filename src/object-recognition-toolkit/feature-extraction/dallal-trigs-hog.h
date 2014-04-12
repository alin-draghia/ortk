#pragma once
#ifndef DALAL_TRIGGS_HOG_H_
#define DALAL_TRIGGS_HOG_H_

#include "object-recognition-toolkit/feature-extraction/feature-extractor.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		class DallalTriggsHog
			: public FeatureExtractor {
		public:
			DallalTriggsHog( );
			virtual ~DallalTriggsHog( );

			virtual const std::string& name( ) const;

		private:
			const std::string name_ = "DallalTriggsHog";
		};

	}
}

#endif DALAL_TRIGGS_HOG_H_