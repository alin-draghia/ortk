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
			virtual ~Clonable() = 0;

		public:
			virtual Clonable* Clone() = 0;
		};
	}
}


#endif // CLONABLE_H_INCLUDED_