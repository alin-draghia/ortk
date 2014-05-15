#include <boost/serialization/string.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/export.hpp>

#include "object-recognition-toolkit/core/algorithm.h"

#if (0)

namespace object_recognition_toolkit {
	namespace core {

		Algorithm::Algorithm()
			: name_("no-name")
		{
		}


		Algorithm::Algorithm(const std::string& name) 
			: name_(name)
		{
		}


		Algorithm::~Algorithm( )
		{
		}

		const std::string& Algorithm::name() const
		{
			return name_;
		}


		void Algorithm::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> name_;
		}


		void Algorithm::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << name_;
		}
	

	}
}

#endif

