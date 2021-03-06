#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"

#include <boost/make_shared.hpp>

namespace object_recognition_toolkit
{
	namespace image_scanning
	{
		namespace bp = boost::python;

		struct ImageScanner_Wrapper
			: ImageScanner
			, bp::wrapper < ImageScanner >
		{

			boost::shared_ptr<ImageScanner> Clone() const
			{
				return this->get_override("Clone")();
			}

			std::vector<Window> compute(const core::Matrix& image) const
			{
				return this->get_override("compute")();
			}
		};

		core::Matrix window_get_image(Window const& w) { return w.image; }
		core::Box window_get_box(Window const& w) { return w.box; }
		void window_set_image(Window& w, core::Matrix const& im) { w.image = im; }
		void window_set_box(Window& w, core::Box const& b) { w.box = b; }

		boost::shared_ptr<ImageScanner> create_DenseImageScanner(core::Size windowSize, core::Size windowStep, core::Size padding)
		{
			return boost::make_shared<DenseImageScanner>(windowSize, windowStep, padding);
		}
	}
}

void py_regiser_image_scanning()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::core;
	using namespace object_recognition_toolkit::image_scanning;
	using object_recognition_toolkit::python_ext::serialize_pickle;

	{
		class_<Window>("Window")
			.add_property("Image", window_get_image, window_set_image)
			.add_property("Box", window_get_box, window_set_box)
			;

		typedef std::vector<Window> WindowVector;
		class_<WindowVector>("WindowVector")
			.def(vector_indexing_suite<WindowVector>())
			;
	}

	{
		class_<ImageScanner_Wrapper, boost::noncopyable>("ImageScanner")
			.def("Clone", pure_virtual(&ImageScanner::Clone))
			.def("compute", pure_virtual(&ImageScanner::compute), args("image"))
			.enable_pickling()
			;
		register_ptr_to_python<boost::shared_ptr<ImageScanner>>();
	}

	{
		class_<DenseImageScanner, bases<ImageScanner>>("DenseImageScanner", init<>())
			.def(init<Size, Size, Size>(args("win_size", "win_step", "padding")))
			.def_readwrite("win_size", &DenseImageScanner::windowSize_)
			.def_readwrite("win_step", &DenseImageScanner::windowStep_)
			.def_readwrite("padding", &DenseImageScanner::padding_)
			.def_pickle(serialize_pickle<DenseImageScanner>())
			;
	}
}