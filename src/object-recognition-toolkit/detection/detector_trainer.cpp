#include "object-recognition-toolkit/detection/detector_trainer.h"

namespace object_recognition_toolkit
{
	namespace detection
	{

		DetectorTrainer::~DetectorTrainer()
		{
		}

		void DetectorTrainer::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

		void DetectorTrainer::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

	}
}