#pragma once
#ifndef DETECTOR_TRAINER_H_INCLUDED_
#define DETECTOR_TRAINER_H_INCLUDED_

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/serialization.h"
#include "../dataset/dataset.h"
#include "./detector.h"

namespace object_recognition_toolkit
{
	namespace detection
	{
		class PUBLIC_API DetectorTrainer
			: public core::Named
			, public core::Clonable
		{
		public:
			virtual ~DetectorTrainer();

		public:
			virtual Detector* TrainWithDataset(const dataset::Dataset& positive, const dataset::Dataset& negative);
			virtual Detector* TrainWithImages(const std::vector<core::Matrix>& positiveImages, const std::vector<core::Matrix>& negativeImages);

		private:
			DECLARE_SERIALIZABLE();
		};
	}
}

#endif // DETECTOR_TRAINER_H_INCLUDED_