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
		void Algorithm::save(Archive& ar, const unsigned int version) const
		{
			ar << name_;
		}

		template<class Archive>
		void Algorithm::load(Archive& ar, const unsigned int version)
		{
			ar >> name_;
		}

	}
}



#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
BOOST_CLASS_EXPORT_IMPLEMENT(object_recognition_toolkit::core::Algorithm);

