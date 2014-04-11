// test-algorithm-loader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <object-recognition-toolkit\core\algorithm.h>
//#include <object-recognition-toolkit\feature-extraction\feature-extractor.h>
#include <Poco\AutoPtr.h>
#include <Poco\Path.h>
#include <Poco\ClassLoader.h>

using Algorithm = object_recognition_toolkit::core::Algorithm;
using AlgorithmFactory = object_recognition_toolkit::core::AlgorithmFactory;
using AlgorithmLibrary = Poco::ClassLoader<AlgorithmFactory>;

int _tmain(int argc, _TCHAR* argv[])
{
	auto parent_dir = Poco::Path::current();

	AlgorithmLibrary algorithmLibrary;

	algorithmLibrary.loadLibrary("object-recognition-toolkit.dll", "AlgorithmFactoryManifest");

	AlgorithmFactory& algorithmFactory = algorithmLibrary.instance("object_recognition_toolkit::core::AlgorithmFactory");

	Poco::AutoPtr<Algorithm> dalalTrigs = algorithmFactory.createInstance("DallalTriggsHog");

	std::string name = dalalTrigs->name();

	
	return 0;
}

