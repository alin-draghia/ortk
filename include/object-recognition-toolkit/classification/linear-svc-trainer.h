#pragma once
#ifndef LINEAR_SVC_TRAINER_H_
#define LINEAR_SVC_TRAINER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

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
			class LinearSvcClassifier : public
				object_recognition_toolkit::classification::Classifier
			{
			public:
				LinearSvcClassifier();
				virtual ~LinearSvcClassifier();

				virtual double PredictConf(const std::vector<float>& instance) const;

				cv::SVM svm_;

			private:
				friend class boost::serialization::access;
				template<class Archive>
				void save(Archive & ar, const unsigned int version) const;
				template<class Archive>
				void load(Archive & ar, const unsigned int version);
				BOOST_SERIALIZATION_SPLIT_MEMBER()
			};
		};
	}
}

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY(object_recognition_toolkit::classification::LinearSvcTrainer::LinearSvcClassifier);
	

#pragma warning(pop)


#endif // LINEAR_SVC_TRAINER_H_