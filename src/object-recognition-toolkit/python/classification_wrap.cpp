#include <boost/python.hpp>

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

			const std::string& name() const
			{
				return this->get_override("name")();
			}

			Clonable* Clone()
			{
				return this->get_override("Clone")();
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



		Classifier* create_LinearSVC(double bias, const cv::Mat_<float>& coefs)
		{
			std::vector<float> coefs_ = coefs;
			std::cout << "create_LinearSVC()" << std::endl;
			std::cout << "bias=" << bias << std::endl;
			std::cout << "coefs=";
			for (auto& val : coefs_)
				std::cout << val << ", ";
			
			std::cout << std::endl;

			return new LinearSVC(bias, coefs_);
		}

	}
}

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


void py_regiser_classification()
{
	using namespace boost::python;

	using namespace object_recognition_toolkit::classification;

	class_<Classifier_Wrapper, boost::noncopyable>("Classifier")
		.def("Predict", pure_virtual(&Classifier::Predict))
		;

	class_<Trainer_Wrapper, boost::noncopyable>("Trainer")
		.def("Train", pure_virtual(&Trainer::Train), return_value_policy<manage_new_object>())
		;

	def("create_LinearSVC", create_LinearSVC, return_value_policy<manage_new_object>());
	/*
	class_<LinearSVC, bases<Classifier>>("LinearSVC", no_init)
		.def("__init__", make_constructor(create_LinearSVC))
		;
		*/
		

}