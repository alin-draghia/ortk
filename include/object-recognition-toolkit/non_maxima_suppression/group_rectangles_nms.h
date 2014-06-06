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

		public:
			virtual const std::string& name() const;

		public:
			virtual core::Clonable* Clone();

		public:
			virtual void suppress(std::vector<core::Box>& detections, std::vector<double>& confidences) const;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};


	}
}

#pragma warning(pop)

#endif // GROUP_RECTANGLES_NMS_HEADER_INCLUDED_