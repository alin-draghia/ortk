#include "object-recognition-toolkit/python/python_ext.h"

#include <boost/python.hpp>

#include "object-recognition-toolkit/object_recognition_toolkit.h"



namespace object_recognition_toolkit
{
	namespace core
	{

		namespace bp = boost::python;

		struct NamedWrap
			: Named
			, bp::wrapper<Named>
		{
			const std::string& name() const
			{
				return this->get_override("name")();
			}
		};

		Named* test_factory() {
			return new feature_extraction::HogFeatureExtractor();
		}
	}
}

namespace object_recognition_toolkit
{
	namespace detection
	{
		namespace bp = boost::python;

		struct DetectorWrap 
			: Detector
			, bp::wrapper < Detector >
		{
			void Detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold = 0.0) const
			{
				this->get_override("Detect")(image, detections, confidences, treshold);
			}
		};
	}
}



BOOST_PYTHON_MODULE(object_recognition_toolkit) {
	using namespace boost::python;

	object_recognition_toolkit::python::initialize_converters();

	using namespace object_recognition_toolkit;
	

	class_<core::NamedWrap, boost::noncopyable>("Named")
		.def("name", pure_virtual(&core::Named::name), return_value_policy<copy_const_reference>())
		;

	def("test_factory", core::test_factory, return_value_policy<manage_new_object>());

}