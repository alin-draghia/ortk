#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
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
			return &PyTuple_Type;
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
			namespace bp = boost::python;
			bp::tuple tup(bp::handle<>(bp::borrowed(obj_ptr)));


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

			namespace bp = boost::python;
			bp::tuple tup(bp::handle<>(bp::borrowed(obj_ptr)));

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
			return &PyTuple_Type;
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
			namespace bp = boost::python;
			bp::tuple tup(bp::handle<>(bp::borrowed(obj_ptr)));

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

			namespace bp = boost::python;
			bp::tuple tup(bp::handle<>(bp::borrowed(obj_ptr)));

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

	}
}

template<class T>
class no_compare_indexing_suite
	: public boost::python::vector_indexing_suite < T, true, no_compare_indexing_suite<T> >
{
public:
	static bool contains(T &container, typename T::value_type const &key)
	{
		return false;
	}
};

	void py_regiser_core()
	{
		using namespace boost::python;
		using namespace object_recognition_toolkit::core;

#if(0)
		to_python_converter<cv::Rect_<int>, ::rect_to_tuple<int>, true>();
		to_python_converter<cv::Rect_<float>, ::rect_to_tuple<float>, true>();
		to_python_converter<cv::Rect_<double>, ::rect_to_tuple<double>, true>();
		to_python_converter<cv::Size_<int>, ::size_to_tuple<int>, true>();
		to_python_converter<cv::Size_<float>, ::size_to_tuple<float>, true>();
		to_python_converter<cv::Size_<double>, ::size_to_tuple<double>, true>();
#endif
#if(0)
		::tuple_to_rect<int>();
		::tuple_to_rect<float>();
		::tuple_to_rect<double>();
		::tuple_to_size<int>();
		::tuple_to_size<float>();
		::tuple_to_size<double>();
#endif

		typedef cv::Rect_<int> Rect;
		typedef cv::Size_<int> Size;

		class_<Rect>("Rect")
			.def_readwrite("x", &Rect::x)
			.def_readwrite("y", &Rect::y)
			.def_readwrite("width", &Rect::width)
			.def_readwrite("height", &Rect::height)
			;

		class_<Size>("Size")
			.def_readwrite("width", &Size::width)
			.def_readwrite("height", &Size::height)
			;


		typedef std::vector<int> VecI32;
		class_<VecI32>("VecI32")
			.def(vector_indexing_suite<VecI32>())
			;

		typedef std::vector<float> VecF32;
		class_<VecF32>("VecF32")
			.def(vector_indexing_suite<VecF32>())
			;

		typedef std::vector<double> VecF64;
		class_<VecF64>("VecF64")
			.def(vector_indexing_suite<VecF64>())
			;


		typedef std::vector<Matrix> MatrixVec;
		class_<MatrixVec>("MatrixVec")
			.def(no_compare_indexing_suite<MatrixVec>())
			;



		class_<Named_Wrapper, boost::noncopyable, Named*>("Named")
			.def("name", &Named::name, return_value_policy<copy_const_reference>())
			;

		class_<Clonable_Wrapper, boost::noncopyable, Clonable*>("Clonable")
			.def("Clone", &Clonable::Clone, return_value_policy<manage_new_object>())
			;
	}


