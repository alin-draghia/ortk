// tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "tests.h"

#include <object-recognition-toolkit/object_recognition_toolkit.h>

int _tmain(int argc, _TCHAR* argv[])
{
	object_recognition_toolkit::non_maxima_suppression::PassThroughNms z;
	(void)z;
	test_detector_deserialization();
	return 0;
}

