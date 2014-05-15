#include "object-recognition-toolkit/detection/detector_base_mt.h"

namespace object_recognition_toolkit
{
	namespace detection
	{

		DetectorBaseMt::DetectorBaseMt()
			: DetectorBase()
		{
		}


		DetectorBaseMt::DetectorBaseMt(ImagePyramid* pyramidBuilder, ImageScanner* imageScanner, FeatureExtractor* featureExtractor, Classifier* classifier, NonMaximaSuppressor* nonMaximaSuppressor)
			: DetectorBase(pyramidBuilder, imageScanner, featureExtractor, classifier, nonMaximaSuppressor)
		{
		}

		DetectorBaseMt::~DetectorBaseMt()
		{
		}

		const std::string& DetectorBaseMt::name() const
		{
			static const std::string name = "DetectorBaseMt";
			return name;
		}

		void DetectorBaseMt::detect(const cv::Mat& image, std::vector<cv::Rect>& detections, std::vector<double>& confidences, double treshold) const
		{
			DetectorBase::detect(image, detections, confidences, treshold);
		}

		void DetectorBaseMt::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<DetectorBase>(*this);
			(void)version;
		}

		void DetectorBaseMt::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<DetectorBase>(*this);
			(void)version;
		}
	}
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::detection::DetectorBaseMt);