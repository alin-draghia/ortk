#include "object-recognition-toolkit/detection/detector.h"

namespace object_recognition_toolkit
{
	namespace detection
	{

		Detector::~Detector()
		{
		}

		void Detector::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

		void Detector::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
	}
}