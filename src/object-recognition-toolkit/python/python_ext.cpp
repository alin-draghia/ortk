#include <boost/python.hpp>

#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"



extern void py_regiser_core();
extern void py_regiser_classification();

BOOST_PYTHON_MODULE(object_recognition_toolkit) {
	using namespace boost::python;

	object_recognition_toolkit::python::initialize_converters();

	using namespace object_recognition_toolkit;

	py_regiser_core();
	py_regiser_classification();

}