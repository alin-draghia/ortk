#pragma once
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <string>
#include <vector>



#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/clonable.h"
#include "../core/serialization.h"

#include <boost/shared_ptr.hpp>

namespace object_recognition_toolkit
{
	namespace classification
	{
		class PUBLIC_API Classifier
		{
		public:
			virtual ~Classifier() = 0;
			
			virtual boost::shared_ptr<Classifier> Clone() const = 0;

		public:
			virtual double Predict(core::FeatureVector const& x) const = 0;
			virtual core::Matrix PredictMulti(core::Matrix const& X) const = 0;

			DECLARE_SERIALIZABLE();
		};
	}
}

#pragma warning(pop)

#endif // CLASSIFIER_H_