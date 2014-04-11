#pragma once

#include "object-recognition-toolkit\design-pattern\Base.h"
namespace object_recognition_toolkit {
	namespace design_pattern {

		class Child2
			: public Base {
		public:
			Child2( );
			virtual ~Child2( );

			virtual void Print( ) const;
		};

	}
}