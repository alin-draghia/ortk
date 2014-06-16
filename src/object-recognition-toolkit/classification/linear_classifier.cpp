#include "precomp.h"
#include "object-recognition-toolkit/classification/linear_classifier.h"

#include <boost/serialization/vector.hpp>

namespace object_recognition_toolkit
{
	namespace classification
	{

		LinearClassifier::LinearClassifier(const float b, std::vector<float>& w)
			: b_{ b }
			, w_{ w }
		{
		}

		LinearClassifier::~LinearClassifier()
		{
		}

		double LinearClassifier::Predict(const core::FeatureVector& x) const
		{
			if (x.cols != w_.size()) {
				throw std::runtime_error("Invalid instance vector size");
			}

			double conf = b_;

			for (int i = 0; i < static_cast<int>(w_.size()); i++) {
				conf += w_[i] * x.at<float>(i);
			}

			return conf;
		}

		void LinearClassifier::PredictMulti(core::Matrix const& X, core::Matrix& y) const
		{
			y.create(X.rows, 1, CV_64F);

			#pragma omp parallel for
			for (int i = 0; i < X.rows; i++) {
				y.at<double>(i) = this->Predict(X.row(i));
			}
		}

		boost::shared_ptr<Classifier> LinearClassifier::Clone() const
		{
			return boost::shared_ptr<Classifier>(new LinearClassifier(*this));
		}

		void LinearClassifier::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> b_;
			ar >> w_;
			ar >> boost::serialization::base_object<Classifier>(*this);
		}

		void LinearClassifier::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << b_;
			ar << w_;
			ar << boost::serialization::base_object<Classifier>(*this);
		}

	}
}