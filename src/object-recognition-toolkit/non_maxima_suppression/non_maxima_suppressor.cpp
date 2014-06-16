#include "precomp.h"
#include "object-recognition-toolkit/non_maxima_suppression/non_maxima_suppressor.h"

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{
		
		NonMaximaSuppressor::~NonMaximaSuppressor() 
		{
		}

		boost::shared_ptr<NonMaximaSuppressor> NonMaximaSuppressor::Clone() const
		{
			throw core::NotImplementedError();
		}

		void NonMaximaSuppressor::suppress(std::vector<core::Box>& detections, std::vector<double>& confidences) const
		{
			throw core::NotImplementedError();
		}

		void NonMaximaSuppressor::serialize(core::iarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}

		void NonMaximaSuppressor::serialize(core::oarchive& ar, const unsigned int version)
		{
			(void)ar;
			(void)version;
		}
	}
}