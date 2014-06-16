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
#include "./detector_builder.h"

namespace object_recognition_toolkit
{
	namespace detection
	{
		class PUBLIC_API DetectorBase
			: public Detector
		{
		public:
			DetectorBase();
			virtual ~DetectorBase();

		public:
			virtual void Detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold = 0.0) const;


		public:
			virtual boost::shared_ptr<Detector> Clone() const;

		protected:
			virtual void buildPyramid(const core::Matrix& image, pyramid::Pyramid& pyramid) const;
			virtual void scanImage(const core::Matrix& image, std::vector<image_scanning::Window>& windows) const;
			virtual void extractFeatures(const core::Matrix& image, core::FeatureVector& features) const;
			virtual void classify(const core::FeatureVector& features, double& confidence) const;
			virtual void nonMaximumSuppression(std::vector<core::Box>& detections, std::vector<double>& confidences) const;

		public:
			virtual pyramid::PyramidBuilder& GetPyramidBuilder() const;
			virtual image_scanning::ImageScanner& GetImageScanner() const;
			virtual feature_extraction::FeatureExtractor& GetFeatureExtracotr() const;
			virtual classification::Classifier& GetClassifier() const;
			virtual non_maxima_suppression::NonMaximaSuppressor& GetNonMaximaSuppressor() const;

		public:
			boost::shared_ptr<pyramid::PyramidBuilder> pyramidBuilder_;
			boost::shared_ptr<image_scanning::ImageScanner> imageScanner_;
			boost::shared_ptr<feature_extraction::FeatureExtractor> featureExtractor_;
			boost::shared_ptr<classification::Classifier> classifier_;
			boost::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaximaSuppressor_;

		private:
			DECLARE_SERIALIZABLE();

		private:
			friend class DetectorBase_Builder;
			friend class DetectorBaseMT_Builder;
		};
	}
}

#pragma warning(pop)

#endif // DETECTOR_BASE_H_INCLUDED_