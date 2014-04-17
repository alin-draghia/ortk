#pragma once
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <string>
#include <vector>

#include <object-recognition-toolkit/core/algorithm.h>

namespace object_recognition_toolkit
{
	namespace classification
	{
		class ORT_API Classifier
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			Classifier(const std::string& name);
			virtual ~Classifier();

			virtual double PredictConf(const std::vector<float>& instance) const = 0;
		};
	}
}

#endif // CLASSIFIER_H_