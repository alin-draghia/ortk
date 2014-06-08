#pragma once
#ifndef CLONABLE_H_INCLUDED_
#define CLONABLE_H_INCLUDED_

#include "./public-api.h"

namespace object_recognition_toolkit
{
	namespace core
	{
		class PUBLIC_API Clonable
		{
		public:
			virtual ~Clonable();

		public:
			virtual Clonable* Clone();
		};
	}
}


#endif // CLONABLE_H_INCLUDED_