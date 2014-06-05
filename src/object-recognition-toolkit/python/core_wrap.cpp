#include <boost/python.hpp>

#include "object-recognition-toolkit/object_recognition_toolkit.h"

namespace {
	/*
	core::Box -> tuple
	*/
	template<typename T>
	struct rect_to_tuple
	{
		static PyObject* convert(cv::Rect_<T> const& r)
		{
			return boost::python::incref(
				boost::python::make_tuple(r.x, r.y, r.width, r.height).ptr()
				);
		}
		static PyTypeObject const* get_pytype()
		{
			return &PyType_Type;
		}
	};

	/*
	tuple -> core::Box
	*/
	template<typename T>
	struct tuple_to_rect
	{

		tuple_to_rect()
		{
			boost::python::converter::registry::push_back(
				&convertible,
				&construct,
				boost::python::type_id< cv::Rect_<T> >());
		}


		static void* convertible(PyObject* obj_ptr)
		{
			boost::python::extract<const boost::python::tuple&> ex(obj_ptr);
			if (!ex.check())
				return nullptr;

			auto tup = ex();

			if (boost::python::len(tup) != 4)
				return nullptr;

			return obj_ptr;
		}

		static void construct(
			PyObject* obj_ptr,
			boost::python::converter::rvalue_from_python_stage1_data* data)
		{
			void* storage = (
				(boost::python::converter::rvalue_from_python_storage<cv::Rect_<T>>*)data)->storage.bytes;

			const boost::python::tuple& tup = boost::python::extract<const boost::python::tuple&>(obj_ptr);
			T x = boost::python::extract<T>(tup[0]);
			T y = boost::python::extract<T>(tup[1]);
			T w = boost::python::extract<T>(tup[2]);
			T h = boost::python::extract<T>(tup[3]);

			// calling inplace constructor
			new (storage)cv::Rect_<T>(x, y, w, h);

			data->convertible = storage;
		}
	};


	/*
	core::Size -> tuple
	*/
	template<typename T>
	struct size_to_tuple
	{
		static PyObject* convert(cv::Size_<T> const& s)
		{
			return boost::python::incref(
				boost::python::make_tuple(s.width, s.height).ptr()
				);
		}
		static PyTypeObject const* get_pytype()
		{
			return &PyType_Type;
		}
	};

	/*
	tuple -> core:Size
	*/
	template<typename T>
	struct tuple_to_size
	{

		tuple_to_size()
		{
			boost::python::converter::registry::push_back(
				&convertible,
				&construct,
				boost::python::type_id< cv::Size_<T> >());
		}


		static void* convertible(PyObject* obj_ptr)
		{
			boost::python::extract<const boost::python::tuple&> ex(obj_ptr);
			if (!ex.check())
				return nullptr;

			auto tup = ex();
			if (boost::python::len(tup) != 2)
				return nullptr;

			return obj_ptr;
		}

		static void construct(
			PyObject* obj_ptr,
			boost::python::converter::rvalue_from_python_stage1_data* data)
		{
			void* storage = (
				(boost::python::converter::rvalue_from_python_storage<cv::Size_<T>>*)data)->storage.bytes;

			const boost::python::tuple& tup = boost::python::extract<const boost::python::tuple&>(obj_ptr);

			T w = boost::python::extract<T>(tup[0]);
			T h = boost::python::extract<T>(tup[1]);

			// calling inplace constructor
			new (storage)cv::Size_<T>(w, h);

			data->convertible = storage;
		}
	};
}

namespace object_recognition_toolkit
{
	namespace core
	{
		namespace bp = boost::python;


		struct Named_Wrapper
			: Named
			, bp::wrapper < Named >
		{
			const std::string& name() const
			{
				return this->get_override("name")();
			}
		};


		struct Clonable_Wrapper
			: Clonable
			, bp::wrapper < Clonable >
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


	to_python_converter<cv::Rect_<int>, ::rect_to_tuple<int>, true>();
	to_python_converter<cv::Rect_<float>, ::rect_to_tuple<float>, true>();
	to_python_converter<cv::Rect_<double>, ::rect_to_tuple<double>, true>();
	to_python_converter<cv::Size_<int>, ::size_to_tuple<int>, true>();
	to_python_converter<cv::Size_<float>, ::size_to_tuple<float>, true>();
	to_python_converter<cv::Size_<double>, ::size_to_tuple<double>, true>();

	::tuple_to_rect<int>();
	::tuple_to_rect<float>();
	::tuple_to_rect<double>();
	::tuple_to_size<int>();
	::tuple_to_size<float>();
	::tuple_to_size<double>();


	class_<Named_Wrapper, boost::noncopyable>("Named")
		.def("name", pure_virtual(&Named::name), return_value_policy<copy_const_reference>())
		;

	class_<Clonable_Wrapper, boost::noncopyable>("Clonable")
		.def("Clone", pure_virtual(&Clonable::Clone), return_value_policy<manage_new_object>())
		;

	def("test_factory", object_recognition_toolkit::core::test_factory, return_value_policy<manage_new_object>());
}


