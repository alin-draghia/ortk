#pragma once
#ifndef DETECTOR_BASE_H_INCLUDED_
#define DETECTOR_BASE_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "../image-pyramid/image-pyramid.h"
#include "../image-scanning/image-scanner.h"
#include "../feature-extraction/feature-extractor.h"
#include "../classification/classifier.h"
#include "../non_maxima_suppression/non_maxima_suppressor.h"
#include "./detector.h"

namespace object_recognition_toolkit
{
	namespace detection
	{
		class PUBLIC_API DetectorBase
			: public Detector
		{
			using ImagePyramid = object_recognition_toolkit::pyramid::ImagePyramid;
			using PyramidLevel = object_recognition_toolkit::pyramid::PyramidLevel;
			using ImageScanner = object_recognition_toolkit::image_scanning::ImageScanner;
			using FeatureExtractor = object_recognition_toolkit::feature_extraction::FeatureExtractor;
			using Classifier = object_recognition_toolkit::classification::Classifier;
			using NonMaximaSuppressor = object_recognition_toolkit::non_maxima_suppression::NonMaximaSuppressor;

		private:
			DetectorBase();

		public:
			DetectorBase(ImagePyramid* pyramidBuilder, ImageScanner* imageScanner, FeatureExtractor* featureExtractor, Classifier* classifier, NonMaximaSuppressor* nonMaximaSuppressor);
			virtual ~DetectorBase();

		public:
			void detect(const cv::Mat& image, std::vector<cv::Rect>& detections, std::vector<double>& confidences, double treshold = 0.0) const;

		protected:
			virtual void buildPyramid(const cv::Mat& image, std::vector<pyramid::PyramidLevel>& pyramid) const;
			virtual void scanImage(const cv::Mat& image, std::vector<cv::Mat>& windows, std::vector<cv::Rect>& boxes) const;
			virtual void extractFeatures(const cv::Mat& image, std::vector<float>& features) const;
			virtual void classify(const std::vector<float>& features, double& confidence) const;
			virtual void nonMaximumSuppression(std::vector<cv::Rect>& detections, std::vector<double>& confidences) const;

		private:
			ImagePyramid& getPyramidBuilder() const;
			ImageScanner& getImageScanner() const;
			FeatureExtractor& getFeatureExtractor() const;
			Classifier& getClassifier() const;
			NonMaximaSuppressor& getNonMaximaSuppressor() const;

		private:
			std::unique_ptr<ImagePyramid> pyramidBuilder_;
			std::unique_ptr<ImageScanner> imageScanner_;
			std::unique_ptr<FeatureExtractor> featureExtractor_;
			std::unique_ptr<Classifier> classifier_;
			std::unique_ptr<NonMaximaSuppressor> nonMaximaSuppressor_;

		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};
	}
}

#pragma warning(pop)

#endif // DETECTOR_BASE_H_INCLUDED_