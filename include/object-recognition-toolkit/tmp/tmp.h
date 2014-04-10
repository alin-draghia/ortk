#pragma once

#include <string>
#include <Poco\RefCountedObject.h>


namespace object_recognition_toolkit
{
	namespace tmp
	{

		class MyClass
			: public Poco::RefCountedObject
		{
			std::string name;
		public:
			MyClass();
			~MyClass();

			virtual const std::string& Name() const;
		};

	}
}
