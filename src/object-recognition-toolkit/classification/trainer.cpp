#include "precomp.h"
#include "object-recognition-toolkit\classification\trainer.h"

namespace object_recognition_toolkit
{
	namespace classification
	{

		Trainer::~Trainer()
		{
		}


		boost::shared_ptr<Trainer> Trainer::Clone() const
		{
			throw core::NotImplementedError();
		}


		boost::shared_ptr<Classifier> Trainer::Train(const cv::Mat& X, const cv::Mat& y)
		{
			throw core::NotImplementedError();
		}

		void Trainer::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}


		void Trainer::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
	}
}