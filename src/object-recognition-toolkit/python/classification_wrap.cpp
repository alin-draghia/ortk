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
			double Predict(const cv::Mat& instance) const {
				return this->get_override("Predict")(instance);
			}
		};

		struct Trainer_Wrapper
			: Trainer
			, bp::wrapper<Trainer>
		{
			std::auto_ptr<Classifier> Train(const cv::Mat& features, const cv::Mat& labels) {
				std::auto_ptr<Classifier> aptr = this->get_override("Train")();
				return aptr;
			}
		};



		std::auto_ptr<LinearClassifier> create_LinearSVC(float bias, std::vector<float>& coefs)
		{
			std::auto_ptr<LinearClassifier> ptr(new LinearClassifier(bias, coefs));
			return ptr;
		}

	}
}

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

	class_<LinearClassifier, LinearClassifier*, bases<Classifier>>("LinearClassifier")
		.def("__init__", make_constructor(create_LinearSVC))
		.def_pickle(serialize_pickle<LinearClassifier*>())
		;

	class_<Trainer_Wrapper, Trainer*, bases<Named, Clonable>>("Trainer")
		.def("Train", &Trainer::Train)
		.def_pickle(serialize_pickle<Trainer*>())
		;

}