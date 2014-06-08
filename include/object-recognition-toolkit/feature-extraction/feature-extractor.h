#pragma once
#ifndef FEATURE_EXTRACTOR_H_
#define FEATURE_EXTRACTOR_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/clonable.h"
#include "../core/serialization.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {


		class PUBLIC_API FeatureExtractor
			: public core::Named
			, public core::Clonable
		{
		public:
			virtual ~FeatureExtractor( ) = 0;
			
		public:
			virtual core::FeatureVector compute(const core::Matrix& image) const = 0;
			virtual int lenght() const = 0;

		private:
			DECLARE_SERIALIZABLE();
		};

		
	}
}

#pragma warning(pop)

#endif // FEATURE_EXTRACTOR_H_