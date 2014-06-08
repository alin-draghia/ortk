#pragma once
#ifndef LINEAR_SVC_H_
#define LINEAR_SVC_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "./classifier.h"

namespace object_recognition_toolkit
{
	namespace classification
	{

		class PUBLIC_API LinearClassifier
			: public Classifier
		{
		public:
			LinearClassifier() = default;
			LinearClassifier(float bias, std::vector<float>& coefs);
			virtual ~LinearClassifier();

			virtual double Predict(const core::FeatureVector& instance) const;

		public:
			virtual const std::string& name() const;

		public:
			virtual core::Clonable* Clone();

		private:
			float bias_;
			std::vector<float> coefs_;


			DECLARE_SERIALIZABLE();
		};

	}
}

#pragma warning(pop)

#endif // LINEAR_SVC_H_