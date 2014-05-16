#pragma once
#ifndef DALLAL_TRIGGS_DETECTOR_TRAINER_H_INCLUDED
#define DALLAL_TRIGGS_DETECTOR_TRAINER_H_INCLUDED

#include "../image-pyramid/image-pyramid.h"
#include "../image-scanning/image-scanner.h"
#include "../feature-extraction/feature-extractor.h"
#include "../classification/classifier.h"
#include "../classification/trainer.h"
#include "../non_maxima_suppression/non_maxima_suppressor.h"
#include "./detector_trainer.h"

namespace object_recognition_toolkit
{
	namespace detection
	{
		class PUBLIC_API DallalTriggsDetectorTrainer
			: public DetectorTrainer
		{
		public:
			struct Params {
				size_t numStages;
				size_t numPositives;
				size_t numNegatives;
				core::Size trainingSize;
				std::shared_ptr<pyramid::ImagePyramid> pyramidBuilder;
				std::shared_ptr<image_scanning::ImageScanner> imageScanner;
				std::shared_ptr<feature_extraction::FeatureExtractor> featureExtractor;
				std::shared_ptr<classification::Trainer> trainer;
				std::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaxSupperssor;
			};

		public:
			DallalTriggsDetectorTrainer();
			DallalTriggsDetectorTrainer(const Params& params);
			virtual ~DallalTriggsDetectorTrainer();
			
		public:
			virtual Detector* Train(const dataset::Dataset& positive, const dataset::Dataset& negative);

		public:
			virtual const std::string& name() const;

		private:
			core::Matrix extractPositives(const dataset::Dataset& positive) const;
			core::Matrix extractNegatives(const dataset::Dataset& negative, classification::Classifier* classifier) const;
			Detector* buildDetector(classification::Classifier* classifier);
		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);

		private:
			Params params_;
		};
	}
}

#endif