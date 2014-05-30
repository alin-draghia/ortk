// train-person-detector-2.cpp : Defines the entry point for the console application.
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

#include <object-recognition-toolkit/object_recognition_toolkit.h>

using object_recognition_toolkit::classification::Trainer;
using object_recognition_toolkit::classification::Classifier;
using object_recognition_toolkit::feature_extraction::FeatureExtractor;
using object_recognition_toolkit::image_scanning::ImageScanner;
using object_recognition_toolkit::pyramid::PyramidBuilder;
using object_recognition_toolkit::pyramid::PyramidLevel;
using object_recognition_toolkit::non_maxima_suppression::NonMaximaSuppressor;
using object_recognition_toolkit::detection::Detector;
using object_recognition_toolkit::core::FeatureVector;

namespace fs = std::tr2::sys;



void test_trainer_interface()
{
	auto dataset_dir = fs::path{ "./../../datasets/INRIAPerson" };
	auto train_dir = dataset_dir / fs::path{ "train_64x128_H96" };
	auto positive_dir = train_dir / fs::path{ "pos" };
	auto negative_dir = train_dir / fs::path{ "neg" };

	object_recognition_toolkit::dataset::Dataset positives, negatives;

	std::vector<std::string> positive_filenames;
	for (auto it = fs::directory_iterator(positive_dir); it != fs::directory_iterator(); it++) {
		positive_filenames.push_back(it->path());
	}

	std::vector<std::string> negative_filenames;
	for (auto it = fs::directory_iterator(negative_dir); it != fs::directory_iterator(); it++) {
		negative_filenames.push_back(it->path());
	}

	object_recognition_toolkit::dataset::LoadDatasetFiles(positive_filenames, positives);
	object_recognition_toolkit::dataset::LoadDatasetFiles(negative_filenames, negatives);

	object_recognition_toolkit::dataset::Box positive_box;
	positive_box.rect.set_left(15);
	positive_box.rect.set_top(15);
	positive_box.rect.set_right(15 + 64 - 1);
	positive_box.rect.set_bottom(15 + 128 - 1);

	auto w = positive_box.rect.width();
	auto h = positive_box.rect.height();

	for (auto& positive_image : positives.images)
	{
		positive_image.boxes.push_back(positive_box);
	}

	auto trainerParams = object_recognition_toolkit::detection::DallalTriggsDetectorTrainer::Params();
	trainerParams.data_directory = "./train_data_v1/";
	trainerParams.numStages = 5;
	trainerParams.numPositives = 3000;
	trainerParams.numNegatives = 3000;
	trainerParams.trainingSize = { 64, 128 };
	trainerParams.pyramidBuilder.reset(
		new object_recognition_toolkit::pyramid::FloatPyramidBuilder(1.2, {}, {})
		);
	trainerParams.imageScanner.reset(
		new object_recognition_toolkit::image_scanning::DenseImageScanner({ 64, 128 }, { 8, 8 }, { 0, 0 })
		);
	trainerParams.featureExtractor.reset(
		new object_recognition_toolkit::feature_extraction::HogFeatureExtractor()
		);
	trainerParams.trainer.reset(
		new object_recognition_toolkit::classification::LinearSvcTrainer()
		);
	trainerParams.nonMaxSupperssor.reset(
		new object_recognition_toolkit::non_maxima_suppression::PassThroughNms()
		);

	std::unique_ptr<object_recognition_toolkit::detection::DetectorTrainer> trainer(
		new object_recognition_toolkit::detection::DallalTriggsDetectorTrainer(trainerParams)
		);

	trainer->Train(positives, negatives);
}

void test_dataset_loader()
{
	auto base_path = R"(..\..\datasets\INRIAPerson)";
	auto train_metadata_dir = R"(..\..\datasets\INRIAPerson\Train\annotations)";

	std::vector<std::string> annotations_filenames;
	for (auto it = fs::directory_iterator(train_metadata_dir); it != fs::directory_iterator(); it++) {
		annotations_filenames.push_back(it->path());
	}

	object_recognition_toolkit::dataset::Dataset dataset;

	object_recognition_toolkit::dataset::LoadDatasetPascalV1(annotations_filenames, dataset, base_path);

	for (auto& im : dataset.images) {
		auto image = object_recognition_toolkit::core::imread(im.filename);

		for (auto& box : im.boxes) {
			cv::Rect rect(box.rect.left(), box.rect.top(), box.rect.width(), box.rect.height());
			cv::rectangle(image, rect, CV_RGB(255, 0, 0));
		}

		object_recognition_toolkit::core::imshow(image);
		int key = object_recognition_toolkit::core::show();
		if (key == 27) {
			break;
		}
	}

	object_recognition_toolkit::dataset::SaveDatasetDlib("mydataset.xml", dataset);

	int odigshod = 0;
}

void test_detectror_class()
{
	{
		std::unique_ptr<Detector> detector
		{
			new object_recognition_toolkit::detection::DetectorBaseMt
			{
				new object_recognition_toolkit::pyramid::FloatPyramidBuilder{ 1.2, { 64, 128 }, { 0, 0 } },
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



void test_detector()
{
	const std::string test_samples_directory = "./../../datasets/INRIAPerson/Test/pos";
	std::vector<std::string> test_samples_filenames;

	for (auto it = fs::directory_iterator(fs::path(test_samples_directory)); it != fs::directory_iterator(); it++) {
		test_samples_filenames.push_back(it->path());
	}


	object_recognition_toolkit::
		dataset::Dataset test_dataset;

	object_recognition_toolkit::dataset::LoadDatasetFiles(test_samples_filenames, test_dataset);

	const std::string detector_filename = "train_data_v1/detector-stage-4.dat";
	std::unique_ptr<Detector> detector;
	{
		std::ifstream ifs(detector_filename);
		bool is_open = ifs.is_open();
		object_recognition_toolkit::core::iarchive ia(ifs);
		ia >> detector;
	}

	(void)detector;

	for (auto& test_image : test_dataset.images)
	{
		auto im = object_recognition_toolkit::core::imread(test_image.filename, false);
		std::vector<cv::Rect> dets;
		std::vector<double> confs;
		detector->detect(im, dets, confs, 2.0);

		auto disp = object_recognition_toolkit::core::imread(test_image.filename, true);
		for (auto& det : dets) {
			cv::rectangle(disp, det, CV_RGB(255, 0, 0));
		}

		object_recognition_toolkit::core::imshow(disp);
		object_recognition_toolkit::core::show();


	}

}


void test_detector2()
{
	const std::string test_samples_directory = "./../../datasets/INRIAPerson/Test/pos";
	std::vector<std::string> test_samples_filenames;

	for (auto it = fs::directory_iterator(fs::path(test_samples_directory)); it != fs::directory_iterator(); it++) {
		test_samples_filenames.push_back(it->path());
	}


	object_recognition_toolkit::
		dataset::Dataset test_dataset;

	object_recognition_toolkit::dataset::LoadDatasetFiles(test_samples_filenames, test_dataset);




	std::unique_ptr<Detector> detector{
		new object_recognition_toolkit::detection::DetectorBaseMt{
			new object_recognition_toolkit::pyramid::FloatPyramidBuilder(1.05, {}, {}),
			new object_recognition_toolkit::image_scanning::DenseImageScanner{ { 64, 128 }, { 8, 8 }, {} },
			new object_recognition_toolkit::feature_extraction::HogFeatureExtractor{},
			new object_recognition_toolkit::classification::MockPersonClassifier{},
			new object_recognition_toolkit::non_maxima_suppression::PassThroughNms{}
		}
	};

	{
		std::ofstream ofs("mock_person_detector");
		object_recognition_toolkit::core::oarchive oa(ofs);
		oa << detector;
	}


	for (auto& test_image : test_dataset.images)
	{
		auto im = object_recognition_toolkit::core::imread(test_image.filename, false);
		std::vector<cv::Rect> dets;
		std::vector<double> confs;
		detector->detect(im, dets, confs, 0.0);

		auto disp = object_recognition_toolkit::core::imread(test_image.filename, true);
		for (auto& det : dets) {
			cv::rectangle(disp, det, CV_RGB(255, 0, 0));
		}

		object_recognition_toolkit::core::imshow(disp);
		object_recognition_toolkit::core::show();

	}

}

int _tmain(int argc, _TCHAR* argv[])
{


	test_detector();
	return 0;

	test_trainer_interface();
	return 0;

	test_dataset_loader();
	return 0;

	test_detectror_class();
	return 0;

}