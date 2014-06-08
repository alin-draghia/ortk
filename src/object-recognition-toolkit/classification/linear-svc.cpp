#include "object-recognition-toolkit/classification/linear-svc.h"

#include <boost/serialization/vector.hpp>

namespace object_recognition_toolkit
{
	namespace classification
	{

		LinearSVC::LinearSVC(const float bias, std::vector<float>& coefs)
			: bias_{ bias }
			, coefs_{ coefs }
		{
		}

		LinearSVC::~LinearSVC()
		{
		}

		const std::string& LinearSVC::name() const
		{
			static const std::string name = "LinearSVC";
			return name;
		}

		double LinearSVC::Predict(const core::FeatureVector& instance) const
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

		core::Clonable* LinearSVC::Clone()
		{
			return new LinearSVC(*this);
		}

		void LinearSVC::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> bias_;
			ar >> coefs_;
			ar >> boost::serialization::base_object<Classifier>(*this);
		}

		void LinearSVC::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << bias_;
			ar << coefs_;
			ar << boost::serialization::base_object<Classifier>(*this);
		}

	}
}