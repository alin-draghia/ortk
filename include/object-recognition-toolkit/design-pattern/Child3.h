#pragma once

#include "object-recognition-toolkit\design-pattern\Child1.h"

namespace object_recognition_toolkit
{
	namespace design_pattern
	{
		class Child3
			: public Child1
		{
		public:
			Child3();
			virtual ~Child3();

			virtual void Print() const;
		};
	}
}