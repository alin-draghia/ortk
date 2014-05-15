#include "object-recognition-toolkit/non_maxima_suppression/pass_through_nms.h"

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{

		PassThroughNms::PassThroughNms()
		{
		}

		PassThroughNms::~PassThroughNms()
		{
		}

		const std::string& PassThroughNms::name() const
		{
			static const std::string name = "PassThroughNms";
			return name;
		}


		void PassThroughNms::suppress(std::vector<cv::Rect>& detections, std::vector<double>& confidences) const
		{
			(void)detections;
			(void)confidences;
		}

		void PassThroughNms::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<NonMaximaSuppressor>(*this);
		}

		void PassThroughNms::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<NonMaximaSuppressor>(*this);
		}
	}
}


#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::non_maxima_suppression::PassThroughNms);