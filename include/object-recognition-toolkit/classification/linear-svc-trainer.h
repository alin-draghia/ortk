#pragma once
#ifndef LINEAR_SVC_TRAINER_H_
#define LINEAR_SVC_TRAINER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#define USE_DLIB 0

#if(USE_DLIB)
#include <dlib/svm.h>
#endif

#include "../core/public-api.h"
#include "./trainer.h"



namespace object_recognition_toolkit
{
	namespace classification
	{
		class PUBLIC_API LinearSvcTrainer
			: public Trainer
		{
		public:
			LinearSvcTrainer();
			LinearSvcTrainer(double C);

			virtual ~LinearSvcTrainer();

			virtual boost::shared_ptr<Classifier> Train(const cv::Mat& X, const cv::Mat& y);

		public:
			virtual boost::shared_ptr<Trainer> Clone() const;

		public:
			double getC() const;
			void setC(double C);


		private:
			double C_;
		};
	}
}


#pragma warning(pop)


#endif // LINEAR_SVC_TRAINER_H_