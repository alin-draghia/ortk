#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "object-recognition-toolkit/object_recognition_toolkit.h"

namespace object_recognition_toolkit
{
	namespace image_scanning
	{
		namespace bp = boost::python;

		struct ImageScanner_Wrapper
			: ImageScanner
			, bp::wrapper < ImageScanner >
		{
			std::vector<Window> compute(const core::Matrix& image) const
			{
				return this->get_override("compute")();
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

		ImageScanner* create_DenseImageScanner(core::Size windowSize, core::Size windowStep, core::Size padding)
		{
			return new DenseImageScanner(windowSize, windowStep, padding);
		}
	}
}

#include "object-recognition-toolkit/python/python_ext.h"

void py_regiser_image_scanning()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit;
	using namespace object_recognition_toolkit::image_scanning;
	using object_recognition_toolkit::python_ext::serialize_pickle;

	class_<Window>("Window")
		.def_readwrite("image", &Window::image)
		.def_readwrite("box", &Window::box)
		;

	typedef std::vector<Window> WindowVector;
	class_<WindowVector>("WindowVector")
		.def(vector_indexing_suite<WindowVector>())
		;

	class_<ImageScanner_Wrapper, boost::noncopyable>("ImageScanner")
		.def("compute", pure_virtual(&ImageScanner::compute))
		.def("Name", pure_virtual(&ImageScanner::name), return_value_policy<copy_const_reference>())
		.def("Clone", pure_virtual(&ImageScanner::Clone), return_value_policy<manage_new_object>())
		.def_pickle(serialize_pickle<ImageScanner>());
	
	def("create_DenseImageScanner", create_DenseImageScanner, return_value_policy<manage_new_object>());
}