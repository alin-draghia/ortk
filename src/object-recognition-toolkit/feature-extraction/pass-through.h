#pragma once
#ifndef PASS_THROUGH_H_
#define PASS_THROUGH_H_

#include <string>

#include "object-recognition-toolkit/feature-extraction/feature-extractor.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		class PassThrough
			: public FeatureExtractor
		{
		public:
			PassThrough( );
			virtual ~PassThrough( );

			virtual const std::string& name( ) const;

		private:
			const std::string name_ = "PassThrough";
		};

	}
}

#endif