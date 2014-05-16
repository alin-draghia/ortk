#include <memory>

#include <opencv2/opencv.hpp>

#include "object-recognition-toolkit/classification/linear-svc-trainer.h"



namespace object_recognition_toolkit {
	namespace classification {


		LinearSvcTrainer::LinearSvcTrainer( )
		{

		}


		LinearSvcTrainer::~LinearSvcTrainer( )
		{

		}


		Classifier* LinearSvcTrainer::Train(const cv::Mat& features, const cv::Mat& labels)
		{

			std::unique_ptr<LinearSvcClassifier> cls(new LinearSvcClassifier( ));

#if(USE_DLIB)
#else



			cv::SVMParams params;
			params.svm_type = cv::SVM::C_SVC;
			params.kernel_type = cv::SVM::LINEAR;
			//params.C = 0.1;
			params.term_crit = cv::TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.00001);

			//bool train_ok = cls->svm_.train(features, labels, { }, { }, params);

			cv::ParamGrid C_grid;
			C_grid.min_val = 0.0001;
			C_grid.max_val = 1000.0;
			C_grid.step = 10.0;

			int k_fold = 5;
			bool train_ok = cls->svm_.train_auto(features, labels, { }, { }, params, k_fold, C_grid);

			cv::SVMParams actual_params = cls->svm_.get_params( );

			if ( train_ok ) {
				return cls.release( );
			}
#endif
			return nullptr;
		}


		LinearSvcTrainer::LinearSvcClassifier::LinearSvcClassifier( )
		{ 
		}

		LinearSvcTrainer::LinearSvcClassifier::~LinearSvcClassifier( )
		{ 
		}

		double LinearSvcTrainer::LinearSvcClassifier::Predict(const core::FeatureVector& instance) const
		{
			cv::Mat m(1, (int)instance.size( ), CV_32F, (void*)instance.data( ));
			return (double)svm_.predict(m, true) * -1.0;
		}


		void LinearSvcTrainer::LinearSvcClassifier::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Classifier>(*this);
			std::string svm_buffer;
			ar >> svm_buffer;
			cv::FileStorage fs(svm_buffer, cv::FileStorage::READ + cv::FileStorage::FORMAT_YAML + cv::FileStorage::MEMORY);
			svm_.read(*fs, *fs["my_svm"]);
		}


		void LinearSvcTrainer::LinearSvcClassifier::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<Classifier>(*this);
			cv::FileStorage fs("", cv::FileStorage::WRITE + cv::FileStorage::FORMAT_YAML + cv::FileStorage::MEMORY);
			svm_.write(*fs, "my_svm");
			std::string svm_buffer = fs.releaseAndGetString( );
			ar << svm_buffer;
		}

		const std::string& LinearSvcTrainer::LinearSvcClassifier::name() const
		{
			static const std::string name = "LinearSvcTrainer::LinearSvcClassifier";
			return name;
		}
	}
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::classification::LinearSvcTrainer::LinearSvcClassifier);

