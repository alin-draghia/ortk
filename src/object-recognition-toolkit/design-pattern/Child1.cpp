#include "object-recognition-toolkit\design-pattern\Child1.h"

#include <iostream>

namespace object_recognition_toolkit {
	namespace design_pattern {

		Child1::Child1( ) { }

		Child1::~Child1( ) { }

		void Child1::Print( ) const {
			std::cout << __FUNCTION__ << std::endl;
		}

	}
}