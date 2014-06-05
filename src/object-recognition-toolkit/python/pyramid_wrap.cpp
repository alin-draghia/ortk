#include <boost/python.hpp>

#include "object-recognition-toolkit/object_recognition_toolkit.h"


namespace object_recognition_toolkit
{
	namespace pyramid
	{

		namespace bp = boost::python;

		struct PyramidBuilder_Wrapper
			: PyramidBuilder
			, bp::wrapper < PyramidBuilder >
		{

			Pyramid Build(const core::Matrix& image) const
			{
				return this->get_override("Build")(image);
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

		PyramidBuilder* create_FloatPyramidBuilder(double scaleFactor, cv::Size minSize, cv::Size maxSize)
		{
			return new FloatPyramidBuilder(scaleFactor, minSize, maxSize);
		}
	}
}

#include "object-recognition-toolkit/python/python_ext.h"

void py_regiser_pyramid()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit;
	using namespace object_recognition_toolkit::pyramid;
	using object_recognition_toolkit::python_ext::serialize_pickle;

	class_<PyramidLevel>("PyramidLevel", init<const core::Matrix&, double>())
		.def("GetScale", &PyramidLevel::GetScale)
		.def("GetImage", &PyramidLevel::GetImage, return_value_policy<copy_const_reference>())
		.def("Transform", &PyramidLevel::Transform<int>)
		.def("Invert", &PyramidLevel::Invert<int>);

	class_<Pyramid>("Pyramid")
		.def("AddLevel", &Pyramid::AddLevel)
		.def("Clear", &Pyramid::Clear)
		.def("GetNumLevels", &Pyramid::GetNumLevels)
		.def("GetLevel", &Pyramid::GetLevel, return_value_policy<copy_const_reference>())
		;

	class_<PyramidBuilder_Wrapper, boost::noncopyable>("PyramidBuilder")
		.def("Build", pure_virtual(&PyramidBuilder::Build))
		.def("Name", pure_virtual(&PyramidBuilder::name), return_value_policy<copy_const_reference>())
		.def("Clone", pure_virtual(&PyramidBuilder::Clone), return_value_policy<manage_new_object>())
		.def_pickle(serialize_pickle<PyramidBuilder>());

	def("create_FloatPyramidBuilder", create_FloatPyramidBuilder, return_value_policy<manage_new_object>());

}