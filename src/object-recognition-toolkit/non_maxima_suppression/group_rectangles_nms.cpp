#include "precomp.h"
#include "object-recognition-toolkit/non_maxima_suppression/group_rectangles_nms.h"

#include <boost/make_shared.hpp>

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{

		GroupRectanglesNms::GroupRectanglesNms()
			: group_threshold(3)
			, eps(0.2)
		{
		}

		GroupRectanglesNms::~GroupRectanglesNms()
		{
		}


		boost::shared_ptr<NonMaximaSuppressor> GroupRectanglesNms::Clone() const
		{
			return boost::make_shared<GroupRectanglesNms>(*this);
		}

		void GroupRectanglesNms::suppress(std::vector<cv::Rect>& detections, std::vector<double>& confidences) const
		{
			(void)detections;
			(void)confidences;

			cv::groupRectangles(detections, group_threshold, eps);
			confidences.clear();
			confidences.resize(detections.size(), 0.0);
		}

		void GroupRectanglesNms::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<NonMaximaSuppressor>(*this);
			ar >> group_threshold;
			ar >> eps;
		}

		void GroupRectanglesNms::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<NonMaximaSuppressor>(*this);
			ar << group_threshold;
			ar << eps;
		}

	}
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::non_maxima_suppression::GroupRectanglesNms);
