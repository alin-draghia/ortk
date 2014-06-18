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
			LinearClassifier(float b, std::vector<float>& w);
			virtual ~LinearClassifier();

			virtual double Predict(core::FeatureVector const& x) const;
			virtual core::Matrix PredictMulti(core::Matrix const& X) const;

		public:
			virtual boost::shared_ptr<Classifier> Clone() const;

		public:
			float b_;
			std::vector<float> w_;


			DECLARE_SERIALIZABLE();
		};

	}
}

#pragma warning(pop)

#endif // LINEAR_SVC_H_