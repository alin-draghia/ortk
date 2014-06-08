#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"

namespace object_recognition_toolkit
{
	namespace classification
	{


		namespace bp = boost::python;

		struct Classifier_Wrapper
			: Classifier
			, bp::wrapper < Classifier >
		{

			double Predict(const cv::Mat& instance) const
			{
				return this->get_override("Predict")(instance);
			}

		};

		struct Trainer_Wrapper
			: Trainer
			, bp::wrapper<Trainer>
		{
			Classifier* Train(const cv::Mat& features, const cv::Mat& labels)
			{
				return this->get_override("Train")();
			}
		};



		std::auto_ptr<LinearSVC> create_LinearSVC(float bias, std::vector<float>& coefs)
		{
			std::auto_ptr<LinearSVC> ptr(new LinearSVC(bias, coefs));
			return ptr;
		}

		Classifier* create_MockPersonClassifier()
		{
			return new MockPersonClassifier();
		}


		Trainer* create_LinearSvcTrainer()
		{
			return new LinearSvcTrainer();
		}
	}
}

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


void py_regiser_classification()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::core;
	using namespace object_recognition_toolkit::classification;

	using object_recognition_toolkit::python_ext::serialize_pickle;

	class_<Classifier_Wrapper, Classifier*, bases<Named, Clonable>>("Classifier")
		.def("Predict", &Classifier::Predict)
		.def_pickle(serialize_pickle<Classifier*>())
		;

	class_<MockPersonClassifier, MockPersonClassifier*, bases<Classifier>>("MockPersonClassifier")
		.def_pickle(serialize_pickle<MockPersonClassifier*>())
		;

	class_<LinearSVC, LinearSVC*, bases<Classifier>>("LinearSVC")
		.def("__init__", make_constructor(create_LinearSVC))
		.def_pickle(serialize_pickle<LinearSVC*>())
		;



	class_<Trainer_Wrapper, boost::noncopyable>("Trainer")
		.def("Train", pure_virtual(&Trainer::Train), return_value_policy<manage_new_object>())
		;

}