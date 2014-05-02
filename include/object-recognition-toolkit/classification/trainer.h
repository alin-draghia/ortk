#pragma once
#ifndef TRAINER_H_
#define TRAINER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <opencv2/opencv.hpp>

#include <object-recognition-toolkit/core/core.h>
#include <object-recognition-toolkit/classification/classifier.h>

namespace object_recognition_toolkit
{
	namespace classification
	{
		class ORT_API Trainer
		{
		public:
			virtual ~Trainer();
			virtual Classifier* Train(const cv::Mat& features, const cv::Mat& labels) = 0;
		};
	}
}

#pragma warning(pop)

#endif // TRAINER_H