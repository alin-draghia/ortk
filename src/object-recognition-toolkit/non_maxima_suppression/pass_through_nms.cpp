#include "precomp.h"
#include "object-recognition-toolkit/non_maxima_suppression/pass_through_nms.h"

#include <boost/make_shared.hpp>

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

		boost::shared_ptr<NonMaximaSuppressor> PassThroughNms::Clone() const
		{
			return boost::make_shared<PassThroughNms>(*this);
		}

		void PassThroughNms::suppress(std::vector<cv::Rect>& detections, std::vector<double>& confidences) const
		{
			(void)detections;
			(void)confidences;
		}

		void PassThroughNms::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<NonMaximaSuppressor>(*this);
		}

		void PassThroughNms::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<NonMaximaSuppressor>(*this);
		}
	}
}


#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::non_maxima_suppression::PassThroughNms);