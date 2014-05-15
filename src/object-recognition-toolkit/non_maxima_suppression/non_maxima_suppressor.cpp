#include "object-recognition-toolkit/non_maxima_suppression/non_maxima_suppressor.h"

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{
		
		NonMaximaSuppressor::NonMaximaSuppressor(const std::string& name)
			: Algorithm(name)
		{
		}

		NonMaximaSuppressor::~NonMaximaSuppressor() 
		{
		}

		void NonMaximaSuppressor::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<Algorithm>(*this);
		}

		void NonMaximaSuppressor::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<Algorithm>(*this);
		}
	}
}