#include "stdafx.h"

#include "tests.h"

#include <object-recognition-toolkit/object_recognition_toolkit.h>


namespace ORT = object_recognition_toolkit;



void test_naked_pointer() {
	std::cout << "to naked poiner -> ";
	try {
		ORT::detection::Detector* d;

		std::ifstream ifs("detector_0.dat");
		ORT::core::iarchive ia(ifs);

		ia >> d;

		if (d) {
			delete d;
		}

		std::cout << "PASS" << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "FAIL!" << std::endl;
		std::cerr << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void test_shared_ptr() {
	std::cout << "to shared_ptr -> ";
	try {
		throw std::runtime_error("not impemented!");
		/*
		std::shared_ptr<ORT::detection::Detector> d;

		std::ifstream ifs("detector_0.dat");
		ORT::core::iarchive ia(ifs);

		ia >> d;
		*/
		
		std::cout << "PASS" << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "FAIL!" << std::endl;
		std::cerr << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

void test_unique_ptr() {

	std::cout << "to unique_ptr -> ";
	try {
		std::unique_ptr<ORT::detection::Detector> d;

		std::ifstream ifs("detector_0.dat");
		ORT::core::iarchive ia(ifs);

		ia >> d;

		std::ofstream ofs("detector_1.dat");
		ORT::core::oarchive oa(ofs);
		const ORT::detection::Detector* ptr = d.get();
		oa << ptr;

		std::cout << "PASS" << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "FAIL!" << std::endl;
		std::cerr << typeid(e).name() << std::endl;
		std::cerr << e.what() << std::endl;
	}
}


void test_detector_deserialization()
{
	std::cout << "Testing detector deserialization:" << std::endl;
	test_naked_pointer();
	test_unique_ptr();
	test_shared_ptr();

	std::cout << std::endl << std::endl;
}