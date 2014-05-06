#pragma once
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <string>
#include <vector>

#include "../core/public-api.h"
#include "../core/algorithm.h"

namespace object_recognition_toolkit
{
	namespace classification
	{
		class PUBLIC_API Classifier
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			Classifier();
			Classifier(const std::string& name);
			virtual ~Classifier();

			virtual double PredictConf(const std::vector<float>& instance) const;

		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};
	}
}

#pragma warning(pop)

#endif // CLASSIFIER_H_