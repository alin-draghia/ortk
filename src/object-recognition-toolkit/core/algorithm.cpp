#include <boost/serialization/string.hpp>
#include "object-recognition-toolkit/core/algorithm.h"

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

	
		template<class Archive>
		void Algorithm::save(Archive & ar, const unsigned int version) const
		{
			ar << name_;
		}


		template<class Archive>
		void Algorithm::load(Archive & ar, const unsigned int version)
		{
			ar >> name_;
		}
	}
}

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/export.hpp>
//BOOST_CLASS_EXPORT_IMPLEMENT(object_recognition_toolkit::core::Algorithm);



namespace object_recognition_toolkit {
	namespace core {
		//template<> ORT_API void Algorithm::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
		//template<> ORT_API void Algorithm::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		template ORT_API void Algorithm::load(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
		template ORT_API void Algorithm::save(boost::archive::polymorphic_oarchive& ar, const unsigned int version) const;
	}
}

