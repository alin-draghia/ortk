#pragma once
#ifndef DETECTOR_BASE_H_INCLUDED_
#define DETECTOR_BASE_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../image-pyramid/pyramid_builder.h"
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
			using PyramidBuilder = pyramid::PyramidBuilder;
			using PyramidLevel = pyramid::PyramidLevel;
			using ImageScanner = image_scanning::ImageScanner;
			using FeatureExtractor = feature_extraction::FeatureExtractor;
			using Classifier = classification::Classifier;
			using NonMaximaSuppressor = non_maxima_suppression::NonMaximaSuppressor;

		protected:
			DetectorBase();

		public:
			DetectorBase(PyramidBuilder* pyramidBuilder, ImageScanner* imageScanner, FeatureExtractor* featureExtractor, Classifier* classifier, NonMaximaSuppressor* nonMaximaSuppressor);
			virtual ~DetectorBase();

		public:
			virtual void detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold = 0.0) const;

		public:
			virtual const std::string& name() const;

		protected:
			virtual void buildPyramid(const core::Matrix& image, std::vector<pyramid::PyramidLevel>& pyramid) const;
			virtual void scanImage(const core::Matrix& image, std::vector<image_scanning::Window>& windows) const;
			virtual void extractFeatures(const core::Matrix& image, core::FeatureVector& features) const;
			virtual void classify(const core::FeatureVector& features, double& confidence) const;
			virtual void nonMaximumSuppression(std::vector<core::Box>& detections, std::vector<double>& confidences) const;

		private:
			PyramidBuilder& getPyramidBuilder() const;
			ImageScanner& getImageScanner() const;
			FeatureExtractor& getFeatureExtractor() const;
			Classifier& getClassifier() const;
			NonMaximaSuppressor& getNonMaximaSuppressor() const;

		private:
			std::unique_ptr<PyramidBuilder> pyramidBuilder_;
			std::unique_ptr<ImageScanner> imageScanner_;
			std::unique_ptr<FeatureExtractor> featureExtractor_;
			std::unique_ptr<Classifier> classifier_;
			std::unique_ptr<NonMaximaSuppressor> nonMaximaSuppressor_;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};
	}
}

#pragma warning(pop)

#endif // DETECTOR_BASE_H_INCLUDED_