#include <boost/python.hpp>

#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"


namespace object_recognition_toolkit
{
	namespace core
	{

		namespace bp = boost::python;

		struct Named_Wrapper
			: Named
			, bp::wrapper<Named>
		{
			const std::string& name() const
			{
				return this->get_override("name")();
			}
		};


		struct Clonable_Wrapper
			: Clonable
			, bp::wrapper<Clonable>
		{
			Clonable* Clone()
			{
				return this->get_override("Clone")();
			}
		};

		Named* test_factory() {
			return new feature_extraction::HogFeatureExtractor();
		}

	}
}

void py_regiser_core()
{
	using namespace boost::python;
	using object_recognition_toolkit::core::Named;
	using object_recognition_toolkit::core::Named_Wrapper;
	using object_recognition_toolkit::core::Clonable;
	using object_recognition_toolkit::core::Clonable_Wrapper;

	class_<Named_Wrapper, boost::noncopyable>("Named")
		.def("name", pure_virtual(&Named::name), return_value_policy<copy_const_reference>())
		;

	class_<Clonable_Wrapper, boost::noncopyable>("Clonable")
		.def("Clone", pure_virtual(&Clonable::Clone), return_value_policy<manage_new_object>())
		;

	def("test_factory", object_recognition_toolkit::core::test_factory, return_value_policy<manage_new_object>());
}


