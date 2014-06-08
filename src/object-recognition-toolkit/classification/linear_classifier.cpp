#include "precomp.h"
#include "object-recognition-toolkit/classification/linear_classifier.h"

#include <boost/serialization/vector.hpp>

namespace object_recognition_toolkit
{
	namespace classification
	{

		LinearClassifier::LinearClassifier(const float bias, std::vector<float>& coefs)
			: bias_{ bias }
			, coefs_{ coefs }
		{
		}

		LinearClassifier::~LinearClassifier()
		{
		}

		const std::string& LinearClassifier::name() const
		{
			static const std::string name = "LinearClassifier";
			return name;
		}

		double LinearClassifier::Predict(const core::FeatureVector& instance) const
		{
			if (instance.cols != coefs_.size()) {
				throw std::runtime_error("Invalid instance vector size");
			}

			double conf = bias_;

			for (int i = 0; i < static_cast<int>(coefs_.size()); i++) {
				conf += coefs_[i] * instance.at<float>(i);
			}

			return conf;
		}

		core::Clonable* LinearClassifier::Clone()
		{
			return new LinearClassifier(*this);
		}

		void LinearClassifier::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> bias_;
			ar >> coefs_;
			ar >> boost::serialization::base_object<Classifier>(*this);
		}

		void LinearClassifier::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << bias_;
			ar << coefs_;
			ar << boost::serialization::base_object<Classifier>(*this);
		}

	}
}