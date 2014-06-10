#include "precomp.h"
#include "object-recognition-toolkit/detection/bootstrapping_detector_trainer.h"
#include "object-recognition-toolkit/detection/detector_base_mt.h"
#include "object-recognition-toolkit/detection/detector_builder.h"

#include <filesystem>
namespace fs = std::tr2::sys;

namespace object_recognition_toolkit
{
	namespace detection
	{
		BootstrappingDetectorTrainer::BootstrappingDetectorTrainer()
		{
		}

		BootstrappingDetectorTrainer::BootstrappingDetectorTrainer(const Params& params)
			: params_(params)
		{
		}

		BootstrappingDetectorTrainer::~BootstrappingDetectorTrainer()
		{
		}

		const std::string& BootstrappingDetectorTrainer::name() const
		{
			static const std::string name = "BootstrappingDetectorTrainer";
			return name;
		}

		core::Clonable* BootstrappingDetectorTrainer::Clone()
		{
			return new BootstrappingDetectorTrainer(*this);
		}

		Detector* BootstrappingDetectorTrainer::TrainWithDataset(const dataset::Dataset& positive, const dataset::Dataset& negative)
		{
			core::Matrix X_pos;
			core::Matrix X_neg, X_neg_total;
			auto numStages = params_.numStages;
			auto numPositives = params_.numPositives;
			auto numNegatives = params_.numNegatives;
			auto trainer = params_.trainer;
			auto featureExtractor = params_.featureExtractor;
			auto featureLenght = featureExtractor->lenght();


			X_pos.create(0, featureLenght, CV_32F);
			X_pos.reserve(numPositives);

			X_neg.create(0, featureLenght, CV_32F);
			X_neg.reserve(numNegatives);

			X_neg_total.create(0, featureLenght, CV_32F);
			X_neg_total.reserve(numNegatives * numStages);

			std::string& data_directory = params_.data_directory;
			if (!fs::exists(fs::path(data_directory))) {
				fs::create_directories(fs::path(data_directory));
			}

			std::unique_ptr<detection::Detector> detector;
			std::unique_ptr<classification::Classifier> classifier;


			fs::path positive_features_filename
				= fs::path(data_directory) / fs::path("positive_features.yaml");


			if (fs::exists(positive_features_filename)) {
				cv::FileStorage fileStorage;
				fileStorage.open(positive_features_filename, cv::FileStorage::READ);
				fileStorage["features"] >> X_pos;
			}
			else {
				X_pos = extractPositives(positive);
				cv::FileStorage fileStorage;
				fileStorage.open(positive_features_filename, cv::FileStorage::WRITE);
				fileStorage << "features" << X_pos;
			}


			for (size_t stage = 0; stage < numStages; stage++)
			{
				fs::path stage_classifier_filename
					= fs::path(data_directory) / fs::path("classifier-stage-" + std::to_string(stage) + ".dat");

				fs::path stage_detector_filename
					= fs::path(data_directory) / fs::path("detector-stage-" + std::to_string(stage) + ".dat");

				fs::path stage_negative_features_filename
					= fs::path(data_directory) / fs::path("negative-features-stage-" + std::to_string(stage) + ".yaml");


				std::cerr << "Training stage " << std::to_string(stage) << " ... ";

				if (fs::exists(stage_negative_features_filename)) {
					cv::FileStorage storage;
					storage.open(stage_negative_features_filename, cv::FileStorage::READ);
					storage["features"] >> X_neg;
				}
				else {
					X_neg = extractNegatives(negative, classifier.get());
					cv::FileStorage storage;
					storage.open(stage_negative_features_filename, cv::FileStorage::WRITE);
					storage << "features" << X_neg;
				}

				X_neg_total.push_back(X_neg);

				int pos_count = X_pos.rows;
				int neg_count = X_neg_total.rows;

				core::Matrix X, y, y_pos, y_neg;
				X.create(0, X_pos.cols, CV_32F);
				X.reserve(pos_count + neg_count);
				y.create(0, 1, CV_32F);
				y.reserve(pos_count + neg_count);

				X.push_back(X_pos);
				X.push_back(X_neg_total);

				y_pos.create(pos_count, 1, CV_32F);
				y_pos = 1.0f;

				y_neg.create(neg_count, 1, CV_32F);
				y_neg = -1.0f;

				y.push_back(y_pos);
				y.push_back(y_neg);

				if (fs::exists(stage_classifier_filename)) {
					std::ifstream ifs(stage_classifier_filename);
					core::iarchive ia(ifs);
					ia >> classifier;
				}
				else {

					classifier.reset(
						trainer->Train(X, y)
						);

					std::ofstream ofs(stage_classifier_filename);
					core::oarchive oa(ofs);
					oa << classifier;
				}

				if (fs::exists(stage_detector_filename)) {
					std::ifstream ifs(stage_detector_filename);
					core::iarchive ia(ifs);
					ia >> detector;
				}
				else {

					detector.reset(
						buildDetector(classifier.get())
						);

					std::ofstream ofs(stage_detector_filename);
					core::oarchive oa(ofs);
					oa << detector;
				}

				std::cerr << "done" << std::endl;
			}

			return detector.release();
		}


		Detector* BootstrappingDetectorTrainer::TrainWithImages(const std::vector<core::Matrix>& positiveImages, const std::vector<core::Matrix>& negativeImages)
		{
			core::Matrix
				X_pos,
				X_neg,
				X_neg_total
				;

			/*
			std::vector < core::Matrix >
				positiveImages,
				negativeImages
				;
				*/

			auto trainer = params_.trainer;
			auto featureExtractor = params_.featureExtractor;

			const int numStages = (int)params_.numStages;
			const int numPositives = (int)params_.numPositives;
			const int numNegatives = (int)params_.numNegatives;
			const int totalNumNegatives = (int)numNegatives * (int)numStages;
			const int featureLenght = (int)featureExtractor->lenght();

			const std::string& data_directory = params_.data_directory;

			X_pos.create(0, featureLenght, CV_32F);
			X_pos.reserve(numPositives);

			X_neg.create(0, featureLenght, CV_32F);
			X_neg.reserve(numNegatives);

			X_neg_total.create(0, featureLenght, CV_32F);
			X_neg_total.reserve(totalNumNegatives);

			if (!fs::exists(fs::path(data_directory))) {
				fs::create_directories(fs::path(data_directory));
			}

			std::unique_ptr<detection::Detector> detector;
			std::unique_ptr<classification::Classifier> classifier;


			fs::path positive_features_filename
				= fs::path(data_directory) / fs::path("positive_features.yaml");


			if (fs::exists(positive_features_filename)) {
				cv::FileStorage fileStorage;
				fileStorage.open(positive_features_filename, cv::FileStorage::READ);
				fileStorage["features"] >> X_pos;
			}
			else {
				//X_pos = extractPositives(positive);
				extractFeaturesFromPositiveImages(positiveImages, X_pos, numPositives);
				cv::FileStorage fileStorage;
				fileStorage.open(positive_features_filename, cv::FileStorage::WRITE);
				fileStorage << "features" << X_pos;
			}


			for (size_t stage = 0; stage < numStages; stage++)
			{
				fs::path stage_classifier_filename
					= fs::path(data_directory) / fs::path("classifier-stage-" + std::to_string(stage) + ".dat");

				fs::path stage_detector_filename
					= fs::path(data_directory) / fs::path("detector-stage-" + std::to_string(stage) + ".dat");

				fs::path stage_negative_features_filename
					= fs::path(data_directory) / fs::path("negative-features-stage-" + std::to_string(stage) + ".yaml");


				std::cerr << "Training stage " << std::to_string(stage) << " ... ";

				if (fs::exists(stage_negative_features_filename)) {
					cv::FileStorage storage;
					storage.open(stage_negative_features_filename, cv::FileStorage::READ);
					storage["features"] >> X_neg;
				}
				else {
					//X_neg = extractNegatives(negative, classifier.get());
					extractFeaturesFromNegativeImages(positiveImages, X_pos, numNegatives, classifier.get());
					cv::FileStorage storage;
					storage.open(stage_negative_features_filename, cv::FileStorage::WRITE);
					storage << "features" << X_neg;
				}

				X_neg_total.push_back(X_neg);

				int pos_count = X_pos.rows;
				int neg_count = X_neg_total.rows;

				core::Matrix X, y, y_pos, y_neg;
				X.create(0, X_pos.cols, CV_32F);
				X.reserve(pos_count + neg_count);
				y.create(0, 1, CV_32F);
				y.reserve(pos_count + neg_count);

				X.push_back(X_pos);
				X.push_back(X_neg_total);

				y_pos.create(pos_count, 1, CV_32F);
				y_pos = 1.0f;

				y_neg.create(neg_count, 1, CV_32F);
				y_neg = -1.0f;

				y.push_back(y_pos);
				y.push_back(y_neg);

				if (fs::exists(stage_classifier_filename)) {
					std::ifstream ifs(stage_classifier_filename);
					core::iarchive ia(ifs);
					ia >> classifier;
				}
				else {

					classifier.reset(
						trainer->Train(X, y)
						);

					std::ofstream ofs(stage_classifier_filename);
					core::oarchive oa(ofs);
					oa << classifier;
				}

				if (fs::exists(stage_detector_filename)) {
					std::ifstream ifs(stage_detector_filename);
					core::iarchive ia(ifs);
					ia >> detector;
				}
				else {

					detector.reset(
						buildDetector(classifier.get())
						);

					std::ofstream ofs(stage_detector_filename);
					core::oarchive oa(ofs);
					oa << detector;
				}

				std::cerr << "done" << std::endl;
			}

			return detector.release();
		}

		struct done_collecting_samples{};

		void BootstrappingDetectorTrainer::extractPositiveImages(const dataset::Dataset& positiveDataset, std::vector<core::Matrix>& images) const
		{
			dataset::ExtractImages(positiveDataset, images, "DONT_CARE", false);
		}

		void BootstrappingDetectorTrainer::extractNegativeImages(const dataset::Dataset& negativeDataset, std::vector<core::Matrix>& images) const
		{
			dataset::ExtractImages(negativeDataset, images, "FULL_IMAGE", false);
		}

		void BootstrappingDetectorTrainer::extractFeaturesFromPositiveImages(const std::vector<core::Matrix>& images, core::Matrix& features, const size_t numSamples) const
		{
			auto featureExtractor = params_.featureExtractor;
			size_t featureLenght = featureExtractor->lenght();


			features.create(0, featureLenght, CV_32F);
			features.reserve(numSamples);

			size_t count = 0;
			for (const core::Matrix& image : images)
			{
				core::FeatureVector features = featureExtractor->compute(image);
				cv::Mat_<float> fv(features);
				cv::Mat_<float> fv0 = fv.reshape(1, 1);


				features.push_back(fv0);

				count += 1;
				if (count == numSamples) {
					break; // throw done_collecting_samples();
				}
			}

		}

		void BootstrappingDetectorTrainer::extractFeaturesFromNegativeImages(const std::vector<core::Matrix>& images, core::Matrix& features, const size_t numSamples, classification::Classifier* classifier) const
		{
			auto featureExtractor = params_.featureExtractor;
			auto imageScanner = params_.imageScanner;
			auto pyramidBuilder = params_.pyramidBuilder;
			size_t featureLenght = featureExtractor->lenght();
			

			features.create(0, featureLenght, CV_32F);
			features.reserve(numSamples);

			try
			{

				if (!classifier)
				{
					int windows_to_keep_per_image = 10;

					int count = 0;
					for (const core::Matrix& image : images)
					{
	
						auto windows = imageScanner->compute(image);

						std::random_shuffle(std::begin(windows), std::end(windows));
						windows.resize(windows_to_keep_per_image);

						for (auto& window : windows) {
							core::FeatureVector features = featureExtractor->compute(window.image);
							cv::Mat_<float> fv(features);
							cv::Mat_<float> fv0 = fv.reshape(1, 1);
							features.push_back(fv0);
							count += 1;
							if (count == numSamples) {
								throw done_collecting_samples();
							}
						}

					}

				}
				else
				{
					int count = 0;
					for (const core::Matrix& image : images)
					{

						pyramid::Pyramid pyramid = pyramidBuilder->Build(image);
						int pyramid_num_levels = pyramid.GetNumLevels();


						for (int pyramid_level_index = 0; pyramid_level_index < pyramid_num_levels; pyramid_level_index++)
						{
							const pyramid::PyramidLevel& pyramid_level = pyramid.GetLevel(pyramid_level_index);

							auto windows =
								imageScanner->compute(pyramid_level.GetImage());

							for (auto& window : windows)
							{
								core::FeatureVector features = featureExtractor->compute(window.image);

								if (classifier->Predict(features))
								{
									cv::Mat_<float> fv(features);
									cv::Mat_<float> fv0 = fv.reshape(1, 1);
									features.push_back(fv0);
									count += 1;
									if (count == numSamples) {
										throw done_collecting_samples();
									}
								}
							}
						}

					}
				}

			}
			catch (done_collecting_samples&) {}
		}

		core::Matrix BootstrappingDetectorTrainer::extractPositives(const dataset::Dataset& positive) const
		{
			auto featureExtractor = params_.featureExtractor;
			size_t featureLenght = featureExtractor->lenght();
			size_t numSamples = params_.numPositives;

			core::Matrix X;
			X.create(0, featureLenght, CV_32F);
			X.reserve(numSamples);

			try {

				size_t count = 0;
				for (auto& dataset_im : positive.images)
				{
					auto& filename = dataset_im.filename;
					core::Matrix image = core::imread(filename, false);

					for (auto& box : dataset_im.boxes) {

						core::Box roi(box.rect.left(), box.rect.top(), box.rect.width(), box.rect.height());

						if (roi.size() != roi.size()) {
							// [TODO:] resize
							continue;
						}

						core::Matrix image0 = image(roi);
						core::FeatureVector features = featureExtractor->compute(image0);
						cv::Mat_<float> fv(features);
						cv::Mat_<float> fv0 = fv.reshape(1, 1);


						X.push_back(fv0);

						count += 1;
						if (count == numSamples) {
							throw done_collecting_samples();
						}

					}
				}

			}
			catch (done_collecting_samples&) {}

			return X;
		}

		core::Matrix BootstrappingDetectorTrainer::extractNegatives(const dataset::Dataset& negative, classification::Classifier* classifier) const
		{
			auto featureExtractor = params_.featureExtractor;
			auto imageScanner = params_.imageScanner;
			auto pyramidBuilder = params_.pyramidBuilder;
			size_t featureLenght = featureExtractor->lenght();
			size_t numSamples = params_.numNegatives;

			core::Matrix X;
			X.create(0, featureLenght, CV_32F);
			X.reserve(numSamples);

			try
			{

				if (!classifier)
				{
					int windows_to_keep_per_image = 10;

					int count = 0;
					for (auto& dataset_im : negative.images)
					{
						auto& filename = dataset_im.filename;
						core::Matrix image = core::imread(filename, false);

						auto windows =
							imageScanner->compute(image);

						std::random_shuffle(std::begin(windows), std::end(windows));
						windows.resize(windows_to_keep_per_image);

						for (auto& window : windows) {
							core::FeatureVector features = featureExtractor->compute(window.image);
							cv::Mat_<float> fv(features);
							cv::Mat_<float> fv0 = fv.reshape(1, 1);
							X.push_back(fv0);
							count += 1;
							if (count == numSamples) {
								throw done_collecting_samples();
							}
						}

					}

				}
				else
				{
					int count = 0;
					for (auto& dataset_im : negative.images)
					{
						auto& filename = dataset_im.filename;
						core::Matrix image = core::imread(filename, false);

						pyramid::Pyramid pyramid = pyramidBuilder->Build(image);
						int pyramid_num_levels = pyramid.GetNumLevels();


						for (int pyramid_level_index = 0; pyramid_level_index < pyramid_num_levels; pyramid_level_index++)
						{
							const pyramid::PyramidLevel& pyramid_level = pyramid.GetLevel(pyramid_level_index);

							auto windows =
								imageScanner->compute(pyramid_level.GetImage());

							for (auto& window : windows)
							{
								core::FeatureVector features = featureExtractor->compute(window.image);

								if (classifier->Predict(features))
								{
									cv::Mat_<float> fv(features);
									cv::Mat_<float> fv0 = fv.reshape(1, 1);
									X.push_back(fv0);
									count += 1;
									if (count == numSamples) {
										throw done_collecting_samples();
									}
								}
							}
						}

					}
				}

			}
			catch (done_collecting_samples&) {}

			return X;
		}

		Detector* BootstrappingDetectorTrainer::buildDetector(classification::Classifier* classifier)
		{
			std::stringstream ss;

			{
				auto pyramidBuilder = params_.pyramidBuilder.get();
				auto imageScanner = params_.imageScanner.get();
				auto featureExtractor = params_.featureExtractor.get();
				auto nonMaximaSuppressor = params_.nonMaxSupperssor.get();


				core::oarchive oa(ss);
				oa << pyramidBuilder;
				oa << imageScanner;
				oa << featureExtractor;
				oa << nonMaximaSuppressor;
				oa << classifier;
			}

			{
				pyramid::PyramidBuilder* pyramidBuilder;
				image_scanning::ImageScanner* imageScanner;
				feature_extraction::FeatureExtractor* featureExtractor;
				non_maxima_suppression::NonMaximaSuppressor* nonMaximaSuppressor;
				classification::Classifier* classifier0;

				core::iarchive ia(ss);
				ia >> pyramidBuilder;
				ia >> imageScanner;
				ia >> featureExtractor;
				ia >> nonMaximaSuppressor;
				ia >> classifier0;

				DetectorBaseMT_Builder detectorBuilder;
				detectorBuilder.PutPyramidBuilder(std::shared_ptr<pyramid::PyramidBuilder>(pyramidBuilder));
				detectorBuilder.PutImageScanner(std::shared_ptr<image_scanning::ImageScanner>(imageScanner));
				detectorBuilder.PutFeatureExtractor(std::shared_ptr<feature_extraction::FeatureExtractor>(featureExtractor));
				detectorBuilder.PutNonMaximaSuppressor(std::shared_ptr<non_maxima_suppression::NonMaximaSuppressor>(nonMaximaSuppressor));
				detectorBuilder.PutClassifier(std::shared_ptr<classification::Classifier>(classifier0));


				std::unique_ptr<detection::Detector> detector{ detectorBuilder.Build() };

				return detector.release();
			}
		}

		void BootstrappingDetectorTrainer::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

		void BootstrappingDetectorTrainer::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

	}
}