#pragma once
#ifndef LINEAR_SVC_TRAINER_H_
#define LINEAR_SVC_TRAINER_H_

#pragma warning(push)
#pragma warning(disable: 4251)



#include "trainer.h"

namespace object_recognition_toolkit
{
	namespace classification
	{
		class ORT_API LinearSvcTrainer
		{
		public:
			LinearSvcTrainer();
			virtual ~LinearSvcTrainer();

			virtual Classifier* Train(const cv::Mat& features, const cv::Mat& labels);

		public:
			class ORT_API LinearSvcClassifier : public
				object_recognition_toolkit::classification::Classifier
			{
			public:
				LinearSvcClassifier();
				virtual ~LinearSvcClassifier();

				virtual double PredictConf(const std::vector<float>& instance) const;

			private:
				friend class LinearSvcTrainer;
				cv::SVM svm_;

			private:
				friend class boost::serialization::access;
				void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
				void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
			};
		};
	}
}


#pragma warning(pop)


#endif // LINEAR_SVC_TRAINER_H_