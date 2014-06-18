#pragma once
#ifndef MOCK_PERSON_CLASSIFIER_H_
#define MOCK_PERSON_CLASSIFIER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "./classifier.h"


namespace object_recognition_toolkit
{
	namespace classification
	{

		class PUBLIC_API MockPersonClassifier
			: public Classifier
		{
		public:
			MockPersonClassifier();
			virtual ~MockPersonClassifier();

			virtual double Predict(core::FeatureVector const& x) const;
			core::Matrix PredictMulti(core::Matrix const& X) const;

		public:
			virtual boost::shared_ptr<Classifier> Clone() const;

		private:
			static float b_;
			static std::vector<float> w_;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};

	}
}

#pragma warning(pop)

#endif // MOCK_PERSON_CLASSIFIER_H_