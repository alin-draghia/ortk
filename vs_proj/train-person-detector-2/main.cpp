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

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

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

using object_recognition_toolkit::classification::Trainer;
using object_recognition_toolkit::classification::Classifier;
using object_recognition_toolkit::feature_extraction::FeatureExtractor;
using object_recognition_toolkit::image_scanning::ImageScanner;
using object_recognition_toolkit::pyramid::ImagePyramid;
using object_recognition_toolkit::pyramid::PyramidLevel;

namespace fs = std::tr2::sys;

void save_classifier(const std::string& filename, const std::unique_ptr<Classifier>& cls);
std::unique_ptr<Classifier> load_classifier(const std::string& filename);

void test_dummy_serialization();

std::unique_ptr<Classifier> first_pass(const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files);
std::unique_ptr<Classifier> second_pass(std::unique_ptr<Classifier>& first_pass_classifier, const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files);

void run_classifier_over_test_images(std::unique_ptr<Classifier>& first_pass_classifier, const std::vector<fs::path>& test_image_files);

int _tmain(int argc, _TCHAR* argv[])
{

	test_dummy_serialization();
	// two pass object detector traing

	// STEP 1. Extract features from all the training positive samples
	// STEP 2. Extract features from 10 random selected windows from the training negative samples
	// STEP 3. Train liniar svm model > 1st-pass.model
	// STEP 4. Using '1st-pass.model' extract positivly classified windows from the training negative
	// samples, augment the the dataset
	// STEP 5. Train liniar svm model > 2nd-pass.model

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


	auto first_stage_classifier = first_pass(positive_sample_files, negative_sample_files);
	save_classifier("person-detector.stage1.svm", first_stage_classifier);
	auto first_stage_classifier0 = load_classifier("person-detector.stage1.svm");



	auto final_classifier = second_pass(first_stage_classifier, positive_sample_files, negative_sample_files);
	save_classifier("person-detector.stage2.svm", final_classifier);
	auto second_stage_classifier0 = load_classifier("person-detector.stage2.svm");

	run_classifier_over_test_images(final_classifier, test_image_files);

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

	cv::Mat_<float> X;
	cv::Mat_<float> y;


	size_t current = 0;

	// positive samples
	for (const fs::path& im_path : positive_sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
		cv::Mat roi = im({ 15, 15, 64, 128 }).clone();
		std::vector<float> features = feature_extractor->compute(roi);
		X.push_back(features);
		y.push_back(1.0f);

		if ((current % 100) == 0) {
			std::clog << "progress " << current << "/" << num_samples << std::endl;
		}
		current++;
	}

	// negative samples
	for (const fs::path& im_path : positive_sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
		std::vector<cv::Mat> windows;
		image_scanner->ScanImage(im, windows, std::vector<cv::Rect>());

		std::random_shuffle(windows.begin(), windows.end());
		windows.resize(num_samples_per_negative_image);

		for (const cv::Mat& window : windows) {
			std::vector<float> features = feature_extractor->compute(window);
			X.push_back(features);
			y.push_back(-1.0f);

			if ((current % 100) == 0) {
				std::clog << "progress " << current << "/" << num_samples << std::endl;
			}
			current++;

		}
	}

	std::clog << "progress " << current << "/" << num_samples << std::endl;






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

	size_t current = 0;

	// positive samples
	for (const fs::path& im_path : positive_sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
		cv::Mat roi = im({ 15, 15, 64, 128 }).clone();
		std::vector<float> features = feature_extractor->compute(roi);
		X.push_back(features);
		y.push_back(1.0f);

		if ((current % 100) == 0) {
			std::clog << "progress " << current << std::endl;
		}
		current++;
	}


	std::clog << "Mining hard negatives" << std::endl;

	// negative samples
	for (const fs::path& im_path : positive_sample_files) {
		cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);

		std::vector<PyramidLevel> pyramid = pyramid_builder->Build(im);

		for (PyramidLevel& level : pyramid) {

			std::vector<cv::Mat> windows;
			image_scanner->ScanImage(level.GetImage(), windows, std::vector<cv::Rect>());

			for (const cv::Mat& window : windows) {
				std::vector<float> features = feature_extractor->compute(window);
				if (first_pass_classifier->PredictConf(features) > 0.0) {
					X.push_back(features);
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


void run_classifier_over_test_images(std::unique_ptr<Classifier>& classifier, const std::vector<fs::path>& test_image_files)
{

	fs::path result_dir("./out");
	fs::create_directory(result_dir);

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

				double conf = classifier->PredictConf(features);

				if (conf > 0.5) {
					detection_boxes.push_back(pyramid_level.Invert(boxes[i]));
					detection_confs.push_back(conf);
				}
			}
		}

		for (auto& box : detection_boxes) {
			cv::rectangle(disp, box, CV_RGB(255, 0, 0));
		}


		fs::path out_file = result_dir / fs::path{ image_file.filename() };
		cv::imwrite(out_file, disp);
		cv::imshow("results", disp);
		int k = cv::waitKey(1);
		if (k == 27) {
			break;
		}
	}
}

void save_classifier(const std::string& filename, const std::unique_ptr<Classifier>& cls)
{
	if (!cls)
		throw std::invalid_argument("cls is null");

	std::ofstream ofs(filename);
	boost::archive::text_oarchive oa(ofs);

	Classifier* p = cls.get();
	oa << p;
}


std::unique_ptr<Classifier> load_classifier(const std::string& filename)
{
	std::ifstream ifs(filename);
	boost::archive::text_iarchive ia(ifs);

	Classifier* cls = nullptr;
	ia >> cls;

	return std::unique_ptr<Classifier>(cls);
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
	std::unique_ptr<Classifier> cls(trainer.Train(X, y));

	save_classifier("dummy.cls", cls);

	auto cls0 = load_classifier("dummy.cls");
}