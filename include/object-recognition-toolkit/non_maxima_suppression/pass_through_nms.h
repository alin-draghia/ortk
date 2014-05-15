#pragma once
#ifndef PASS_THROUGH_NMS_H_INCLUDED_
#define PASS_THROUGH_NMS_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "./non_maxima_suppressor.h"

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{
		class PUBLIC_API PassThroughNms
			: public NonMaximaSuppressor
		{
		public:
			PassThroughNms();
			virtual ~PassThroughNms();

		public:
			virtual const std::string& name() const;

		public:
			virtual void suppress(std::vector<cv::Rect>& detections, std::vector<double>& confidences) const;

		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};


	}
}

#pragma warning(pop)

#endif // PASS_THROUGH_NMS_H_INCLUDED_