#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{
		namespace bp = boost::python;

		struct NonMaximaSuppressor_Wrapper
			: NonMaximaSuppressor
			, bp::wrapper < NonMaximaSuppressor >
		{

			boost::shared_ptr<NonMaximaSuppressor> Clone() const
			{
				return this->get_override("Clone")();
			}

			void suppress(std::vector<core::Box>& detections, std::vector<double>& confidences) const
			{
				this->get_override("suppress")(detections, confidences);
			}

		};

		boost::shared_ptr<NonMaximaSuppressor> create_PassThroughNms()
		{
			return boost::shared_ptr<NonMaximaSuppressor>(new PassThroughNms());
		}

		boost::shared_ptr<NonMaximaSuppressor> create_GroupRectanglesNms()
		{
			return boost::shared_ptr<NonMaximaSuppressor>(new GroupRectanglesNms());
		}
	}
}

#include "object-recognition-toolkit/python/python_ext.h"

void py_regiser_non_maxima_suppression()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::core;
	using namespace object_recognition_toolkit::non_maxima_suppression;
	using object_recognition_toolkit::python_ext::serialize_pickle;

	{
		typedef std::vector<Box> BoxVector;
		class_<BoxVector>("BoxVector")
			.def(vector_indexing_suite<BoxVector>())
			;
	}
	{
		class_<NonMaximaSuppressor_Wrapper, boost::noncopyable>("NonMaximaSuppressor")
			.def("Clone", pure_virtual(&NonMaximaSuppressor::Clone))
			.def("suppress", pure_virtual(&NonMaximaSuppressor::suppress), args("detections", "confidences"))
			.enable_pickling()
			;
		register_ptr_to_python<boost::shared_ptr<NonMaximaSuppressor>>();
	}
	{
		class_<PassThroughNms, bases<NonMaximaSuppressor>>("PassThroughNms", init<>())
			.def_pickle(serialize_pickle<PassThroughNms>())
			;
	}

	{
		class_<GroupRectanglesNms, bases<NonMaximaSuppressor>>("GroupRectanglesNms", init<>())
			.def_pickle(serialize_pickle<GroupRectanglesNms>())
			;
	}
	
}