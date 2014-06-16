#include "precomp.h"
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


		DetectorBaseMt::~DetectorBaseMt()
		{
		}


		boost::shared_ptr<Detector> DetectorBaseMt::Clone() const
		{
			std::stringstream ss;
			core::oarchive oa(ss);
			oa << this;

			core::iarchive ia(ss);
			DetectorBaseMt* ptr;
			ia >> ptr;

			return boost::shared_ptr<Detector>(ptr);
		}


		void DetectorBaseMt::Detect(const cv::Mat& image, std::vector<cv::Rect>& detections, std::vector<double>& confidences, double treshold) const
		{
			detections.clear();
			confidences.clear();

			using object_recognition_toolkit::pyramid::PyramidLevel;

			std::vector<image_scanning::Window> windows;
			std::vector<int> pyramid_level_indices;
			pyramid::Pyramid pyramid;

			this->buildPyramid(image, pyramid);

			for (int pyramid_level_index = 0; pyramid_level_index < pyramid.GetNumLevels(); pyramid_level_index++)
			{

				const PyramidLevel& pyramid_level = pyramid.GetLevel(pyramid_level_index);

				std::vector<image_scanning::Window> level_windows;


				const cv::Mat& pyramid_level_image = pyramid_level.GetImage();

				this->scanImage(pyramid_level_image, level_windows);


				windows.insert(std::end(windows), std::begin(level_windows), std::end(level_windows));
				pyramid_level_indices.insert(std::end(pyramid_level_indices), level_windows.size(), pyramid_level_index);

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

				const image_scanning::Window& window = windows[i];
				const int pyramid_level_index = pyramid_level_indices[i];
				const PyramidLevel& pyramid_level = pyramid.GetLevel(pyramid_level_index);

				double confidence = 0.0;
				core::FeatureVector features;

				this->extractFeatures(window.image, features);
				this->classify(features, confidence);

				if (confidence > treshold) {
					thread_local_detections[thread_id].push_back(pyramid_level.Invert(window.box));
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