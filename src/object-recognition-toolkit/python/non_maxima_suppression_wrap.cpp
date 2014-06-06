#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "object-recognition-toolkit/object_recognition_toolkit.h"



namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{
		namespace bp = boost::python;

		struct NonMaximaSuppressor_Wrapper
			: NonMaximaSuppressor
			, bp::wrapper<NonMaximaSuppressor>
		{

			void suppress(std::vector<core::Box>& detections, std::vector<double>& confidences) const
			{
				this->get_override("suppress")(detections, confidences);
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

		std::auto_ptr<NonMaximaSuppressor> create_PassThroughNms()
		{
			return std::auto_ptr<NonMaximaSuppressor>(new PassThroughNms());
		}

		std::auto_ptr<NonMaximaSuppressor> create_GroupRectanglesNms()
		{
			return std::auto_ptr<NonMaximaSuppressor>(new GroupRectanglesNms());
		}
	}
}

#include "object-recognition-toolkit/python/python_ext.h"

void py_regiser_non_maxima_suppression()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit;
	using namespace object_recognition_toolkit::non_maxima_suppression;
	using object_recognition_toolkit::python_ext::serialize_pickle;

	typedef std::vector<core::Box> BoxVector;
	class_<BoxVector>("BoxVector")
		.def(vector_indexing_suite<BoxVector>())
		;

	typedef std::vector<double> DoubleVector;
	class_<DoubleVector>("DoubleVector")
		.def(vector_indexing_suite<DoubleVector>())
		;

	class_<NonMaximaSuppressor_Wrapper, boost::noncopyable>("NonMaximaSuppressor")
		.def("suppress", pure_virtual(&NonMaximaSuppressor::suppress))
		.def("Name", pure_virtual(&NonMaximaSuppressor::name), return_value_policy<copy_const_reference>())
		.def("Clone", pure_virtual(&NonMaximaSuppressor::Clone), return_value_policy<manage_new_object>())
		.def_pickle(serialize_pickle<NonMaximaSuppressor>());

	class_<GroupRectanglesNms, bases<NonMaximaSuppressor>>("GroupRectanglesNms")
		;

	def("create_PassThroughNms", create_PassThroughNms);
	def("create_GroupRectanglesNms", create_GroupRectanglesNms);

	register_ptr_to_python<std::auto_ptr<NonMaximaSuppressor>>();
}