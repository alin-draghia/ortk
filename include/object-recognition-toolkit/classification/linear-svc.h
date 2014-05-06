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

		class PUBLIC_API LinearSVC
			: public Classifier
		{
		public:
			LinearSVC(const float bias, std::vector<float>& coefs);
			virtual ~LinearSVC();

			virtual double PredictConf(const std::vector<float>& instance) const;

		private:
			float bias_;
			std::vector<float> coefs_;
		};

	}
}

#pragma warning(pop)

#endif // LINEAR_SVC_H_