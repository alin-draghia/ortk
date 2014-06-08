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
			: public core::Named
			, public core::Clonable
		{
		public:
			virtual ~Trainer();
			virtual Classifier* Train(const cv::Mat& features, const cv::Mat& labels);

			DECLARE_SERIALIZABLE();
		};
	}
}

#pragma warning(pop)

#endif // TRAINER_H