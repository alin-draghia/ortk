#include <memory>

#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/factory.hpp>
#include <opencv2/opencv.hpp>

#include "object-recognition-toolkit/classification/linear-svc-trainer.h"



namespace object_recognition_toolkit
{
	namespace classification
	{


		LinearSvcTrainer::LinearSvcTrainer()
		{

		}


		LinearSvcTrainer::~LinearSvcTrainer()
		{

		}


		Classifier* LinearSvcTrainer::Train(const cv::Mat& features, const cv::Mat& labels)
		{

			std::unique_ptr<LinearSvcClassifier> cls(new LinearSvcClassifier());

			cv::SVMParams params;
			params.svm_type = cv::SVM::C_SVC;
			params.kernel_type = cv::SVM::LINEAR;
			params.term_crit = cv::TermCriteria(CV_TERMCRIT_ITER, (int)1e7, 1e-6);

			bool train_ok = cls->svm_.train(features, labels, {}, {}, params);
			if (train_ok) {
				return cls.release();
			}

			return nullptr;
		}


		LinearSvcTrainer::LinearSvcClassifier::LinearSvcClassifier()
			: Classifier("LinearSvcTrainer::LinearSvcClassifier")
		{
		}

		LinearSvcTrainer::LinearSvcClassifier::~LinearSvcClassifier()
		{
		}

		double LinearSvcTrainer::LinearSvcClassifier::PredictConf(const std::vector<float>& instance) const
		{
			cv::Mat m(1, (int)instance.size(), CV_32F, (void*)instance.data());
			return (double)svm_.predict(m, true);
		}


		void LinearSvcTrainer::LinearSvcClassifier::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Classifier>(*this);
			std::string svm_buffer;
			ar >> svm_buffer;
			cv::FileStorage fs(svm_buffer, cv::FileStorage::READ + cv::FileStorage::FORMAT_YAML + cv::FileStorage::MEMORY);
			svm_.read(*fs, *fs["my_svm"]);
		}


		void LinearSvcTrainer::LinearSvcClassifier::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<Classifier>(*this);
			cv::FileStorage fs("", cv::FileStorage::WRITE + cv::FileStorage::FORMAT_YAML + cv::FileStorage::MEMORY);
			svm_.write(*fs, "my_svm");
			std::string svm_buffer = fs.releaseAndGetString();
			ar << svm_buffer;
		}


	}
}

BOOST_CLASS_EXPORT(object_recognition_toolkit::classification::LinearSvcTrainer::LinearSvcClassifier)
