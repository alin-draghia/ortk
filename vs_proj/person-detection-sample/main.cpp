// person-detection-sample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// STL
#include <memory>

// OpenCv
#include <opencv2/opencv.hpp>

// Object Recognition Toolkit
#include <object-recognition-toolkit/image-pyramid/float-pyramid.h>
#include <object-recognition-toolkit/feature-extraction/hog-feature-extractor.h>
#include <object-recognition-toolkit/image-scanning/dense-image-scanner.h>
#include <object-recognition-toolkit/classification/mock-person-classifier.h>


int _tmain(int argc, _TCHAR* argv[])
{

	auto person_classifier = std::unique_ptr<object_recognition_toolkit::classification::Classifier>{
		new object_recognition_toolkit::classification::MockPersonClassifier{}
	};

	auto feature_extractor = std::unique_ptr<object_recognition_toolkit::feature_extraction::FeatureExtractor>{
		new object_recognition_toolkit::feature_extraction::HogFeatureExtractor{}
	};
	
	auto image_scanner = std::unique_ptr<object_recognition_toolkit::image_scanning::ImageScanner>{
		new object_recognition_toolkit::image_scanning::DenseImageScanner{ { 64, 128 }, { 8, 8 }, {} }
	};

	auto pyramid_builder = std::unique_ptr<object_recognition_toolkit::pyramid::ImagePyramid>{
		new object_recognition_toolkit::pyramid::FloatImagePyramid{ 1.2, {64, 128}, {} }
	};

	cv::Mat sample_image = cv::imread(R"(d:\DEV.0\object-recognition-toolkit\datasets\INRIAPerson\Test\pos\crop001573.png)", cv::IMREAD_GRAYSCALE);
	cv::Mat display_iamge;
	cv::cvtColor(sample_image, display_iamge, cv::COLOR_GRAY2BGR);


	auto pyramid = pyramid_builder->Build(sample_image);

	std::vector<cv::Rect> detected_boxes;
	std::vector<double> detected_confs;

	for (auto pyramid_levele : pyramid) {

		std::vector<cv::Mat> windows;
		std::vector<cv::Rect> boxes;

		image_scanner->ScanImage(pyramid_levele.GetImage(), windows, boxes);

		for (size_t i = 0; i < windows.size(); i++){
			std::vector<float> features = feature_extractor->compute(windows[i]);

			double conf = person_classifier->PredictConf(features);
			if (conf > 0.5) {
				detected_boxes.push_back(pyramid_levele.Invert(boxes[i]));
				detected_confs.push_back(conf);
			}
		}

	}

	for (size_t i = 0; i < detected_boxes.size(); i++){
		cv::rectangle(display_iamge, detected_boxes[i], CV_RGB(255, 0, 0));
	}

	cv::imshow("results", display_iamge);
	cv::waitKey();

	return 0;
}

