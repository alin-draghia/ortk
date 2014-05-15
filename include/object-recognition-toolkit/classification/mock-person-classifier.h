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

			virtual double PredictConf(const std::vector<float>& instance) const;

		public:
			virtual const std::string& name() const;

		private:
			static float b_;
			static std::vector<float> w_;

		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};

	}
}

#pragma warning(pop)

#endif // MOCK_PERSON_CLASSIFIER_H_