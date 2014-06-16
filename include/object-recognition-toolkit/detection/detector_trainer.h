#pragma once
#ifndef DETECTOR_TRAINER_H_INCLUDED_
#define DETECTOR_TRAINER_H_INCLUDED_

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/serialization.h"
#include "../dataset/dataset.h"
#include "./detector.h"

#include <boost/shared_ptr.hpp>

namespace object_recognition_toolkit
{
	namespace detection
	{
		class PUBLIC_API DetectorTrainer
		{
		public:
			virtual ~DetectorTrainer();

			virtual boost::shared_ptr<DetectorTrainer> Clone() const;

		public:
			virtual boost::shared_ptr<Detector> TrainWithDataset(dataset::Dataset const& positive, dataset::Dataset const & negative);
			virtual boost::shared_ptr<Detector> TrainWithImages(std::vector<core::Matrix> const & positiveImages, std::vector<core::Matrix> const& negativeImages);

		private:
			DECLARE_SERIALIZABLE();
		};
	}
}

#endif // DETECTOR_TRAINER_H_INCLUDED_