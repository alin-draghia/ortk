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
			double Predict(cv::Mat const& x) const
			{
				return this->get_override("Predict")(x);
			}

			core::Matrix PredictMulti(core::Matrix const& X) const
			{
				return this->get_override("PredictMulti")(X);
			}

			boost::shared_ptr<Classifier> Clone() const
			{
				return this->get_override("Clone")();
			}
		};

		struct Trainer_Wrapper
			: Trainer
			, bp::wrapper < Trainer >
		{
			boost::shared_ptr<Classifier> Train(cv::Mat const& X, cv::Mat const& y)
			{
				return this->get_override("Train")(X, y);
			}

			boost::shared_ptr<Trainer> Clone() const
			{
				return this->get_override("Clone")();
			}

		};
	}
}

void py_regiser_classification()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::core;
	using namespace object_recognition_toolkit::classification;

	using object_recognition_toolkit::python_ext::serialize_pickle;

	{
		class_<Classifier_Wrapper, boost::noncopyable>("Classifier")
			.def("Clone", pure_virtual(&Classifier::Clone))
			.def("Predict", pure_virtual(&Classifier::Predict), args("x"))
			.def("PredictMulti", pure_virtual(&Classifier::PredictMulti), args("X"))
			.enable_pickling()
			;
		register_ptr_to_python<boost::shared_ptr<Classifier>>();
	}

	{
		class_<MockPersonClassifier, bases<Classifier>>("MockPersonClassifier", init<>())
			.def_pickle(serialize_pickle<MockPersonClassifier>())
			;
	}

	{
		class_<LinearClassifier, bases<Classifier>>("LinearClassifier", init<>())
			.def(init<float, std::vector<float>&>(args("bias", "weights")))
			.def_readwrite("bias", &LinearClassifier::b_)
			.def_readwrite("weights", &LinearClassifier::w_)
			.def_pickle(serialize_pickle<LinearClassifier>())
			;
	}

	{
		class_<Trainer_Wrapper, boost::noncopyable>("Trainer")
			.def("Clone", pure_virtual(&Trainer::Clone))
			.def("Train", pure_virtual(&Trainer::Train), args("X", "y"))
			.enable_pickling()
			;
		register_ptr_to_python<boost::shared_ptr<Trainer>>();
	}

	{
		class_<LinearSvcTrainer, bases<Trainer>>("LinearSvcTrainer", init<>())
			.add_property("C", &LinearSvcTrainer::getC, &LinearSvcTrainer::setC)
			.def_pickle(serialize_pickle<LinearSvcTrainer>())
			;
	}

}