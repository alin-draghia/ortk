#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"

#include <boost/make_shared.hpp>

namespace object_recognition_toolkit {
	namespace feature_extraction {

		namespace bp = boost::python;

		struct FeatureExtractor_Wrapper
			: FeatureExtractor
			, bp::wrapper < FeatureExtractor >
		{
			boost::shared_ptr<FeatureExtractor> Clone() const { 
				return  this->get_override("Clone")();
			}
			
			core::FeatureVector Compute(const core::Matrix& image) const {
				return this->get_override("Compute")(image);
			}

			void ComputeMulti(std::vector<core::Matrix> const& images, core::Matrix& features) const
			{
				this->get_override("ComputeMulti")(images, features);
			}

			int Lenght() const {
				return this->get_override("Lenght")();
			}

		};

		core::Matrix FeatureExtractor_ComputeMulti_v2(FeatureExtractor const& fe, std::vector<core::Matrix> const& images)
		{
			cv::Mat X((int)images.size(), fe.Lenght(), CV_32F);
			fe.ComputeMulti(images, X);
			return X;
		}
	}
}

#include "object-recognition-toolkit/python/python_ext.h"

void py_regiser_feature_extraction()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::core;
	using namespace object_recognition_toolkit::feature_extraction;
	using object_recognition_toolkit::python_ext::serialize_pickle;

	{
		class_<FeatureExtractor_Wrapper, boost::noncopyable>("FeatureExtractor")
			.def("Clone", pure_virtual(&FeatureExtractor::Clone))
			.def("Compute", pure_virtual(&FeatureExtractor::Compute), args("image"))
			.def("ComputeMulti", pure_virtual(&FeatureExtractor::ComputeMulti), args("images", "features"))
			.def("ComputeMulti", FeatureExtractor_ComputeMulti_v2, args("images"))
			.def("Lenght", pure_virtual(&FeatureExtractor::Lenght))
			.enable_pickling()
			;

		register_ptr_to_python<boost::shared_ptr<FeatureExtractor> >();
	}
	
	{
		class_<HogFeatureExtractor, bases<FeatureExtractor>>("HogFeatureExtractor", init<>())
			.def(init<Size, Size, Size, Size, int>(args("win_size", "block_size", "block_stride", "cell_size", "n_bins")))
			.def_readwrite("win_size", &HogFeatureExtractor::winSize_)
			.def_readwrite("block_size", &HogFeatureExtractor::blockSize_)
			.def_readwrite("block_stride", &HogFeatureExtractor::blockStride_)
			.def_readwrite("cell_size", &HogFeatureExtractor::cellSize_)
			.def_readwrite("n_bins", &HogFeatureExtractor::nBins_)
			.def_pickle(serialize_pickle<HogFeatureExtractor>())
			;
	}

}