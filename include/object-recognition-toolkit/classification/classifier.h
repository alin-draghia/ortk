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

namespace object_recognition_toolkit
{
	namespace classification
	{
		class PUBLIC_API Classifier
			: public core::Named
			, public core::Clonable
		{
		public:
			virtual ~Classifier();

		public:
			virtual double Predict(const core::FeatureVector& instance) const;

			DECLARE_SERIALIZABLE();
		};
	}
}

#pragma warning(pop)

#endif // CLASSIFIER_H_