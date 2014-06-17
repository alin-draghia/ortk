#include "boost_python_precomp.h"
#include "object-recognition-toolkit/python/python_ext.h"
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
			boost::shared_ptr<Detector> Clone() const { return this->get_override("Clone")(); }
			void Detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold = 0.0) const { this->get_override("Detect")(image, detections, confidences, treshold); }
		};

		struct DetectorTrainer_Wrapper
			: DetectorTrainer
			, bp::wrapper < DetectorTrainer >
		{
			boost::shared_ptr<DetectorTrainer> Clone() const { return this->get_override("Clone")(); }
			boost::shared_ptr<Detector> TrainWithDataset(const dataset::Dataset& positive, const dataset::Dataset& negative){ return this->get_override("TrainWithDataset")(positive, negative); }
			boost::shared_ptr<Detector> TrainWithImages(const std::vector<core::Matrix>& positiveImages, const std::vector<core::Matrix>& negativeImages){ return this->get_override("TrainWithImages")(positiveImages, negativeImages); }
		};

		struct BootstrappingDetectorTrainerCallback_Wrapper
			: BootstrappingDetectorTrainerCallback
			, bp::wrapper < BootstrappingDetectorTrainerCallback >
		{
			void OnBeginIteration(int iteration) { this->get_override("OnBeginIteration")(iteration); }
			void OnClassifier(boost::shared_ptr<classification::Classifier> classifier) { this->get_override("OnClassifier")(classifier); }
			void OnEndIteration(int iteration){ this->get_override("OnEndIteration")(iteration); }
			void OnPositiveSample(int count, core::Matrix const& image, core::FeatureVector const& features_vector) { this->get_override("OnPositiveSample")(count, image, features_vector); }
			void OnDoneCollectiongPositiveSamples(core::Matrix const& features) { this->get_override("OnDoneCollectiongPositiveSamples")(features); }
			void OnNegativeSample(int count, core::Matrix const& image, core::FeatureVector const& features_vector, double score) { this->get_override("OnNegativeSample")(count, image, features_vector, score); }
			void OnDoneCollectiongNegativeSamples(core::Matrix const& features) { this->get_override("OnDoneCollectiongNegativeSamples")(features); }
		};

	}
}


#include "object-recognition-toolkit/python/python_ext.h"

void py_regiser_detection()
{
	using namespace boost::python;
	using namespace object_recognition_toolkit::core;
	using namespace object_recognition_toolkit::detection;
	using object_recognition_toolkit::python_ext::serialize_pickle;

	{
		class_<Detector_Wrapper, boost::noncopyable>("Detector")
			.def("Clone", pure_virtual(&Detector::Clone))
			.def("Detect", pure_virtual(&Detector::Detect), (arg("image"), arg("detections"), arg("confidences"), arg("treshold")))
			.enable_pickling()
			;
		register_ptr_to_python<boost::shared_ptr<Detector>>();
	}

	{
		class_<DetectorBase, bases<Detector>>("DetectorBase", init<>())
			.def_readwrite("pyramid_builder", &DetectorBase::pyramidBuilder_)
			.def_readwrite("image_scanner", &DetectorBase::imageScanner_)
			.def_readwrite("feature_extractor", &DetectorBase::featureExtractor_)
			.def_readwrite("classifier", &DetectorBase::classifier_)
			.def_readwrite("non_maxima_suppressor", &DetectorBase::nonMaximaSuppressor_)
			.def_pickle(serialize_pickle<DetectorBase>())
			;
	}

	{
		class_<DetectorBaseMt, bases<DetectorBase>>("DetectorBaseMt", init<>())
			.def_pickle(serialize_pickle<DetectorBaseMt>())
			;
	}

	{
		class_<DetectorTrainer_Wrapper, boost::noncopyable>("DetectorTrainer")
			.def("Clone", pure_virtual(&DetectorTrainer::Clone))
			.def("TrainWithDataset", pure_virtual(&DetectorTrainer::TrainWithDataset), (arg("positives"), arg("negatives")))
			.def("TrainWithImages", pure_virtual(&DetectorTrainer::TrainWithImages), (arg("positives"), arg("negatives")))
			.enable_pickling()
			;
		register_ptr_to_python<boost::shared_ptr<DetectorTrainer>>();
	}

	{
		class_<BootstrappingDetectorTrainerCallback_Wrapper, boost::noncopyable>("BootstrappingDetectorTrainerCallback")
			.def("OnBeginIteration", pure_virtual(&BootstrappingDetectorTrainerCallback::OnBeginIteration), args("iteration"))
			.def("OnClassifier", pure_virtual(&BootstrappingDetectorTrainerCallback::OnClassifier), args("classifier"))
			.def("OnEndIteration", pure_virtual(&BootstrappingDetectorTrainerCallback::OnEndIteration), args("iteration"))
			.def("OnPositiveSample", pure_virtual(&BootstrappingDetectorTrainerCallback::OnPositiveSample), (arg("count"), arg("image"), arg("feature_vector")))
			.def("OnDoneCollectiongPositiveSamples", pure_virtual(&BootstrappingDetectorTrainerCallback::OnDoneCollectiongPositiveSamples), args("features"))
			.def("OnNegativeSample", pure_virtual(&BootstrappingDetectorTrainerCallback::OnNegativeSample), (arg("count"), arg("image"), arg("feature_vector"), arg("score")))
			.def("OnDoneCollectiongNegativeSamples", pure_virtual(&BootstrappingDetectorTrainerCallback::OnDoneCollectiongNegativeSamples), args("features"))
			.enable_pickling()
			;

		register_ptr_to_python<boost::shared_ptr<BootstrappingDetectorTrainerCallback>>();

		class_<BootstrappingDetectorTrainer, bases<DetectorTrainer>>("BootstrappingDetectorTrainer", init<>())
			.def_readwrite("num_iterations", &BootstrappingDetectorTrainer::num_iterations)
			.def_readwrite("num_positives", &BootstrappingDetectorTrainer::num_positives)
			.def_readwrite("num_negatives", &BootstrappingDetectorTrainer::num_negatives)
			.def_readwrite("detector_size", &BootstrappingDetectorTrainer::detector_size)
			.def_readwrite("data_directory", &BootstrappingDetectorTrainer::data_directory)
			.def_readwrite("pyramid_builder", &BootstrappingDetectorTrainer::pyramid_builder)
			.def_readwrite("image_scanner", &BootstrappingDetectorTrainer::image_scanner)
			.def_readwrite("feature_extractor", &BootstrappingDetectorTrainer::feature_extractor)
			.def_readwrite("trainer", &BootstrappingDetectorTrainer::trainer)
			.def_readwrite("non_max_supperssor", &BootstrappingDetectorTrainer::non_max_supperssor)
			.def_readwrite("callback", &BootstrappingDetectorTrainer::callback)
			.def_pickle(serialize_pickle<BootstrappingDetectorTrainer>())
			;
	}

}