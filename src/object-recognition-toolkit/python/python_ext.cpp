#include <boost/python.hpp>

#include "object-recognition-toolkit/object_recognition_toolkit.h"


extern void py_regiser_converters();
extern void py_regiser_core();
extern void py_regiser_classification();
extern void py_regiser_feature_extraction();



BOOST_PYTHON_MODULE(object_recognition_toolkit) {


	py_regiser_converters();
	py_regiser_core();
	py_regiser_classification();
	py_regiser_feature_extraction();

}