#pragma once
#ifndef DALLAL_TRIGGS_DETECTOR_TRAINER_H_INCLUDED
#define DALLAL_TRIGGS_DETECTOR_TRAINER_H_INCLUDED

#include "../image-pyramid/pyramid_builder.h"
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
		class PUBLIC_API BootstrappingDetectorTrainer
			: public DetectorTrainer
		{
		public:
			struct Params {
				size_t numStages;
				size_t numPositives;
				size_t numNegatives;
				core::Size trainingSize;
				std::string data_directory;
				std::shared_ptr<pyramid::PyramidBuilder> pyramidBuilder;
				std::shared_ptr<image_scanning::ImageScanner> imageScanner;
				std::shared_ptr<feature_extraction::FeatureExtractor> featureExtractor;
				std::shared_ptr<classification::Trainer> trainer;
				std::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaxSupperssor;
			};

		public:
			BootstrappingDetectorTrainer();
			BootstrappingDetectorTrainer(const Params& params);
			virtual ~BootstrappingDetectorTrainer();
			
		public:
			virtual Detector* TrainWithDataset(const dataset::Dataset& positive, const dataset::Dataset& negative);
			virtual Detector* TrainWithImages(const std::vector<core::Matrix>& positiveImages, const std::vector<core::Matrix>& negativeImages);

		public:
			virtual const std::string& name() const;

		public:
			virtual Clonable* Clone();

		private:
			core::Matrix extractPositives(const dataset::Dataset& positive) const;
			core::Matrix extractNegatives(const dataset::Dataset& negative, classification::Classifier* classifier) const;

			void extractFeaturesFromPositiveImages(const std::vector<core::Matrix>& images, core::Matrix& features, const size_t numSamples) const;
			void extractFeaturesFromNegativeImages(const std::vector<core::Matrix>& images, core::Matrix& features, const size_t numSamples, classification::Classifier* classifier) const;

			void extractPositiveImages(const dataset::Dataset& positiveDataset, std::vector<core::Matrix>& images) const;
			void extractNegativeImages(const dataset::Dataset& negativeDataset, std::vector<core::Matrix>& images) const;

			Detector* buildDetector(classification::Classifier* classifier);

		private:
			DECLARE_SERIALIZABLE();

		private:
			Params params_;
		};
	}
}

#endif