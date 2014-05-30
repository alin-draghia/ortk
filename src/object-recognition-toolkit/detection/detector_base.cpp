#include "object-recognition-toolkit/detection/detector_base.h"


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

		DetectorBase::DetectorBase(PyramidBuilder* pyramidBuilder, ImageScanner* imageScanner, FeatureExtractor* featureExtractor, Classifier* classifier, NonMaximaSuppressor* nonMaximaSuppressor)
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

		void DetectorBase::detect(const core::Matrix& image, std::vector<cv::Rect>& detections, std::vector<double>& confidences, double treshold) const
		{
			using object_recognition_toolkit::pyramid::PyramidLevel;

			std::vector<PyramidLevel> pyramid;

			this->buildPyramid(image, pyramid);

			for (PyramidLevel& pyramid_level : pyramid)
			{
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

			this->nonMaximumSuppression(detections, confidences);

		}

		void DetectorBase::buildPyramid(const core::Matrix& image, std::vector<pyramid::PyramidLevel>& pyramid) const
		{
			pyramid = this->getPyramidBuilder().Build(image);
		}

		void DetectorBase::scanImage(const core::Matrix& image, std::vector<image_scanning::Window>& windows) const
		{
			windows = this->getImageScanner().compute(image);
		}

		void DetectorBase::extractFeatures(const core::Matrix& image, core::FeatureVector& features) const
		{
			features = this->getFeatureExtractor().compute(image);
		}

		void DetectorBase::classify(const core::FeatureVector& features, double& confidence) const
		{
			confidence = this->getClassifier().Predict(features);			
		}

		void DetectorBase::nonMaximumSuppression(std::vector<core::Box>& detections, std::vector<double>& confidences) const
		{
			// pass
		}

		PyramidBuilder& DetectorBase::getPyramidBuilder() const
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