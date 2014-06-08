#include "precomp.h"
#include "object-recognition-toolkit/non_maxima_suppression/group_rectangles_nms.h"

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{

		GroupRectanglesNms::GroupRectanglesNms()
		{
		}

		GroupRectanglesNms::~GroupRectanglesNms()
		{
		}

		const std::string& GroupRectanglesNms::name() const
		{
			static const std::string name = "GroupRectanglesNms";
			return name;
		}

		core::Clonable* GroupRectanglesNms::Clone()
		{
			return new GroupRectanglesNms(*this);
		}

		void GroupRectanglesNms::suppress(std::vector<cv::Rect>& detections, std::vector<double>& confidences) const
		{
			(void)detections;
			(void)confidences;

			cv::groupRectangles(detections, 2);
			confidences.clear();
			confidences.resize(detections.size(), 0.0);
		}

		void GroupRectanglesNms::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<NonMaximaSuppressor>(*this);
		}

		void GroupRectanglesNms::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<NonMaximaSuppressor>(*this);
		}

	}
}
