#include <boost/python.hpp>

#include "object-recognition-toolkit/object_recognition_toolkit.h"


namespace object_recognition_toolkit
{
	namespace detection
	{
		namespace bp = boost::python;

		struct Detector_Wrapper
			: Detector
			, bp::wrapper < Detector >
		{
			void Detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold = 0.0) const
			{
				this->get_override("Detect")(image, detections, confidences, treshold);
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


		struct DetectorTrainer_Wrapper
			: DetectorTrainer
			, bp::wrapper<DetectorTrainer>
		{
			Detector* Train(const dataset::Dataset& positive, const dataset::Dataset& negative)
			{
				return this->get_override("Train")(positive, negative);
			}			

			const std::string& name() const
			{
				return this->get_override("name")();
			}

		};


		Detector* build_Detector(
			std::auto_ptr<pyramid::PyramidBuilder> pyramidBuilder,
			std::auto_ptr<image_scanning::ImageScanner> imageScanner, 
			std::auto_ptr<feature_extraction::FeatureExtractor> featureExtractor, 
			std::auto_ptr<classification::Classifier> classifier, 
			std::auto_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaximaSuppressor)
		{
			DetectorBase_Builder builder;
			builder.PutPyramidBuilder(std::shared_ptr<pyramid::PyramidBuilder>(pyramidBuilder.release()));
			builder.PutImageScanner(std::shared_ptr<image_scanning::ImageScanner>(imageScanner.release()));
			builder.PutFeatureExtractor(std::shared_ptr<feature_extraction::FeatureExtractor>(featureExtractor.release()));
			builder.PutClassifier(std::shared_ptr<classification::Classifier>(classifier.release()));
			builder.PutNonMaximaSuppressor(std::shared_ptr<non_maxima_suppression::NonMaximaSuppressor>(nonMaximaSuppressor.release()));
			return builder.Build();
		}


		Detector* build_DetectorMT(
			std::auto_ptr<pyramid::PyramidBuilder> pyramidBuilder,
			std::auto_ptr<image_scanning::ImageScanner> imageScanner,
			std::auto_ptr<feature_extraction::FeatureExtractor> featureExtractor,
			std::auto_ptr<classification::Classifier> classifier,
			std::auto_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaximaSuppressor)
		{
			DetectorBaseMT_Builder builder;
			builder.PutPyramidBuilder(std::shared_ptr<pyramid::PyramidBuilder>(pyramidBuilder.release()));
			builder.PutImageScanner(std::shared_ptr<image_scanning::ImageScanner>(imageScanner.release()));
			builder.PutFeatureExtractor(std::shared_ptr<feature_extraction::FeatureExtractor>(featureExtractor.release()));
			builder.PutClassifier(std::shared_ptr<classification::Classifier>(classifier.release()));
			builder.PutNonMaximaSuppressor(std::shared_ptr<non_maxima_suppression::NonMaximaSuppressor>(nonMaximaSuppressor.release()));
			return builder.Build();
		}
	}
}


#include "object-recognition-toolkit/python/python_ext.h"

void py_regiser_detection()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::detection;
	using object_recognition_toolkit::python_ext::serialize_pickle;


	class_<Detector_Wrapper, boost::noncopyable>("Detector")
		.def("Detect", pure_virtual(&Detector::Detect))
		.def("Name", pure_virtual(&Detector::name), return_value_policy<copy_const_reference>())
		.def("Clone", pure_virtual(&Detector::Clone), return_value_policy<manage_new_object>())
		.def_pickle(serialize_pickle<Detector>())
		;

	class_<DetectorTrainer_Wrapper, boost::noncopyable>("DetectorTrainer")
		.def("Train", pure_virtual(&DetectorTrainer::Train), return_value_policy<manage_new_object>())
		.def("Name", pure_virtual(&DetectorTrainer::name), return_value_policy<copy_const_reference>())
		.def_pickle(serialize_pickle<DetectorTrainer>())
		;


	def("build_Detector", build_Detector, return_value_policy<manage_new_object>());
	def("build_DetectorMT", build_DetectorMT, return_value_policy<manage_new_object>());
}