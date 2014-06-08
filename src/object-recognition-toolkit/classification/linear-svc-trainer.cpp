#include "precomp.h"
#include <memory>

#include <opencv2/opencv.hpp>

#include "object-recognition-toolkit/classification/linear-svc-trainer.h"




namespace object_recognition_toolkit {
	namespace classification {


		class LinearSvcClassifier__
			: public Classifier
		{
		public:
			LinearSvcClassifier__() = default;
			virtual ~LinearSvcClassifier__() = default;

			virtual double Predict(const core::FeatureVector& instance) const {
				return (double)svm_.predict(instance, true) * -1.0;
			}

			virtual const std::string& name() const {
				static const std::string name = "LinearSvcTrainer::LinearSvcClassifier";
				return name;
			}

			virtual core::Clonable* Clone() {
				std::stringstream ss; // clone via serialization
				core::oarchive oa(ss);
				oa << this;
				core::iarchive ia(ss);
				LinearSvcClassifier__* ptr;
				ia >> ptr;
				return ptr;
			}

			friend class LinearSvcTrainer;

			void serialize(core::iarchive& ar, const unsigned int version) {
				ar >> boost::serialization::base_object<Classifier>(*this);
				std::string svm_buffer;
				ar >> svm_buffer;
				cv::FileStorage fs(svm_buffer, cv::FileStorage::READ + cv::FileStorage::FORMAT_YAML + cv::FileStorage::MEMORY);
				svm_.read(*fs, *fs["my_svm"]);
			}

			void serialize(core::oarchive& ar, const unsigned int version) {
				ar << boost::serialization::base_object<Classifier>(*this);
				cv::FileStorage fs("", cv::FileStorage::WRITE + cv::FileStorage::FORMAT_YAML + cv::FileStorage::MEMORY);
				svm_.write(*fs, "my_svm");
				std::string svm_buffer = fs.releaseAndGetString();
				ar << svm_buffer;
			}

			cv::SVM svm_;
		};


		LinearSvcTrainer::LinearSvcTrainer()
			: C_(1.0)
		{
		}

		LinearSvcTrainer::LinearSvcTrainer(double C)
			: C_(C)
		{
		}

		LinearSvcTrainer::~LinearSvcTrainer()
		{
		}


		Classifier* LinearSvcTrainer::Train(const cv::Mat& features, const cv::Mat& labels)
		{

			std::unique_ptr<LinearSvcClassifier__> cls(new LinearSvcClassifier__());

			cv::SVMParams params;
			params.svm_type = cv::SVM::C_SVC;
			params.kernel_type = cv::SVM::LINEAR;
			params.C = this->getC();
			params.term_crit = cv::TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, 0.00001);

			bool train_ok = cls->svm_.train(features, labels, {}, {}, params);

			if (train_ok) {
				return cls.release();
			}

			throw std::runtime_error("training failed");
		}


		const std::string& LinearSvcTrainer::name() const
		{
			static const std::string name = "LinearSvcTrainer::LinearSvcTrainer";
			return name;
		}

		core::Clonable* LinearSvcTrainer::Clone()
		{
			return new LinearSvcTrainer(*this);
		}

		double LinearSvcTrainer::getC() const
		{
			return this->C_;
		}

		void LinearSvcTrainer::setC(double C)
		{
			this->C_ = C;
		}



	}
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::classification::LinearSvcClassifier__);

