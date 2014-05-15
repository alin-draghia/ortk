// train-person-detector-2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


// train-person-detector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <filesystem>
#include <memory>
#include <algorithm>
#include <random>



#include <opencv2/opencv.hpp>

#include <object-recognition-toolkit/feature-extraction/feature-extractor.h>
#include <object-recognition-toolkit/feature-extraction/hog-feature-extractor.h>
#include <object-recognition-toolkit/image-scanning/image-scanner.h>
#include <object-recognition-toolkit/image-scanning/dense-image-scanner.h>
#include <object-recognition-toolkit/image-pyramid/image-pyramid.h>
#include <object-recognition-toolkit/image-pyramid/float-pyramid.h>
#include <object-recognition-toolkit/classification/classifier.h>
#include <object-recognition-toolkit/classification/linear-svc.h>
#include <object-recognition-toolkit/classification/linear-svc-trainer.h>
#include <object-recognition-toolkit/classification/mock-person-classifier.h>
#include <object-recognition-toolkit/non_maxima_suppression/non_maxima_suppressor.h>
#include <object-recognition-toolkit/non_maxima_suppression/pass_through_nms.h>

#include <object-recognition-toolkit/detection/detector.h>
#include <object-recognition-toolkit/detection/detector_base.h>
#include <object-recognition-toolkit/detection/detector_base_mt.h>

using object_recognition_toolkit::classification::Trainer;
using object_recognition_toolkit::classification::Classifier;
using object_recognition_toolkit::feature_extraction::FeatureExtractor;
using object_recognition_toolkit::image_scanning::ImageScanner;
using object_recognition_toolkit::pyramid::ImagePyramid;
using object_recognition_toolkit::pyramid::PyramidLevel;
using object_recognition_toolkit::non_maxima_suppression::NonMaximaSuppressor;
using object_recognition_toolkit::detection::Detector;

void test_detectror_class()
{
	{
		std::unique_ptr<Detector> detector
		{
			new object_recognition_toolkit::detection::DetectorBaseMt
			{
				new object_recognition_toolkit::pyramid::FloatImagePyramid{ 1.2, { 64, 128 }, { 0, 0 } },
				new object_recognition_toolkit::image_scanning::DenseImageScanner{ { 64, 128 }, { 8, 8 }, { 0, 0 } },
				new object_recognition_toolkit::feature_extraction::HogFeatureExtractor{},
				new object_recognition_toolkit::classification::MockPersonClassifier(),
				new object_recognition_toolkit::non_maxima_suppression::PassThroughNms()
			}
		};

		std::ofstream ofs("detector.dat");
		object_recognition_toolkit::core::oarchive oa(ofs);
		oa << detector;
	}

	{
	std::unique_ptr<Detector> detector;
	std::ifstream ifs("detector.dat");
	object_recognition_toolkit::core::iarchive ia(ifs);
	ia >> detector;

	(void)detector;


	cv::Mat disp, im = cv::imread(R"(..\datasets\INRIAPerson\Test\pos\crop001501.png)", cv::IMREAD_GRAYSCALE);
	cv::cvtColor(im, disp, cv::COLOR_GRAY2BGR);
	std::vector<cv::Rect> dets;
	std::vector<double> conf;
	detector->detect(im, dets, conf, 0.0);

	for (cv::Rect& det : dets) {
		cv::rectangle(disp, det, CV_RGB(255, 0, 0), 1);
	}

	cv::imshow("out", disp);
	cv::waitKey();

}


}

namespace fs = std::tr2::sys;



void save_classifier(const std::string& filename, const std::shared_ptr<Classifier>& cls);
std::shared_ptr<Classifier> load_classifier(const std::string& filename);

void test_dummy_serialization();

std::unique_ptr<Classifier> first_pass(const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files);
std::unique_ptr<Classifier> second_pass(std::unique_ptr<Classifier>& first_pass_classifier, const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files);

void run_classifier_over_test_images(std::shared_ptr<Classifier>& first_pass_classifier, const std::vector<fs::path>& test_image_files, const std::string& output_dir = "");

cv::Mat extract_positive_dataset(const std::vector<fs::path>& sample_files,
	std::shared_ptr<FeatureExtractor> feature_extractor,
	int num_samples)
{
	cv::Mat X;
	X.create(0, feature_extractor->lenght(), CV_32F);
	X.reserve(num_samples);
	int count = 0;
	for (auto im_path : sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
		cv::Mat roi = im({ 15, 15, 64, 128 }).clone();
		std::vector<float> features = feature_extractor->compute(roi);
		cv::Mat_<float> fv(features);
		cv::Mat_<float> fv0 = fv.reshape(1, 1);
		X.push_back(fv0);

		count += 1;
		if (count == num_samples) {
			break;
		}
	}

	return X;
}


cv::Mat extract_negative_dataset(const std::vector<fs::path>& sample_files,
	std::shared_ptr<FeatureExtractor> feature_extractor, int num_samples,
	std::shared_ptr<ImagePyramid> pyramid_builder,
	std::shared_ptr<ImageScanner> image_scanner,
	std::shared_ptr<Classifier> classifier)
{
	cv::Mat X;
	X.create(0, feature_extractor->lenght(), CV_32F);
	X.reserve(num_samples);

	if (!classifier) {
		// first stage
		int windows_to_keep_per_image = 10;
		int count = 0;
		for (auto im_path : sample_files) {

			cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
			std::vector<cv::Mat> windows;
			image_scanner->ScanImage(im, windows, std::vector<cv::Rect>());

			std::random_shuffle(windows.begin(), windows.end());
			windows.resize(windows_to_keep_per_image);

			for (const cv::Mat& window : windows) {

				std::vector<float> features = feature_extractor->compute(window);
				cv::Mat_<float> fv(features);
				cv::Mat_<float> fv0 = fv.reshape(1, 1);
				X.push_back(fv0);

				count += 1;
				if (count == num_samples) {
					goto done;
				}

			}

		}

	}
	else {

		int count = 0;
		for (auto im_path : sample_files) {

			cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);

			std::vector<PyramidLevel> pyramid = pyramid_builder->Build(im);

			for (PyramidLevel& level : pyramid) {

				std::vector<cv::Mat> windows;
				image_scanner->ScanImage(level.GetImage(), windows, std::vector<cv::Rect>());

				for (const cv::Mat& window : windows) {

					std::vector<float> features = feature_extractor->compute(window);
					if (classifier->Predict(features) > 0.0) {
						cv::Mat_<float> fv(features);
						cv::Mat_<float> fv0 = fv.reshape(1, 1);
						X.push_back(fv0);

						count += 1;
						if (count == num_samples) {
							goto done;
						}
					}

				}
			}

		}

	}

done:
	return X;
}


std::shared_ptr<Classifier> train_stage(cv::Mat X_pos, cv::Mat X_neg, std::shared_ptr<Trainer> trainer)
{
	cv::Mat X, y, y_pos, y_neg;
	y_pos.create(X_pos.rows, 1, CV_32F);
	y_pos = +1.0f;
	y_neg.create(X_neg.rows, 1, CV_32F);
	y_neg = -1.0f;
	X.push_back(X_neg);
	X.push_back(X_pos);
	y.push_back(y_neg);
	y.push_back(y_pos);

	auto ret = std::shared_ptr<Classifier>(trainer->Train(X, y));

	std::vector<float> pos_sample = X_pos.row(0);
	std::vector<float> neg_sample = X_neg.row(0);

	auto val_pos = ret->Predict(pos_sample);
	std::cout << "val for positive sample=" << val_pos << std::endl;
	auto val_neg = ret->Predict(neg_sample);
	std::cout << "val for negative sample=" << val_neg << std::endl;

	return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
	test_detectror_class();
	return 0;

	// two pass object detector traing

	// STEP 1. Extract features from all the training positive samples
	// STEP 2. Extract features from 10 random selected windows from the training negative samples
	// STEP 3. Train liniar svm model > 1st-pass.model
	// STEP 4. Using '1st-pass.model' extract positivly classified windows from the training negative
	// samples, augment the the dataset
	// STEP 5. Train liniar svm model > 2nd-pass.model

	const int num_pos = 3000;
	const int num_neg = 3000;
	const int num_stages = 5;


	auto dataset_dir = fs::path{ "./../../datasets/INRIAPerson" };
	auto train_dir = dataset_dir / fs::path{ "train_64x128_H96" };
	auto positive_dir = train_dir / fs::path{ "pos" };
	auto negative_dir = train_dir / fs::path{ "neg" };

	std::vector<fs::path> positive_sample_files;
	for (auto it = fs::directory_iterator(positive_dir); it != fs::directory_iterator(); it++) {
		positive_sample_files.push_back(it->path());
	}

	std::vector<fs::path> negative_sample_files;
	for (auto it = fs::directory_iterator(negative_dir); it != fs::directory_iterator(); it++) {
		negative_sample_files.push_back(it->path());
	}

	auto test_dir = dataset_dir / fs::path("Test") / fs::path("pos");
	std::vector<fs::path> test_image_files;
	for (auto it = fs::directory_iterator(test_dir); it != fs::directory_iterator(); it++) {
		test_image_files.push_back(it->path());
	}



	{ // save results for each stage	
#if(1)
		const int num_stages = 5;
		for (int stage = 0; stage < num_stages; stage++) {
			std::string stage_classifier_filename = "stage-" + std::to_string(stage) + ".cls";
			std::string resutl_dir = "stage-" + std::to_string(stage) + "-results";
			if (fs::exists(fs::path(stage_classifier_filename))){
				auto classifier = load_classifier(stage_classifier_filename);
				run_classifier_over_test_images(classifier, test_image_files, resutl_dir);
			}
		}
		return 0;
#endif
	}


	auto feature_extractor = std::shared_ptr<FeatureExtractor>{
		new object_recognition_toolkit::feature_extraction::HogFeatureExtractor{}
	};

	auto image_scanner = std::shared_ptr<ImageScanner>{
		new object_recognition_toolkit::image_scanning::DenseImageScanner{ { 64, 128 }, { 8, 8 }, { 0, 0 } }
	};

	auto pyramid_builder = std::shared_ptr<ImagePyramid>{
		new object_recognition_toolkit::pyramid::FloatImagePyramid{ 1.2, { 64, 128 }, { 0, 0 } }
	};

	auto trainer = std::shared_ptr<Trainer> {
		new object_recognition_toolkit::classification::LinearSvcTrainer{}
	};

	// starting with an empty classifier
	auto classifier = std::shared_ptr<Classifier> {nullptr};

	for (int stage = 0; stage < num_stages; stage++) {

		std::cout << "Training stage " << std::to_string(stage) << " ..." << std::endl;

		cv::Mat X_pos, X_neg;
		std::string stage_dataset_filename = "stage-" + std::to_string(stage) + ".ds.yaml";
		std::string prev_stage_dataset_filename = "stage-" + std::to_string(stage - 1) + ".ds.yaml";

		std::string stage_classifier_filename = "stage-" + std::to_string(stage) + ".cls";
		std::string prev_stage_classifier_filename = "stage-" + std::to_string(stage - 1) + ".cls";

		if (!fs::exists(fs::path(stage_dataset_filename))) {

			std::cout << "Computing features..." << std::endl;

			X_pos = extract_positive_dataset(positive_sample_files, feature_extractor, num_pos);
			X_neg = extract_negative_dataset(negative_sample_files, feature_extractor, num_neg, pyramid_builder, image_scanner, classifier);
			if (fs::exists(fs::path(prev_stage_dataset_filename))) {
				cv::FileStorage fs;
				if (fs.open(prev_stage_dataset_filename, cv::FileStorage::READ)) {
					cv::Mat X_neg_prev;
					fs["X_neg"] >> X_neg_prev;
					X_neg.push_back(X_neg_prev);
				}
			}

			std::cout << "Computed " << std::to_string(X_pos.rows) << " positive samples" << std::endl;
			std::cout << "Computed " << std::to_string(X_neg.rows) << " negative samples" << std::endl;

			cv::FileStorage fs;
			if (fs.open(stage_dataset_filename, cv::FileStorage::WRITE)) {
				fs << "X_pos" << X_pos;
				fs << "X_neg" << X_neg;
			}

		}
		else {
			std::cout << "Loading features..." << std::endl;
			cv::FileStorage fs;
			if (fs.open(stage_dataset_filename, cv::FileStorage::READ)) {
				fs["X_pos"] >> X_pos;
				fs["X_neg"] >> X_neg;
			}
			std::cout << "Computed " << std::to_string(X_pos.rows) << " positive samples" << std::endl;
			std::cout << "Computed " << std::to_string(X_neg.rows) << " negative samples" << std::endl;
		}

		if (!fs::exists(fs::path(stage_classifier_filename))) {
			std::cout << "Training classifier..." << std::endl;
			classifier = train_stage(X_pos, X_neg, trainer);
			std::cout << "done" << std::endl;
			save_classifier(stage_classifier_filename, classifier);
		}
		else {
			std::cout << "Loading classifier..." << std::endl;
			classifier = load_classifier(stage_classifier_filename);
			std::cout << "done" << std::endl;
		}

		std::cout << "Running classifier over test images ..." << std::endl;
		run_classifier_over_test_images(classifier, test_image_files);
		std::cout << "done" << std::endl;
	}

	return 0;
}

std::unique_ptr<Classifier> first_pass(const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files)
{
	std::clog << "Begin stage 1..." << std::endl;


	std::clog << std::endl;
	std::clog << "Extracting features..." << std::endl;

	const int num_samples_per_negative_image = 10;
	const int num_positive_samples = (int)positive_sample_files.size();
	const int num_negative_samples = (int)negative_sample_files.size() * num_samples_per_negative_image;
	const int num_samples = num_positive_samples + num_negative_samples;
	const int feature_size = (64 / 8 - 1) * (128 / 8 - 1) * (4 * 9);

	std::clog << "num_samples_per_negative_image=" << num_samples_per_negative_image << std::endl;
	std::clog << "num_positive_samples=" << num_positive_samples << std::endl;
	std::clog << "num_negative_samples=" << num_negative_samples << std::endl;
	std::clog << "num_samples=" << num_samples << std::endl;
	std::clog << "feature_size=" << feature_size << std::endl;


	// object recognition toolkit
	auto feature_extractor = std::unique_ptr<FeatureExtractor>{
		new object_recognition_toolkit::feature_extraction::HogFeatureExtractor{}
	};

	auto image_scanner = std::unique_ptr<ImageScanner>{
		new object_recognition_toolkit::image_scanning::DenseImageScanner{ { 64, 128 }, { 8, 8 }, { 0, 0 } }
	};

	auto pyramid_builder = std::unique_ptr<ImagePyramid>{
		new object_recognition_toolkit::pyramid::FloatImagePyramid{ 1.2, { 64, 128 }, { 0, 0 } }
	};


	// dataset
	//std::vector<std::vector<float>> X;
	//std::vector<float> y;
	//X.reserve(num_samples);
	//y.reserve(num_samples);

	int num_pos = 10000;
	int num_neg = 10000;

	int descriptor_lenght = feature_extractor->lenght();

	cv::Mat_<float> X;
	cv::Mat_<float> y;

	X.create(0, descriptor_lenght);
	X.reserve(num_pos + num_neg);
	y.create(0, 1);
	y.reserve(num_pos + num_neg);

	size_t current = 0;

	int pos_count = 0;
	// positive samples
	for (const fs::path& im_path : positive_sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
		cv::Mat roi = im({ 15, 15, 64, 128 }).clone();
		std::vector<float> features = feature_extractor->compute(roi);
		cv::Mat_<float> fv(features);
		cv::Mat_<float> fv0 = fv.reshape(1, 1);
		X.push_back(fv0);
		y.push_back(1.0f);

		pos_count += 1;
		if (pos_count == num_pos) {
			break;
		}

		if ((current % 100) == 0) {
			std::clog << "progress " << current << "/" << num_samples << std::endl;
		}
		current++;
	}

	int neg_count = 0;
	// negative samples
	for (const fs::path& im_path : negative_sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
		std::vector<cv::Mat> windows;
		image_scanner->ScanImage(im, windows, std::vector<cv::Rect>());

		std::random_shuffle(windows.begin(), windows.end());
		windows.resize(num_samples_per_negative_image);

		for (const cv::Mat& window : windows) {
			std::vector<float> features = feature_extractor->compute(window);
			cv::Mat_<float> fv(features);
			cv::Mat_<float> fv0 = fv.reshape(1, 1);
			X.push_back(fv0);
			y.push_back(-1.0f);

			neg_count += 1;
			if (neg_count == num_neg) {
				break;
			}

			if ((current % 100) == 0) {
				std::clog << "progress " << current << "/" << num_samples << std::endl;
			}
			current++;

		}
	}

	std::clog << "progress " << current << "/" << num_samples << std::endl;

	cv::FileStorage fs;
	if (fs.open("stage.1.dataset.yml", cv::FileStorage::FORMAT_YAML + cv::FileStorage::WRITE)) {
		fs << "X" << X;
		fs << "y" << y;
		fs.release();
	}





	std::clog << std::endl;
	std::clog << "Training..." << std::endl;

	auto trainer = object_recognition_toolkit::classification::LinearSvcTrainer();
	std::unique_ptr<Classifier> cls(trainer.Train(X, y));

	return cls;
}


std::unique_ptr<Classifier> second_pass(std::unique_ptr<Classifier>& first_pass_classifier, const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files)
{
	std::clog << "Begin stage 2..." << std::endl;

	// object recognition toolkit
	auto feature_extractor = std::unique_ptr<FeatureExtractor>{
		new object_recognition_toolkit::feature_extraction::HogFeatureExtractor{}
	};

	auto image_scanner = std::unique_ptr<ImageScanner>{
		new object_recognition_toolkit::image_scanning::DenseImageScanner{ { 64, 128 }, { 8, 8 }, { 0, 0 } }
	};

	auto pyramid_builder = std::unique_ptr<ImagePyramid>{
		new object_recognition_toolkit::pyramid::FloatImagePyramid{ 1.2, { 64, 128 }, { 0, 0 } }
	};

	std::clog << std::endl;
	std::clog << "Extracting features..." << std::endl;

	// dataset
	cv::Mat_<float> X;
	cv::Mat_<float> y;


	int descriptor_lenght = feature_extractor->lenght();
	int cache_size = 200000;

	X.create(0, descriptor_lenght);
	X.reserve(cache_size);
	y.create(0, 1);
	y.reserve(cache_size);

	size_t current = 0;

	// positive samples
	for (const fs::path& im_path : positive_sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
		cv::Mat roi = im({ 15, 15, 64, 128 }).clone();
		std::vector<float> features = feature_extractor->compute(roi);
		cv::Mat_<float> fv(features);
		cv::Mat_<float> fv0 = fv.reshape(1, 1);
		X.push_back(fv0);
		y.push_back(1.0f);

		if ((current % 100) == 0) {
			std::clog << "progress " << current << std::endl;
		}
		current++;
	}


	std::clog << "Mining hard negatives" << std::endl;

	// negative samples
	for (const fs::path& im_path : negative_sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);

		std::vector<PyramidLevel> pyramid = pyramid_builder->Build(im);

		for (PyramidLevel& level : pyramid) {

			std::vector<cv::Mat> windows;
			image_scanner->ScanImage(level.GetImage(), windows, std::vector<cv::Rect>());

			for (const cv::Mat& window : windows) {
				std::vector<float> features = feature_extractor->compute(window);
				if (first_pass_classifier->Predict(features) > 0.0) {
					cv::Mat_<float> fv(features);
					cv::Mat_<float> fv0 = fv.reshape(1, 1);
					X.push_back(fv0);
					y.push_back(-1.0f);

					if ((current % 100) == 0) {
						std::clog << "progress " << current << std::endl;
					}
					current++;
				}
			}
		}
	}

	std::clog << "progress " << current << std::endl;

	std::clog << std::endl;
	std::clog << "Training..." << std::endl;

	auto trainer = object_recognition_toolkit::classification::LinearSvcTrainer();
	std::unique_ptr<Classifier> cls(trainer.Train(X, y));

	return cls;
}


void run_classifier_over_test_images(std::shared_ptr<Classifier>& classifier,
	const std::vector<fs::path>& test_image_files,
	const std::string& output_dir)
{

	double threshold = 0.1;

	fs::path result_dir(output_dir);
	if (!output_dir.empty()) {
		fs::create_directories(result_dir);
	}

	// object recognition toolkit
	auto feature_extractor = std::unique_ptr<FeatureExtractor>{
		new object_recognition_toolkit::feature_extraction::HogFeatureExtractor{}
	};

	auto image_scanner = std::unique_ptr<ImageScanner>{
		new object_recognition_toolkit::image_scanning::DenseImageScanner{ { 64, 128 }, { 8, 8 }, { 0, 0 } }
	};

	auto pyramid_builder = std::unique_ptr<ImagePyramid>{
		new object_recognition_toolkit::pyramid::FloatImagePyramid{ 1.2, { 64, 128 }, { 0, 0 } }
	};

	for (const auto& image_file : test_image_files) {
		cv::Mat im = cv::imread(image_file, cv::IMREAD_GRAYSCALE);
		cv::Mat disp;
		cv::cvtColor(im, disp, cv::COLOR_GRAY2BGR);

		auto pyramid = pyramid_builder->Build(im);

		std::vector<cv::Rect> detection_boxes;
		std::vector<double> detection_confs;

		for (PyramidLevel& pyramid_level : pyramid) {

			std::vector<cv::Mat> windows;
			std::vector<cv::Rect> boxes;

			image_scanner->ScanImage(pyramid_level.GetImage(), windows, boxes);

			for (size_t i = 0; i < windows.size(); i++) {
				std::vector<float> features = feature_extractor->compute(windows[i]);

				double conf = classifier->Predict(features);

				if (conf > threshold) {
					detection_boxes.push_back(pyramid_level.Invert(boxes[i]));
					detection_confs.push_back(conf);
				}
			}
		}

		for (auto& box : detection_boxes) {
			cv::rectangle(disp, box, CV_RGB(255, 0, 0));
		}


		if (!output_dir.empty()) {
			fs::path out_file = result_dir / fs::path{ image_file.filename() };
			cv::imwrite(out_file, disp);
		}

		cv::imshow("results", disp);
		int k = cv::waitKey(15);
		if (k == 27) {
			cv::destroyWindow("results");
			std::cout << "abort" << std::endl;
			return;
		}
	}
}

void save_classifier(const std::string& filename, const std::shared_ptr<Classifier>& cls)
{
	if (!cls)
		throw std::invalid_argument("cls is null");

	std::ofstream ofs(filename);
	object_recognition_toolkit::core::oarchive oa(ofs);
	Classifier* p = cls.get();
	oa << p;
}


std::shared_ptr<Classifier> load_classifier(const std::string& filename)
{
	std::ifstream ifs(filename);
	object_recognition_toolkit::core::iarchive ia(ifs);

	Classifier* cls = nullptr;
	ia >> cls;

	return std::shared_ptr<Classifier>(cls);
}

void test_dummy_serialization()
{
	cv::Mat_<float> X, y;
	X.push_back(cv::Matx12f{ 1, 0 });
	X.push_back(cv::Matx12f{ 0, 1 });
	X.push_back(cv::Matx12f{ 1, 1 });
	X.push_back(cv::Matx12f{ 0, 0 });
	X = X.reshape(1);

	y.push_back(1);
	y.push_back(1);
	y.push_back(1);
	y.push_back(-1);

	auto trainer = object_recognition_toolkit::classification::LinearSvcTrainer();
	std::shared_ptr<Classifier> cls(trainer.Train(X, y));


	save_classifier("dummy.cls", cls);

	auto cls0 = load_classifier("dummy.cls");
}