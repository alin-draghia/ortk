// test-feature-extractor-loader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <memory>

#include <Poco/SharedLibrary.h>
#include <Poco/ClassLoader.h>

#include <object-recognition-toolkit/feature-extraction/feature-extractor.h>

namespace FE = object_recognition_toolkit::feature_extraction;

int _tmain(int argc, _TCHAR* argv[])
{
	FE::FeatureExtractorLoader loader;
	loader.loadLibrary("object-recognition-toolkit" + Poco::SharedLibrary::suffix(), FE::Manifest);
	
	for ( auto library : loader ) {
		std::cout << "library: " << library->first << std::endl;
		std::cout << "\t" << "className: " << library->second->className( ) << std::endl;
		for ( auto cls : (*library->second) ) {
			std::cout << "\t\t" << "name:" << cls->name( ) << std::endl;
			std::unique_ptr<FE::FeatureExtractor> p(cls->create( ));

			auto name = p->name( );
		}		
	}

	
		std::cout << std::endl << std::endl;
		std::cout << "Try creating class MockFeatureExtractor..." << std::endl;
		std::unique_ptr<FE::FeatureExtractor> extractor;
		try {
			extractor.reset(loader.create("MockFeatureExtractor"));
		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name( ) << "] = " << e.what( ) << std::endl;
		}

		std::cout << "Enumerating params..." << std::endl;
		try {		
			for ( auto& paramName : extractor->getParamList( ) ) {
				std::cout << "paramName= " << paramName << std::endl;
				std::cout << "paramHelp= " << extractor->getParamHelp(paramName) << std::endl;
				std::cout << "paramType= " << extractor->getParamType(paramName).name() << std::endl;
				
				auto param = extractor->getParam(paramName);
				std::string valStr;
				try {
					valStr = param.toString( );
				} catch ( std::exception& e ) {
					valStr = std::string(typeid(e).name( ));
				}
				std::cout << "paramValue= " << valStr << std::endl;
			}
		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name() << "] = " << e.what() << std::endl;
		}
		
	
		try {
			extractor->setParam("non_existing", 1024);
		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name() << "] = " << e.what() << std::endl;
		}
		

		try {
			extractor->setParam("n", 1024.0);
		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name() << "] = " << e.what() << std::endl;
		}

		try {
			int n = extractor->getParam("n").convert<int>( );
			int v = n;
		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name() << "] = " << e.what() << std::endl;
		}

		try {
			extractor->setParam("n", "512");
		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name() << "] = " << e.what() << std::endl;
		}
		
		try {
			int n = extractor->getParam("n").convert<int>();
			int v = n;
		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name() << "] = " << e.what() << std::endl;
		}
		

		try {
			auto matExpt = (cv::Mat)*(cv::Mat_<double>(1, 5) << 1, 2, 3, 4, 5);
			extractor->setParam("mat", matExpt);
		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name() << "] = " << e.what() << std::endl;
		}

		try {
			auto param = extractor->getParam("mat");
		
			auto mat = extractor->getParam("mat").extract<cv::Mat>( );
			std::cout << cv::format(mat, "NUMPY") << std::endl;

			//auto vec = extractor->getParam("mat").convert<std::vector<float>>( );
			//auto x = vec;

		} catch ( std::exception& e ) {
			std::cout << "Exception[" << typeid(e).name() << "] = " << e.what() << std::endl;
		}

	return 0;
}

