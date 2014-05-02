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

using object_recognition_toolkit::classification::Classifier;
using object_recognition_toolkit::feature_extraction::FeatureExtractor;
using object_recognition_toolkit::image_scanning::ImageScanner;
using object_recognition_toolkit::pyramid::ImagePyramid;
using object_recognition_toolkit::pyramid::PyramidLevel;

namespace fs = std::tr2::sys;


std::unique_ptr<Classifier> first_pass(const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files);
std::unique_ptr<Classifier> second_pass(std::unique_ptr<Classifier>& first_pass_classifier, const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files);

void run_classifier_over_test_images(std::unique_ptr<Classifier>& first_pass_classifier, const std::vector<fs::path>& test_image_files);

int _tmain(int argc, _TCHAR* argv[])
{
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

	auto first_stage_classifier = first_pass(positive_sample_files, negative_sample_files);
	auto final_classifier = second_pass(first_stage_classifier, positive_sample_files, negative_sample_files);


	auto test_dir = dataset_dir / fs::path("Test") / fs::path("pos");
	std::vector<fs::path> test_image_files;
	for (auto it = fs::directory_iterator(test_dir); it != fs::directory_iterator(); it++) {
		test_image_files.push_back(it->path());
	}

	run_classifier_over_test_images(final_classifier, test_image_files);

	return 0;
}

std::unique_ptr<Classifier> first_pass(const std::vector<fs::path>& positive_sample_files, const std::vector<fs::path>& negative_sample_files)
{
	std::clog << "Begin stage 1..." << std::endl;
	if (fs::exists(fs::path{ "first.stage.in" })) {
		std::clog << "Skip feature extraction..." << std::endl;
	}
	else {

		std::clog << std::endl;
		std::clog << "Extracting features..." << std::endl;

		const int num_samples_per_negative_image = 10;
		const int num_positive_samples = positive_sample_files.size();
		const int num_negative_samples = negative_sample_files.size() * num_samples_per_negative_image;
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
		std::vector<std::vector<float>> X;
		std::vector<float> y;
		X.reserve(num_samples);
		y.reserve(num_samples);



		size_t current = 0;

		// positive samples
		for (const fs::path& im_path : positive_sample_files) {
			cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
			cv::Mat roi = im({ 15, 15, 64, 128 }).clone();
			std::vector<float> features = feature_extractor->compute(roi);
			X.push_back(features);
			y.push_back(1.0);

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
				y.push_back(-1.0);

				if ((current % 100) == 0) {
					std::clog << "progress " << current << "/" << num_samples << std::endl;
				}
				current++;

			}
		}

		std::clog << "progress " << current << "/" << num_samples << std::endl;

		std::clog << std::endl;
		std::clog << "Writing first.stage.in ..." << std::endl;

		std::ofstream ofs;
		ofs.open("first.stage.in", std::ios::out);
		if (!ofs.is_open()) {
			std::cerr << "failed to open file for writing: " << "first.stage.in" << std::endl;
		}

		for (size_t i = 0; i < X.size(); i++) {
			float label = y[i];
			std::vector<float>& sample = X[i];
			ofs << label;
			for (const float& f : sample) {
				ofs << ", " << f;
			}
			ofs << std::endl;
		}

	}


	std::clog << std::endl;
	std::clog << "Training..." << std::endl;

	if (fs::exists(fs::path{ "first.stage.intercept_.out" }) && 
		fs::exists(fs::path{ "first.stage.coef_.out" })) {
		std::clog << "Skip training..." << std::endl;
	} else {
		int ret_code = std::system("python train_first_stage.py");
		if (ret_code != 0) {
			throw std::runtime_error("Error occured while training...");
		}
	}

	std::clog << std::endl;
	std::clog << "Load model..." << std::endl;

	float bias = 0.0f;
	std::vector<float> coefs;

	std::ifstream fid;

	fid.open("first.stage.intercept_.out", std::ios::in);
	fid >> bias;
	fid.close();

	fid.open("first.stage.coef_.out", std::ios::in);

	std::copy(std::istream_iterator<float>(fid), std::istream_iterator<float>(), std::back_inserter(coefs));

	std::unique_ptr<Classifier> first_pass_classifier {
		new object_recognition_toolkit::classification::LinearSVC{ bias, coefs }
	};

	return first_pass_classifier;
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


	if (fs::exists(fs::path{ "second.stage.in" })) {
		std::clog << "Skip feature extraction..." << std::endl;
	}
	else {
		// dataset
		std::vector<std::vector<float>> X;
		std::vector<float> y;

		size_t current = 0;

		// positive samples
		for (const fs::path& im_path : positive_sample_files) {
			cv::Mat im = cv::imread(im_path, cv::IMREAD_GRAYSCALE);
			cv::Mat roi = im({ 15, 15, 64, 128 }).clone();
			std::vector<float> features = feature_extractor->compute(roi);
			X.push_back(features);
			y.push_back(1.0);

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
					X.push_back(features);
					y.push_back(-1.0);

					if ((current % 100) == 0) {
						std::clog << "progress " << current << std::endl;
					}
					current++;

				}
			}
		}

		std::clog << "progress " << current << std::endl;

		std::clog << std::endl;
		std::clog << "Writing second.stage.in ..." << std::endl;

		std::ofstream ofs;
		ofs.open("second.stage.in", std::ios::out);
		if (!ofs.is_open()) {
			std::cerr << "failed to open file for writing: " << "second.stage.in" << std::endl;
		}

		for (size_t i = 0; i < X.size(); i++) {
			float label = y[i];
			std::vector<float>& sample = X[i];
			ofs << label;
			for (const float& f : sample) {
				ofs << ", " << f;
			}
			ofs << std::endl;
		}

	}

	std::clog << std::endl;
	std::clog << "Training..." << std::endl;

	if (fs::exists(fs::path{ "second.stage.intercept_.out" }) &&
		fs::exists(fs::path{ "second.stage.coef_.out" })) {
		std::clog << "Skip training..." << std::endl;
	}
	else {
		int ret_code = std::system("python train_second_stage.py");
		if (ret_code != 0) {
			throw std::runtime_error("Error occured while training...");
		}
	}


	float bias = 0.0f;
	std::vector<float> coefs;

	std::ifstream fid;

	fid.open("second.stage.intercept_.out", std::ios::in);
	fid >> bias;
	fid.close();

	fid.open("second.stage.coef_.out", std::ios::in);

	std::copy(std::istream_iterator<float>(fid), std::istream_iterator<float>(), std::back_inserter(coefs));

	std::unique_ptr<Classifier> second_pass_classifier{
		new object_recognition_toolkit::classification::LinearSVC{ bias, coefs }
	};

	return second_pass_classifier;
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