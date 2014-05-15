#pragma once
#ifndef NAMED_H_INCLUDED_
#define NAMED_H_INCLUDED_

#include <string>
#include "./public-api.h"

namespace object_recognition_toolkit
{
	namespace core
	{
		class PUBLIC_API Named
		{
		public:
			virtual ~Named() = 0;
			virtual const std::string& name() const = 0;
		};
	}
}

#endif // NAMED_H_INCLUDED_