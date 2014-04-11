#include "object-recognition-toolkit\design-pattern\Child2.h"

#include <iostream>

namespace object_recognition_toolkit {
	namespace design_pattern {

		Child2::Child2( ) { }

		Child2::~Child2( ) { }

		void Child2::Print( ) const {
			std::cout << __FUNCTION__ << std::endl;
		}
	}
}