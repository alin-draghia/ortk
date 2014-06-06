#include <boost/python.hpp>

#include "object-recognition-toolkit/object_recognition_toolkit.h"


extern void py_regiser_classification();
extern void py_regiser_converters();
extern void py_regiser_core();
extern void py_regiser_detection();
extern void py_regiser_feature_extraction();
extern void py_regiser_image_scanning();
extern void py_regiser_non_maxima_suppression();
extern void py_regiser_pyramid();

#if defined(_DEBUG)
BOOST_PYTHON_MODULE(object_recognition_toolkitd)
#else
BOOST_PYTHON_MODULE(object_recognition_toolkit)
#endif
{
	py_regiser_classification();
	py_regiser_converters();
	py_regiser_core();
	py_regiser_detection();
	py_regiser_feature_extraction();
	py_regiser_image_scanning();
	py_regiser_non_maxima_suppression();
	py_regiser_pyramid();

}