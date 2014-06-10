#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"

namespace object_recognition_toolkit
{
	namespace dataset
	{

		long Rectangle_get_left(const Rectangle& r) { return r.left(); }
		void Rectangle_set_left(Rectangle& r, long val) { r.left() = val; }

		long Rectangle_get_right(const Rectangle& r) { return r.right(); }
		void Rectangle_set_right(Rectangle& r, long val) { r.right() = val; }

		long Rectangle_get_top(const Rectangle& r) { return r.top(); }
		void Rectangle_set_top(Rectangle& r, long val) { r.top() = val; }

		long Rectangle_get_bottom(const Rectangle& r) { return r.bottom(); }
		void Rectangle_set_bottom(Rectangle& r, long val) { r.bottom() = val; }

		std::string Rectangle__str__(const Rectangle& r)
		{
			std::ostringstream sout;
			sout << r;
			return sout.str();
		}

		std::string Rectangle__repr__(const Rectangle& r)
		{
			std::ostringstream sout;
			sout << "rectangle(" << r.left() << "," << r.top() << "," << r.right() << "," << r.bottom() << ")";
			return sout.str();
		}



	}
}


namespace dlib
{
	namespace image_dataset_metadata
	{
		bool operator==(const box& a, const box& b){ return false; }
		bool operator==(const image& a, const image& b){ return false; }
	}
}


cv::Mat test_mat_return()
{
	cv::Mat m = cv::Mat::eye(3, 3, CV_8U);
	return m;
}


std::vector<cv::Mat> test_mat_vec_return()
{
	std::vector<cv::Mat> v;
	for (int i = 0; i < 3; i++) {
		v.push_back(cv::Mat::eye(3 + i, 3 + i, CV_8U));
	}
	return v;
}

void py_regiser_dataset()
{

	using namespace boost::python;
	using namespace object_recognition_toolkit;
	using namespace object_recognition_toolkit::dataset;


	def("test_mat_return", test_mat_return);
	def("test_mat_vec_return", test_mat_vec_return);

	{
		class_<dataset::Rectangle>("Rectangle")
			.def(init<long, long, long, long>((arg("left"), arg("top"), arg("right"), arg("bottom"))))
			.add_property("left", &Rectangle_get_left, &Rectangle_set_left)
			.add_property("top", &Rectangle_get_top, &Rectangle_set_top)
			.add_property("right", &Rectangle_get_right, &Rectangle_set_right)
			.add_property("bottom", &Rectangle_get_bottom, &Rectangle_set_bottom)
			.def("width", &Rectangle::width)
			.def("height", &Rectangle::height)
			.def("__str__", &Rectangle__str__)
			.def("__repr__", &Rectangle__repr__)
			;

		typedef std::vector<dataset::Rectangle> Rectangles;
		class_<Rectangles>("Rectangles")
			.def(vector_indexing_suite<Rectangles>())
			;
	}

	{
		class_<Box>("Box")
			.add_property("rect", &Box::rect)
			.add_property("label", &Box::label)
			.add_property("difficult", &Box::difficult)
			.add_property("truncated", &Box::truncated)
			.add_property("occluded", &Box::occluded)
			.add_property("ignore", &Box::ignore)
			.add_property("angle", &Box::angle)
			;

		typedef std::vector<Box> Boxes;
		class_<Boxes>("Boxes")
			.def(vector_indexing_suite<Boxes>())
			;
	}

	{
		class_<Image>("Image")
			//.add_property("filename", &Image::filename)
			.def_readwrite("filename", &Image::filename)
			.add_property("boxes", &Image::boxes)
			;

		typedef std::vector<Image> Images;
		class_<Images>("Images")
			.def(vector_indexing_suite<Images>())
			;
	}

	{
		class_<Dataset>("Dataset")
			.add_property("images", &Dataset::images)
			.add_property("comment", &Dataset::comment)
			.add_property("name", &Dataset::name)
			;
	}


	{
		def("LoadDatasetDlib", &LoadDatasetDlib);
		def("SaveDatasetDlib", &SaveDatasetDlib);
		def("LoadDatasetPascalV1", &LoadDatasetPascalV1);
		def("LoadDatasetPascalXml", &LoadDatasetPascalXml);
		def("LoadDatasetFiles", &LoadDatasetFiles);
		def("ExtractImages", &ExtractImages);
	}
}