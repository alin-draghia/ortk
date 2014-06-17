#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"

#include <boost/python/scope.hpp>

extern void py_regiser_classification();
extern void py_regiser_converters();
extern void py_regiser_core();
extern void py_regiser_dataset();
extern void py_regiser_detection();
extern void py_regiser_feature_extraction();
extern void py_regiser_image_scanning();
extern void py_regiser_non_maxima_suppression();
extern void py_regiser_pyramid();

class core_scope {};
class dataset_scope {};
class classification_scope {};
class detection_scope {};
class feature_extraction_scope {};
class image_scanning_scope {};
class non_maxima_suppression_scope {};
class pyramid_scope {};

#if defined(_DEBUG)
BOOST_PYTHON_MODULE(object_recognition_toolkitd_ext)
#else
BOOST_PYTHON_MODULE(object_recognition_toolkit_ext)
#endif
{
	namespace bp = boost::python;

	py_regiser_converters();

	{
		//bp::scope core = bp::class_<core_scope>("core");
		py_regiser_core();
	}

	{
		//bp::scope dataset = bp::class_<dataset_scope>("dataset");
		py_regiser_dataset();
	}

	{
		//bp::scope classification = bp::class_<classification_scope>("classification");
		py_regiser_classification();
	}

	{
		//bp::scope detection = bp::class_<detection_scope>("detection");
		py_regiser_detection();
	}

	{
		//bp::scope feature_extraction = bp::class_<feature_extraction_scope>("feature_extraction");
		py_regiser_feature_extraction();
	}

	{
		//bp::scope image_scanning = bp::class_<image_scanning_scope>("image_scanning");
		py_regiser_image_scanning();
	}

	{
		//bp::scope non_maxima_suppression = bp::class_<non_maxima_suppression_scope>("non_maxima_suppression");
		py_regiser_non_maxima_suppression();
	}

	{
		//bp::scope pyramid = bp::class_<pyramid_scope>("pyramid");
		py_regiser_pyramid();
	}
}