#pragma once

#include "object-recognition-toolkit\design-pattern\Base.h"
namespace object_recognition_toolkit {
	namespace design_pattern {

		class Child1
			: public Base {
		public:
			Child1( );
			virtual ~Child1( );

			virtual void Print( ) const;
		};

	}
}