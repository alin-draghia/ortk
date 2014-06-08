#include "precomp.h"
#include "object-recognition-toolkit\classification\trainer.h"

namespace object_recognition_toolkit
{
	namespace classification
	{

		Trainer::~Trainer()
		{
		}

		std::auto_ptr<Classifier> Trainer::Train(const cv::Mat& features, const cv::Mat& labels)
		{
			throw std::runtime_error("not implemented, [this is supposed to be a base class]");
		}
	}
}