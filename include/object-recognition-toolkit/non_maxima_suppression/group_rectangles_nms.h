#pragma once
#ifndef GROUP_RECTANGLES_NMS_HEADER_INCLUDED_
#define GROUP_RECTANGLES_NMS_HEADER_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "./non_maxima_suppressor.h"


namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{
		class PUBLIC_API GroupRectanglesNms
			: public NonMaximaSuppressor
		{
		public:
			GroupRectanglesNms();
			virtual ~GroupRectanglesNms();

			virtual boost::shared_ptr<NonMaximaSuppressor> Clone() const;

		public:
			virtual void suppress(std::vector<core::Box>& detections, std::vector<double>& confidences) const;

			int group_threshold;
			double eps;

		private:
			DECLARE_SERIALIZABLE();
		};


	}
}

#pragma warning(pop)

#endif // GROUP_RECTANGLES_NMS_HEADER_INCLUDED_