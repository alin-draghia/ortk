#include "precomp.h"
#include "object-recognition-toolkit/detection/detector_base.h"

#include "object-recognition-toolkit/image-pyramid/float_pyramid_builder.h"

using PyramidBuilder = object_recognition_toolkit::pyramid::PyramidBuilder;
using PyramidLevel = object_recognition_toolkit::pyramid::PyramidLevel;
using ImageScanner = object_recognition_toolkit::image_scanning::ImageScanner;
using FeatureExtractor = object_recognition_toolkit::feature_extraction::FeatureExtractor;
using Classifier = object_recognition_toolkit::classification::Classifier;
using NonMaximaSuppressor = object_recognition_toolkit::non_maxima_suppression::NonMaximaSuppressor;

namespace object_recognition_toolkit
{
	namespace detection
	{
		DetectorBase::DetectorBase()
		{
		}

		DetectorBase::~DetectorBase()
		{
		}


		
		boost::shared_ptr<Detector> DetectorBase::Clone() const
		{
			std::stringstream ss;
			core::oarchive oa(ss);			
			oa << this;

			core::iarchive ia(ss);
			DetectorBase* ptr;
			ia >> ptr;

			return boost::shared_ptr<Detector>(ptr);
		}

		void DetectorBase::Detect(const core::Matrix& image, std::vector<cv::Rect>& detections, std::vector<double>& confidences, double treshold) const
		{
			detections.clear();
			confidences.clear();

			using object_recognition_toolkit::pyramid::PyramidLevel;

			pyramid::Pyramid pyramid;

			this->buildPyramid(image, pyramid);

			int pyramid_levels_count = pyramid.GetNumLevels();

			std::vector<core::Matrix> window_images;
			std::vector<core::Box> window_boxes;
			core::Matrix features;
			core::Matrix scores;


			window_images.reserve(100000);
			window_boxes.reserve(100000);

			for (int pyramid_level_index = 0; pyramid_level_index < pyramid_levels_count; pyramid_level_index++)
			{
				const pyramid::PyramidLevel& pyramid_level = pyramid.GetLevel(pyramid_level_index);

				std::vector<image_scanning::Window> windows;
				const core::Matrix& pyramid_level_image = pyramid_level.GetImage();

				this->scanImage(pyramid_level_image, windows);

				for (size_t i = 0; i < windows.size(); i++)
				{
					window_images.push_back(windows[i].image);
					window_boxes.push_back(pyramid_level.Invert(windows[i].box));
				}
			}

			features.create(window_images.size(), featureExtractor_->Lenght(), CV_32F);
			scores.create(window_images.size(), 1, CV_64F);

			featureExtractor_->ComputeMulti(window_images, features);
			classifier_->PredictMulti(features, scores);

			for (int i = 0; i < scores.rows; i++)
			{
				if (scores.at<double>(i) > treshold) 
				{
					detections.push_back(window_boxes[i]);
					confidences.push_back(scores.at<double>(i));
				}
			}
			

#if(0)
			for (int pyramid_level_index = 0; pyramid_level_index < pyramid_levels_count; pyramid_level_index++)
			{
				const pyramid::PyramidLevel& pyramid_level = pyramid.GetLevel(pyramid_level_index);

				std::vector<image_scanning::Window> windows;
				const core::Matrix& pyramid_level_image = pyramid_level.GetImage();

				this->scanImage(pyramid_level_image, windows);

				for (size_t i = 0; i < windows.size(); i++)
				{
					double confidence;
					core::FeatureVector features;

					this->extractFeatures(windows[i].image, features);
					this->classify(features, confidence);

					if (confidence > treshold) {
						detections.push_back(pyramid_level.Invert(windows[i].box));
						confidences.push_back(confidence);
					}
				}
			}
#endif

			this->nonMaximumSuppression(detections, confidences);

		}

		void DetectorBase::buildPyramid(const core::Matrix& image, pyramid::Pyramid& pyramid) const
		{
			pyramid = this->GetPyramidBuilder().Build(image);
		}

		void DetectorBase::scanImage(const core::Matrix& image, std::vector<image_scanning::Window>& windows) const
		{
			windows = this->GetImageScanner().compute(image);
		}

		void DetectorBase::extractFeatures(const core::Matrix& image, core::FeatureVector& features) const
		{
			features = this->GetFeatureExtracotr().Compute(image);
		}

		void DetectorBase::classify(const core::FeatureVector& features, double& confidence) const
		{
			confidence = this->GetClassifier().Predict(features);			
		}

		void DetectorBase::nonMaximumSuppression(std::vector<core::Box>& detections, std::vector<double>& confidences) const
		{
			this->GetNonMaximaSuppressor().suppress(detections, confidences);
		}

		PyramidBuilder& DetectorBase::GetPyramidBuilder() const
		{
			return *this->pyramidBuilder_;
		}

		ImageScanner& DetectorBase::GetImageScanner() const
		{
			return *this->imageScanner_;
		}

		FeatureExtractor& DetectorBase::GetFeatureExtracotr() const
		{
			return *this->featureExtractor_;
		}

		Classifier& DetectorBase::GetClassifier() const
		{
			return *this->classifier_;
		}

		NonMaximaSuppressor& DetectorBase::GetNonMaximaSuppressor() const
		{
			return *this->nonMaximaSuppressor_;
		}

		void DetectorBase::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Detector>(*this);
			ar >> pyramidBuilder_;
			ar >> imageScanner_;
			ar >> featureExtractor_;
			ar >> classifier_;
			ar >> nonMaximaSuppressor_;
		}

		void DetectorBase::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<Detector>(*this);
			ar << pyramidBuilder_;
			ar << imageScanner_;
			ar << featureExtractor_;
			ar << classifier_;
			ar << nonMaximaSuppressor_;
		}
	}
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::detection::DetectorBase);