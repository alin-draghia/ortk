#include "object-recognition-toolkit/detection/detector_base_mt.h"

#include <omp.h>

namespace object_recognition_toolkit
{
	namespace detection
	{

		DetectorBaseMt::DetectorBaseMt()
			: DetectorBase()
		{
		}


		DetectorBaseMt::DetectorBaseMt(ImagePyramid* pyramidBuilder, ImageScanner* imageScanner, FeatureExtractor* featureExtractor, Classifier* classifier, NonMaximaSuppressor* nonMaximaSuppressor)
			: DetectorBase(pyramidBuilder, imageScanner, featureExtractor, classifier, nonMaximaSuppressor)
		{
		}

		DetectorBaseMt::~DetectorBaseMt()
		{
		}

		const std::string& DetectorBaseMt::name() const
		{
			static const std::string name = "DetectorBaseMt";
			return name;
		}

		void DetectorBaseMt::detect(const cv::Mat& image, std::vector<cv::Rect>& detections, std::vector<double>& confidences, double treshold) const
		{
			using object_recognition_toolkit::pyramid::PyramidLevel;

			std::vector<cv::Mat> windows;
			std::vector<cv::Rect> boxes;
			std::vector<size_t> pyramid_level_indices;
			std::vector<PyramidLevel> pyramid;

			this->buildPyramid(image, pyramid);

			for (size_t pyramid_level_index = 0; pyramid_level_index < pyramid.size(); pyramid_level_index++)
			{

				PyramidLevel& pyramid_level = pyramid[pyramid_level_index];

				std::vector<cv::Mat> level_windows;
				std::vector<cv::Rect> level_boxes;

				const cv::Mat& pyramid_level_image = pyramid_level.GetImage();

				this->scanImage(pyramid_level_image, level_windows, level_boxes);


				windows.insert(std::end(windows), std::begin(level_windows), std::end(level_windows));
				boxes.insert(std::end(boxes), std::begin(level_boxes), std::end(level_boxes));
				pyramid_level_indices.insert(std::end(pyramid_level_indices), level_boxes.size(), pyramid_level_index);

			}

			
			std::vector<std::vector<cv::Rect>> thread_local_detections;
			std::vector<std::vector<double>> thread_local_confidences;

			int num_threads = omp_get_max_threads();
			thread_local_detections.resize(num_threads);
			thread_local_confidences.resize(num_threads);

			long num_windows = (long)windows.size();
			
			#pragma omp parallel for
			for (int i = 0; i < num_windows; i++)
			{

				int thread_id = omp_get_thread_num();

				const cv::Mat& window = windows[i];
				const cv::Rect& box = boxes[i];
				const size_t pyramid_level_index = pyramid_level_indices[i];
				const PyramidLevel& pyramid_level = pyramid[pyramid_level_index];

				double confidence = 0.0;
				core::FeatureVector features;

				this->extractFeatures(window, features);
				this->classify(features, confidence);

				if (confidence > treshold) {
					thread_local_detections[thread_id].push_back(pyramid_level.Invert(box));
					thread_local_confidences[thread_id].push_back(confidence);
				}
			}

			for (int i = 0; i < num_threads; i++)
			{				
				detections.insert(std::end(detections), std::begin(thread_local_detections[i]), std::end(thread_local_detections[i]));
				confidences.insert(std::end(confidences), std::begin(thread_local_confidences[i]), std::end(thread_local_confidences[i]));
				thread_local_detections[i].clear();
				thread_local_confidences[i].clear();
			}

			this->nonMaximumSuppression(detections, confidences);
		}

		void DetectorBaseMt::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<DetectorBase>(*this);
			(void)version;
		}

		void DetectorBaseMt::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<DetectorBase>(*this);
			(void)version;
		}
	}
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::detection::DetectorBaseMt);