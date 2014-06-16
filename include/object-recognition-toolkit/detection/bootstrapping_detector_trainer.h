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

#include <boost/shared_ptr.hpp>

namespace object_recognition_toolkit
{
	namespace detection
	{
		struct BootstrappingDetectorTrainerCallback
		{
			virtual ~BootstrappingDetectorTrainerCallback() = 0 {}
			virtual void OnBeginIteration(int iteration) = 0;
			virtual void OnClassifier(boost::shared_ptr<classification::Classifier> classifier) = 0;
			virtual void OnEndIteration(int iteration) = 0;
			virtual void OnPositiveSample(int count, core::Matrix const& image, core::FeatureVector const& features_vector) = 0;
			virtual void OnNegativeSample(int count, core::Matrix const& image, core::FeatureVector const& features_vector, double score) = 0;
		};

		class PUBLIC_API BootstrappingDetectorTrainer
			: public DetectorTrainer
		{
		public:
			BootstrappingDetectorTrainer();
			virtual ~BootstrappingDetectorTrainer();
			
		public:
			virtual boost::shared_ptr<Detector> TrainWithDataset(const dataset::Dataset& positive, const dataset::Dataset& negative);
			virtual boost::shared_ptr<Detector> TrainWithImages(const std::vector<core::Matrix>& positiveImages, const std::vector<core::Matrix>& negativeImages);


		public:
			virtual boost::shared_ptr<DetectorTrainer> Clone() const;

		private:
			core::Matrix extractPositives(const dataset::Dataset& positive) const;
			core::Matrix extractNegatives(const dataset::Dataset& negative, boost::shared_ptr<classification::Classifier> classifier) const;

			void extractFeaturesFromPositiveImages(const std::vector<core::Matrix>& images, core::Matrix& features, const size_t numSamples) const;
			void extractFeaturesFromNegativeImages(const std::vector<core::Matrix>& images, core::Matrix& features, const size_t numSamples, boost::shared_ptr<classification::Classifier> classifier) const;

			void extractPositiveImages(const dataset::Dataset& positiveDataset, std::vector<core::Matrix>& images) const;
			void extractNegativeImages(const dataset::Dataset& negativeDataset, std::vector<core::Matrix>& images) const;

			void OnBeginIteration(int iteration);
			void OnClassifier(boost::shared_ptr<classification::Classifier> classifier);
			void OnEndIteration(int iteration);

		public:
			int num_iterations;
			int num_positives;
			int num_negatives;
			core::Size detector_size;
			std::string data_directory;
			boost::shared_ptr<pyramid::PyramidBuilder> pyramid_builder;
			boost::shared_ptr<image_scanning::ImageScanner> image_scanner;
			boost::shared_ptr<feature_extraction::FeatureExtractor> feature_extractor;
			boost::shared_ptr<classification::Trainer> trainer;
			boost::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> non_max_supperssor;
			boost::shared_ptr<BootstrappingDetectorTrainerCallback> callback;

		private:
			DECLARE_SERIALIZABLE();
		};
	}
}

#endif