#pragma once
#ifndef MOCK_PERSON_CLASSIFIER_H_
#define MOCK_PERSON_CLASSIFIER_H_

#include <object-recognition-toolkit/classification/classifier.h>


namespace object_recognition_toolkit
{
	namespace classification
	{

		class ORT_API MockPersonClassifier
			: public Classifier
		{
		public:
			MockPersonClassifier();
			virtual ~MockPersonClassifier();

			virtual double PredictConf(const std::vector<float>& instance) const;

		private:
			static float b_;
			static std::vector<float> w_;
		};

	}
}

#endif // MOCK_PERSON_CLASSIFIER_H_