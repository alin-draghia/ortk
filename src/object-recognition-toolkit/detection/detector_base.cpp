#include "object-recognition-toolkit/detection/detector_base.h"


using ImagePyramid = object_recognition_toolkit::pyramid::ImagePyramid;
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

		DetectorBase::DetectorBase(ImagePyramid* pyramidBuilder, ImageScanner* imageScanner, FeatureExtractor* featureExtractor, Classifier* classifier, NonMaximaSuppressor* nonMaximaSuppressor)
			: pyramidBuilder_(pyramidBuilder)
			, imageScanner_(imageScanner)
			, featureExtractor_(featureExtractor)
			, classifier_(classifier)
			, nonMaximaSuppressor_(nonMaximaSuppressor)
		{
		}

		DetectorBase::~DetectorBase()
		{
		}

		const std::string& DetectorBase::name() const
		{
			static const std::string name = "DetectorBase";
			return name;
		}

		void DetectorBase::detect(const cv::Mat& image, std::vector<cv::Rect>& detections, std::vector<double>& confidences, double treshold) const
		{
			using object_recognition_toolkit::pyramid::PyramidLevel;

			std::vector<PyramidLevel> pyramid;

			this->buildPyramid(image, pyramid);

			for (PyramidLevel& pyramid_level : pyramid)
			{

				std::vector<cv::Mat> windows;
				std::vector<cv::Rect> boxes;

				const cv::Mat& pyramid_level_image = pyramid_level.GetImage();

				this->scanImage(pyramid_level_image, windows, boxes);

				for (size_t i = 0; i < windows.size(); i++)
				{
					double confidence;
					std::vector<float> features;
					const cv::Mat& window_image = windows[i];
					const cv::Rect& window_box = boxes[i];

					this->extractFeatures(window_image, features);
					this->classify(features, confidence);

					if (confidence > treshold) {
						detections.push_back(pyramid_level.Invert(window_box));
						confidences.push_back(confidence);
					}
				}
			}

			this->nonMaximumSuppression(detections, confidences);

		}

		void DetectorBase::buildPyramid(const cv::Mat& image, std::vector<pyramid::PyramidLevel>& pyramid) const
		{
			pyramid = this->getPyramidBuilder().Build(image);
		}

		void DetectorBase::scanImage(const cv::Mat& image, std::vector<cv::Mat>& windows, std::vector<cv::Rect>& boxes) const
		{
			this->getImageScanner().ScanImage(image, windows, boxes);
		}

		void DetectorBase::extractFeatures(const cv::Mat& image, std::vector<float>& features) const
		{
			features = this->getFeatureExtractor().compute(image);
		}

		void DetectorBase::classify(const std::vector<float>& features, double& confidence) const
		{
			confidence = this->getClassifier().PredictConf(features);
		}

		void DetectorBase::nonMaximumSuppression(std::vector<cv::Rect>& detections, std::vector<double>& confidences) const
		{
			// pass
		}

		ImagePyramid& DetectorBase::getPyramidBuilder() const
		{
			return *this->pyramidBuilder_;
		}

		ImageScanner& DetectorBase::getImageScanner() const
		{
			return *this->imageScanner_;
		}

		FeatureExtractor& DetectorBase::getFeatureExtractor() const
		{
			return *this->featureExtractor_;
		}

		Classifier& DetectorBase::getClassifier() const
		{
			return *this->classifier_;
		}

		NonMaximaSuppressor& DetectorBase::getNonMaximaSuppressor() const
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