#include "object-recognition-toolkit/detection/dallal_triggs_detector_trainer.h"
#include "object-recognition-toolkit/detection/detector_base_mt.h"

namespace object_recognition_toolkit
{
	namespace detection
	{
		DallalTriggsDetectorTrainer::DallalTriggsDetectorTrainer()
		{
		}

		DallalTriggsDetectorTrainer::DallalTriggsDetectorTrainer(const Params& params)
			: params_(params)
		{
		}

		DallalTriggsDetectorTrainer::~DallalTriggsDetectorTrainer()
		{
		}

		const std::string& DallalTriggsDetectorTrainer::name() const
		{
			static const std::string name = "DallalTriggsDetectorTrainer";
			return name;
		}

		Detector* DallalTriggsDetectorTrainer::Train(const dataset::Dataset& positive, const dataset::Dataset& negative)
		{



			std::unique_ptr<classification::Classifier> classifier;
			auto trainer = params_.trainer;

			core::Matrix X_pos = extractPositives(positive);
			core::Matrix X_neg, X_neg_total;

			auto numStages = params_.numStages;

			for (size_t stage = 0; stage < numStages; stage++)
			{

				std::cerr << "Training stage " << std::to_string(stage) << " ... ";

				X_neg = extractNegatives(negative, classifier.get());
				X_neg_total.push_back(X_neg);

				int pos_count = X_pos.rows;
				int neg_count = X_neg_total.rows;

				core::Matrix X, y;
				X.create(0, X_pos.cols, CV_32F);
				X.reserve(pos_count + neg_count);
				y.create(0, 1, CV_32F);
				y.reserve(pos_count + neg_count);

				X.push_back(X_pos);
				X.push_back(X_neg_total);
				y.push_back(core::Matrix::ones(pos_count, 1, CV_32F) * 1.0f);
				y.push_back(core::Matrix::ones(neg_count, 1, CV_32F) * -1.0f);


				classifier.reset(
					trainer->Train(X, y)
					);

				std::cerr << "done" << std::endl;
			}

			std::stringstream ss;
			{
				auto pyramidBuilder = params_.pyramidBuilder.get();
				auto imageScanner = params_.imageScanner.get();
				auto featureExtractor = params_.featureExtractor.get();
				auto nonMaximaSuppressor = params_.nonMaxSupperssor.get();
				auto classifier0 = classifier.get();

				core::oarchive oa(ss);
				oa << pyramidBuilder;
				oa << imageScanner;
				oa << featureExtractor;
				oa << nonMaximaSuppressor;
				oa << classifier0;
			}

			{
				pyramid::ImagePyramid* pyramidBuilder;
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

				std::unique_ptr<detection::Detector> detector{
					new DetectorBaseMt(pyramidBuilder,
						imageScanner,
						featureExtractor,
						classifier0,
						nonMaximaSuppressor
					)
				};

				return detector.release();

			}

		}

		struct done_collecting_samples{};

		core::Matrix DallalTriggsDetectorTrainer::extractPositives(const dataset::Dataset& positive) const
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

		core::Matrix DallalTriggsDetectorTrainer::extractNegatives(const dataset::Dataset& negative, classification::Classifier* classifier) const
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

						std::vector<cv::Mat> windows;
						imageScanner->ScanImage(image, windows, std::vector<cv::Rect>());

						std::random_shuffle(std::begin(windows), std::end(windows));
						windows.resize(windows_to_keep_per_image);

						for (auto& window : windows) {
							core::FeatureVector features = featureExtractor->compute(window);
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

						auto pyramid = pyramidBuilder->Build(image);

						for (auto& pyramidLevel : pyramid)
						{
							std::vector<cv::Mat> windows;
							imageScanner->ScanImage(pyramidLevel.GetImage(), windows, std::vector<cv::Rect>());

							for (auto& window : windows)
							{
								core::FeatureVector features = featureExtractor->compute(window);

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


	}
}