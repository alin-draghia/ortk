#include "object-recognition-toolkit/non_maxima_suppression/non_maxima_suppressor.h"

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{
		
		NonMaximaSuppressor::~NonMaximaSuppressor() 
		{
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