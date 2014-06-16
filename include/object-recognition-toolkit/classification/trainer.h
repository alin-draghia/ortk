#pragma once
#ifndef TRAINER_H_
#define TRAINER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <opencv2/opencv.hpp>

#include "../core/public-api.h"
#include "../core/serialization.h"
#include "./classifier.h"

namespace object_recognition_toolkit
{
	namespace classification
	{
		class PUBLIC_API Trainer
		{
		public:
			virtual ~Trainer();

			virtual boost::shared_ptr<Classifier> Train(cv::Mat const& X, cv::Mat const& y);

			virtual boost::shared_ptr<Trainer> Clone() const;

			DECLARE_SERIALIZABLE();
		};
	}
}

#pragma warning(pop)

#endif // TRAINER_H