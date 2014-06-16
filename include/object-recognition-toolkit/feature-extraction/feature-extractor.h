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

#include <boost/shared_ptr.hpp>

namespace object_recognition_toolkit {
	namespace feature_extraction {


		class PUBLIC_API FeatureExtractor
		{
		public:
			virtual ~FeatureExtractor() = 0;
			
			virtual boost::shared_ptr<FeatureExtractor> Clone() const = 0;

		public:
			virtual core::FeatureVector Compute(core::Matrix const& image) const = 0;
			virtual void ComputeMulti(std::vector<core::Matrix> const& images, core::Matrix& features) const = 0;
			virtual int Lenght() const = 0;

		private:
			DECLARE_SERIALIZABLE();
		};

		
	}
}

#pragma warning(pop)

#endif // FEATURE_EXTRACTOR_H_