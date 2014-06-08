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
			LinearSVC() = default;
			LinearSVC(float bias, std::vector<float>& coefs);
			virtual ~LinearSVC();

			virtual double Predict(const core::FeatureVector& instance) const;

		public:
			virtual const std::string& name() const;

		public:
			virtual core::Clonable* Clone();

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);

		private:
			float bias_;
			std::vector<float> coefs_;
		};

	}
}

#pragma warning(pop)

#endif // LINEAR_SVC_H_