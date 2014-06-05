#include <boost/python.hpp>

#include "object-recognition-toolkit/object_recognition_toolkit.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		namespace bp = boost::python;

		struct FeatureExtractor_Wrapper
			: FeatureExtractor
			, bp::wrapper<FeatureExtractor>
		{
			core::FeatureVector compute(const core::Matrix& image) const
			{
				return this->get_override("compute")(image);
			}

			int lenght() const
			{
				return this->get_override("lenght")();
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


		FeatureExtractor* create_HogFeatureExtractor()
		{
			return new HogFeatureExtractor();
		}

	}
}

#include "object-recognition-toolkit/python/python_ext.h"

void py_regiser_feature_extraction()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::feature_extraction;
	using object_recognition_toolkit::python_ext::serialize_pickle;


	class_<FeatureExtractor_Wrapper, boost::noncopyable>("FeatureExtractor")
		.def("compute", pure_virtual(&FeatureExtractor::compute))
		.def("lenght", pure_virtual(&FeatureExtractor::lenght))
		.def_pickle(serialize_pickle<FeatureExtractor>());

	def("create_HogFeatureExtractor", create_HogFeatureExtractor, return_value_policy<manage_new_object>());
	
}