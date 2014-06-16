#include "precomp.h"
#include "object-recognition-toolkit/detection/detector.h"

namespace object_recognition_toolkit
{
	namespace detection
	{

		Detector::~Detector()
		{
		}

		boost::shared_ptr<Detector> Detector::Clone() const
		{
			throw core::NotImplementedError();
		}

		void Detector::Detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold) const
		{ 
			throw core::NotImplementedError();
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