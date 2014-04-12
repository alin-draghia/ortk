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

	return 0;
}

