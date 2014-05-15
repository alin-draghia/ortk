#pragma once
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <string>
#include <vector>



#include "../core/public-api.h"
#include "../core/named.h"
#include "../core/serialization.h"

namespace object_recognition_toolkit
{
	namespace classification
	{
		class PUBLIC_API Classifier
			: public core::Named
		{
		public:

			virtual ~Classifier() = 0;
			virtual double PredictConf(const std::vector<float>& instance) const = 0;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};
	}
}

#pragma warning(pop)

#endif // CLASSIFIER_H_