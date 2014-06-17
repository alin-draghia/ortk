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

		BootstrappingDetectorTrainer::~BootstrappingDetectorTrainer()
		{
		}


		boost::shared_ptr<DetectorTrainer> BootstrappingDetectorTrainer::Clone() const
		{
			return boost::shared_ptr<DetectorTrainer>(new BootstrappingDetectorTrainer(*this));
		}

		boost::shared_ptr<Detector> BootstrappingDetectorTrainer::TrainWithDataset(const dataset::Dataset& positive, const dataset::Dataset& negative)
		{
			core::Matrix X_pos;
			core::Matrix X_neg_total;
			auto num_iterations = this->num_iterations;
			auto num_positives = this->num_positives;
			auto num_negatives = this->num_negatives;
			auto trainer = this->trainer;
			auto feature_extractor = this->feature_extractor;
			auto feature_lenght = feature_extractor->Lenght();


			X_pos.create(0, feature_lenght, CV_32F);
			X_pos.reserve(num_positives);



			X_neg_total.create(0, feature_lenght, CV_32F);
			X_neg_total.reserve(num_negatives * num_iterations);

			std::string& data_directory = this->data_directory;
			if (!fs::exists(fs::path(data_directory))) {
				fs::create_directories(fs::path(data_directory));
			}

			boost::shared_ptr<detection::Detector> detector;
			boost::shared_ptr<classification::Classifier> classifier;

	
			X_pos = extractPositives(positive);
			if (callback) {
				callback->OnDoneCollectiongPositiveSamples(X_pos);
			}

			for (size_t iteration = 0; iteration < num_iterations; iteration++)
			{
				core::Matrix X_neg;
	
				OnBeginIteration(iteration);



				X_neg = extractNegatives(negative, classifier);
				if (callback) {
					callback->OnDoneCollectiongNegativeSamples(X_neg);
				}

				X_neg_total.push_back(X_neg);
				X_neg.release();

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

				classifier = trainer->Train(X, y);
				OnClassifier(classifier);
				
				OnEndIteration(iteration);

			}

			return detector;
		}


		boost::shared_ptr<Detector> BootstrappingDetectorTrainer::TrainWithImages(const std::vector<core::Matrix>& positiveImages, const std::vector<core::Matrix>& negativeImages)
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

			auto trainer = this->trainer;
			auto feature_extractor = this->feature_extractor;

			auto num_iterations = this->num_iterations;
			auto num_positives = this->num_positives;
			auto num_negatives = this->num_negatives;
			const int total_num_negatives = (int)num_negatives * (int)num_iterations;
			const int feature_lenght = (int)feature_extractor->Lenght();

			const std::string& data_directory = this->data_directory;

			X_pos.create(0, feature_lenght, CV_32F);
			X_pos.reserve(num_positives);

			X_neg.create(0, feature_lenght, CV_32F);
			X_neg.reserve(num_negatives);

			X_neg_total.create(0, feature_lenght, CV_32F);
			X_neg_total.reserve(total_num_negatives);

			if (!fs::exists(fs::path(data_directory))) {
				fs::create_directories(fs::path(data_directory));
			}

			boost::shared_ptr<detection::Detector> detector;
			boost::shared_ptr<classification::Classifier> classifier;


			fs::path positive_features_filename
				= fs::path(data_directory) / fs::path("positive_features.yaml");


			if (fs::exists(positive_features_filename)) {
				cv::FileStorage fileStorage;
				fileStorage.open(positive_features_filename, cv::FileStorage::READ);
				fileStorage["features"] >> X_pos;
			}
			else {
				//X_pos = extractPositives(positive);
				extractFeaturesFromPositiveImages(positiveImages, X_pos, num_positives);
				cv::FileStorage fileStorage;
				fileStorage.open(positive_features_filename, cv::FileStorage::WRITE);
				fileStorage << "features" << X_pos;
			}


			for (size_t iteration = 0; iteration < num_iterations; iteration++)
			{
				OnBeginIteration(iteration);

				fs::path stage_negative_features_filename
					= fs::path(data_directory) / fs::path("negative-features-stage-" + std::to_string(iteration) + ".yaml");

				if (fs::exists(stage_negative_features_filename)) {
					cv::FileStorage storage;
					storage.open(stage_negative_features_filename, cv::FileStorage::READ);
					storage["features"] >> X_neg;
				}
				else {
					//X_neg = extractNegatives(negative, classifier.get());
					extractFeaturesFromNegativeImages(positiveImages, X_pos, num_negatives, classifier);
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

				classifier = trainer->Train(X, y);
				OnClassifier(classifier);

				OnEndIteration(iteration);

			}

			return detector;
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
			auto feature_extractor = this->feature_extractor;
			size_t feature_lenght = feature_extractor->Lenght();


			features.create(0, feature_lenght, CV_32F);
			features.reserve(numSamples);

			size_t count = 0;
			for (const core::Matrix& image : images)
			{
				core::FeatureVector features = feature_extractor->Compute(image);
				cv::Mat_<float> fv(features);
				cv::Mat_<float> fv0 = fv.reshape(1, 1);


				features.push_back(fv0);

				count += 1;
				if (count == numSamples) {
					break; // throw done_collecting_samples();
				}
			}

		}

		void BootstrappingDetectorTrainer::extractFeaturesFromNegativeImages(const std::vector<core::Matrix>& images, core::Matrix& features, const size_t numSamples, boost::shared_ptr<classification::Classifier> classifier) const
		{
			auto feature_extractor = this->feature_extractor;
			auto image_scanner = this->image_scanner;
			auto pyramid_builder = this->pyramid_builder;
			size_t feature_lenght = feature_extractor->Lenght();
			

			features.create(0, feature_lenght, CV_32F);
			features.reserve(numSamples);

			try
			{

				if (!classifier)
				{
					int windows_to_keep_per_image = 10;

					int count = 0;
					for (const core::Matrix& image : images)
					{
	
						auto windows = image_scanner->compute(image);

						std::random_shuffle(std::begin(windows), std::end(windows));
						windows.resize(windows_to_keep_per_image);

						for (auto& window : windows) {
							core::FeatureVector features = feature_extractor->Compute(window.image);
							cv::Mat_<float> fv(features);
							cv::Mat_<float> fv0 = fv.reshape(1, 1);
							features.push_back(fv0);
							count += 1;

							if (this->callback) {
								this->callback->OnNegativeSample(count, window.image, features, 0.0);
							}


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

						pyramid::Pyramid pyramid = pyramid_builder->Build(image);
						int pyramid_num_levels = pyramid.GetNumLevels();


						for (int pyramid_level_index = 0; pyramid_level_index < pyramid_num_levels; pyramid_level_index++)
						{
							const pyramid::PyramidLevel& pyramid_level = pyramid.GetLevel(pyramid_level_index);

							auto windows =
								image_scanner->compute(pyramid_level.GetImage());

							for (auto& window : windows)
							{
								core::FeatureVector features = feature_extractor->Compute(window.image);

								double score = classifier->Predict(features);
								if (score > 0.0)
								{
									cv::Mat_<float> fv(features);
									cv::Mat_<float> fv0 = fv.reshape(1, 1);
									features.push_back(fv0);
									count += 1;

									if (this->callback) {
										this->callback->OnNegativeSample(count, window.image, features, score);
									}

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
			auto feature_extractor = this->feature_extractor;
			size_t feature_lenght = feature_extractor->Lenght();
			size_t num_positives = this->num_positives;

			core::Matrix X;
			X.create(0, feature_lenght, CV_32F);
			X.reserve(num_positives);

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

						core::Matrix image0;
						image(roi).copyTo(image0);
						core::FeatureVector features = feature_extractor->Compute(image0);

						if (features.type() != CV_32F) {
							core::FeatureVector fv;
							features.convertTo(fv, CV_32F);
							features = fv.reshape(1, 1);
						}
						else {
							features = features.reshape(1, 1);
						}
						

						X.push_back(features);

						count += 1;
						
						if (this->callback) {
							this->callback->OnPositiveSample(count, image0, features);
						}


						if (count == num_positives) {
							throw done_collecting_samples();
						}

					}
				}

			}
			catch (done_collecting_samples&) {}

			return X;
		}

		core::Matrix BootstrappingDetectorTrainer::extractNegatives(const dataset::Dataset& negative, boost::shared_ptr<classification::Classifier> classifier) const
		{
			auto feature_extractor = this->feature_extractor;
			auto image_scanner = this->image_scanner;
			auto pyramid_builder = this->pyramid_builder;
			size_t feature_lenght = feature_extractor->Lenght();
			size_t num_negatives = this->num_negatives;

			core::Matrix X;
			X.create(0, feature_lenght, CV_32F);
			X.reserve(num_negatives);

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
							image_scanner->compute(image);

						std::random_shuffle(std::begin(windows), std::end(windows));
						windows.resize(windows_to_keep_per_image);

						for (auto& window : windows) {

							auto window_im = window.image.clone();
							core::FeatureVector features = feature_extractor->Compute(window_im);
							if (features.type() != CV_32F) {
								core::FeatureVector fv;
								features.convertTo(fv, CV_32F);
								features = fv.reshape(1, 1);
							}
							else {
								features = features.reshape(1, 1);
							}
							X.push_back(features);
							count += 1;

							if (this->callback) {
								this->callback->OnNegativeSample(count, window_im, features, 0.0);
							}

							if (count == num_negatives) {
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

						pyramid::Pyramid pyramid = pyramid_builder->Build(image);
						int pyramid_num_levels = pyramid.GetNumLevels();

						

						for (int pyramid_level_index = 0; pyramid_level_index < pyramid_num_levels; pyramid_level_index++)
						{


							const pyramid::PyramidLevel& pyramid_level = pyramid.GetLevel(pyramid_level_index);

							auto windows = image_scanner->compute(pyramid_level.GetImage());

							for (auto& window : windows)
							{
								auto window_im = window.image.clone();
								core::FeatureVector feature = feature_extractor->Compute(window_im);
								if((feature.rows * feature.cols) != feature_lenght){
									throw std::runtime_error("ALIN: (feature.rows * feature.cols) != feature_lenght !!!");
								}

								core::FeatureVector fv, fv0;
								if (feature.type() != CV_32F) {
									feature.convertTo(fv, CV_32F);
									feature = fv.reshape(1, 1);
								}
								else {
									feature = feature.reshape(1, 1);
								}
								
								double score = classifier->Predict(feature);

								if (score > 0.0) 
								{
									X.push_back(feature);
									count += 1;

									if (this->callback) {
										this->callback->OnNegativeSample(count, window_im, feature, score);
									}

									if (count == num_negatives) {
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

		void BootstrappingDetectorTrainer::OnBeginIteration(int iteration)
		{
			if (this->callback) { this->callback->OnBeginIteration(iteration); }
		}

		void BootstrappingDetectorTrainer::OnClassifier(boost::shared_ptr<classification::Classifier> classifier)
		{
			if (this->callback) { this->callback->OnClassifier(classifier); }
		}

		void BootstrappingDetectorTrainer::OnEndIteration(int iteration)
		{
			if (this->callback) { this->callback->OnEndIteration(iteration); }
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