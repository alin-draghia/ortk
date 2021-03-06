#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"
#include <boost/make_shared.hpp>

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		namespace bp = boost::python;

		struct PyramidBuilder_Wrapper
			: public PyramidBuilder
			, public bp::wrapper < PyramidBuilder >
		{		
			virtual ~PyramidBuilder_Wrapper() {}

			Pyramid Build(core::Matrix const& image) const {
				return this->get_override("Build")(image);
			}

			boost::shared_ptr<PyramidBuilder> Clone() const {
				return this->get_override("Clone")();
			}
		};
	}
}



#include "object-recognition-toolkit/python/python_ext.h"


void py_regiser_pyramid()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::core;
	using namespace object_recognition_toolkit::pyramid;
	using object_recognition_toolkit::python_ext::serialize_pickle;

	class_<PyramidLevel>("PyramidLevel", 
			init<const Matrix&, double>((arg("image"),arg("scale"))))
		.def("GetScale", &PyramidLevel::GetScale)
		.def("GetImage", &PyramidLevel::GetImage, 
			return_value_policy<copy_const_reference>())
		.def("Transform", &PyramidLevel::Transform<int>, arg("box"))
		.def("Invert", &PyramidLevel::Invert<int>, arg("box"))
		;

	class_<Pyramid>("Pyramid", init<>())
		.def("AddLevel", &Pyramid::AddLevel, arg("level"))
		.def("Clear", &Pyramid::Clear)
		.def("GetNumLevels", &Pyramid::GetNumLevels)
		.def("GetLevel", &Pyramid::GetLevel, arg("index"), 
			return_value_policy<copy_const_reference>())
		;
	
	class_<PyramidBuilder_Wrapper, boost::noncopyable>("PyramidBuilder")
		.def("Clone", pure_virtual(&PyramidBuilder::Clone))
		.def("Build", pure_virtual(&PyramidBuilder::Build), arg("image"))
		.enable_pickling()
		;
	register_ptr_to_python<boost::shared_ptr<PyramidBuilder>>();

	class_<FloatPyramidBuilder, bases<PyramidBuilder>>("FloatPyramidBuilder", init<>())
		.def(init<double, Size, Size>(args("scale_factor", "min_size", "max_size")))
		.def_readwrite("scale_factor", &FloatPyramidBuilder::scaleFactor_)
		.def_readwrite("max_size", &FloatPyramidBuilder::maxSize_)
		.def_readwrite("min_size", &FloatPyramidBuilder::minSize_)
		.def_pickle(serialize_pickle<FloatPyramidBuilder>())
		;		
}


