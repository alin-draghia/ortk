#include "precomp.h"
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

		Detector* DetectorTrainer::TrainWithDataset(const dataset::Dataset& positive, const dataset::Dataset& negative)
		{
			throw std::runtime_error("not implemented");
		}

		Detector* DetectorTrainer::TrainWithImages(const std::vector<core::Matrix>& positiveImages, const std::vector<core::Matrix>& negativeImages)
		{
			throw std::runtime_error("not implemented");
		}

	}
}