#include "precomp.h"
#include "object-recognition-toolkit/detection/detector_builder.h"
#include "object-recognition-toolkit/detection/detector_base.h"
#include "object-recognition-toolkit/detection/detector_base_mt.h"

namespace object_recognition_toolkit
{
	namespace detection
	{



		DetectorBuilder::~DetectorBuilder()
		{
		}

		/*
		virtual Detector* DetectorBuilder::Build() const = 0;
		{
			std::unique_ptr<Detector_Type> detector_ptr(new Detector_Type());

			detector_ptr->pyramidBuilder_.reset(
				(pyramid::PyramidBuilder*)this->pyramidBuilder_->Clone()
				);

			detector_ptr->imageScanner_.reset(
				(image_scanning::ImageScanner*)this->imageScanner_->Clone()
				);

			detector_ptr->featureExtractor_.reset(
				(feature_extraction::FeatureExtractor*)this->featureExtractor_->Clone()
				);

			detector_ptr->classifier_.reset(
				(classification::Classifier*)this->classifier_->Clone()
				);

			detector_ptr->nonMaximaSuppressor_.reset(
				(non_maxima_suppression::NonMaximaSuppressor*)this->nonMaximaSuppressor_->Clone()
				);

			return detector_ptr.release();
		}
		*/


		void DetectorBuilder::PutPyramidBuilder(std::shared_ptr<pyramid::PyramidBuilder> pyramidBuilder){
			this->pyramidBuilder_ = pyramidBuilder;
		}

		void DetectorBuilder::PutImageScanner(std::shared_ptr<image_scanning::ImageScanner> imageScanner){
			this->imageScanner_ = imageScanner;
		}

		void DetectorBuilder::PutFeatureExtractor(std::shared_ptr<feature_extraction::FeatureExtractor> featureExtractor){
			this->featureExtractor_ = featureExtractor;
		}

		void DetectorBuilder::PutClassifier(std::shared_ptr<classification::Classifier> classifier){
			this->classifier_ = classifier;
		}

		void DetectorBuilder::PutNonMaximaSuppressor(std::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaximaSuppressor){
			this->nonMaximaSuppressor_ = nonMaximaSuppressor;
		}


		DetectorBase_Builder::~DetectorBase_Builder()
		{
		}

		Detector* DetectorBase_Builder::Build() const
		{
			std::unique_ptr<DetectorBase> detector_ptr(new DetectorBase());

			detector_ptr->pyramidBuilder_.reset(
				(pyramid::PyramidBuilder*)this->pyramidBuilder_->Clone()
				);

			detector_ptr->imageScanner_.reset(
				(image_scanning::ImageScanner*)this->imageScanner_->Clone()
				);

			detector_ptr->featureExtractor_.reset(
				(feature_extraction::FeatureExtractor*)this->featureExtractor_->Clone()
				);

			detector_ptr->classifier_.reset(
				(classification::Classifier*)this->classifier_->Clone()
				);

			detector_ptr->nonMaximaSuppressor_.reset(
				(non_maxima_suppression::NonMaximaSuppressor*)this->nonMaximaSuppressor_->Clone()
				);

			return detector_ptr.release();
		}

		DetectorBaseMT_Builder::~DetectorBaseMT_Builder()
		{
		}

		Detector* DetectorBaseMT_Builder::Build() const
		{
			std::unique_ptr<DetectorBaseMt> detector_ptr(new DetectorBaseMt());

			detector_ptr->pyramidBuilder_.reset(
				(pyramid::PyramidBuilder*)this->pyramidBuilder_->Clone()
				);

			detector_ptr->imageScanner_.reset(
				(image_scanning::ImageScanner*)this->imageScanner_->Clone()
				);

			detector_ptr->featureExtractor_.reset(
				(feature_extraction::FeatureExtractor*)this->featureExtractor_->Clone()
				);

			detector_ptr->classifier_.reset(
				(classification::Classifier*)this->classifier_->Clone()
				);

			detector_ptr->nonMaximaSuppressor_.reset(
				(non_maxima_suppression::NonMaximaSuppressor*)this->nonMaximaSuppressor_->Clone()
				);

			return detector_ptr.release();
		}

	}
}