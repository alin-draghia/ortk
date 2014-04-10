#include "object-recognition-toolkit\design-pattern\Child3.h"

#include <iostream>

namespace object_recognition_toolkit
{
	namespace design_pattern
	{
		Child3::Child3()
		{
		}

		Child3::~Child3()
		{
		}

		void Child3::Print() const
		{
			std::cout << __FUNCTION__ << std::endl;
		}
	}
}