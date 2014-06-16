#include "precomp.h"
#include "object-recognition-toolkit/detection/detector_trainer.h"

namespace object_recognition_toolkit
{
	namespace detection
	{

		DetectorTrainer::~DetectorTrainer()
		{
		}

		boost::shared_ptr<DetectorTrainer> DetectorTrainer::Clone() const
		{
			throw core::NotImplementedError();
		}

		boost::shared_ptr<Detector> DetectorTrainer::TrainWithDataset(const dataset::Dataset& positive, const dataset::Dataset& negative)
		{
			throw core::NotImplementedError();
		}

		boost::shared_ptr<Detector> DetectorTrainer::TrainWithImages(const std::vector<core::Matrix>& positiveImages, const std::vector<core::Matrix>& negativeImages)
		{
			throw core::NotImplementedError();
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