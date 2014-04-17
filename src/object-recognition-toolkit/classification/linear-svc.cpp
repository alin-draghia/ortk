#include <object-recognition-toolkit/classification/linear-svc.h>

namespace object_recognition_toolkit
{
	namespace classification
	{

		LinearSVC::LinearSVC(const float bias, std::vector<float>& coefs)
			: Classifier("LinearSVC")
			, bias_{ bias }
			, coefs_{ coefs }
		{
		}

		LinearSVC::~LinearSVC()
		{
		}

		double LinearSVC::PredictConf(const std::vector<float>& instance) const
		{
			if (instance.size() != coefs_.size()) {
				throw std::runtime_error("Invalid instance vector size");
			}

			double conf = bias_;

			for (size_t i = 0; i < coefs_.size(); i++) {
				conf += coefs_[i] * instance[i];
			}

			return conf;
		}

	}
}