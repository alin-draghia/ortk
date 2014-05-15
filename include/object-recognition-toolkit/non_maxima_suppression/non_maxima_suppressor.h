#pragma once
#ifndef NON_MAXIMA_SUPPRESSOR_H_INCLUDED_
#define NON_MAXIMA_SUPPRESSOR_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/algorithm.h"


namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{

		class PUBLIC_API NonMaximaSuppressor
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			NonMaximaSuppressor(const std::string& name);
			virtual ~NonMaximaSuppressor() = 0;

		public:
			virtual void suppress(std::vector<cv::Rect>& detections, std::vector<double>& confidences) const = 0;

		private:
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};

	}
}

#pragma warning(pop)

#endif // NON_MAXIMA_SUPPRESSOR_H_INCLUDED_