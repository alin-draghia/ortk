#include "object-recognition-toolkit/core/algorithm.h"

namespace object_recognition_toolkit {
	namespace core {

		
		Algorithm::Algorithm(const std::string& name) : 
			name_(name)
		{
		}


		Algorithm::~Algorithm( )
		{
		}

		const std::string& Algorithm::name() const
		{
			return name_;
		}

	}
}