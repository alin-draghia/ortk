#pragma once

namespace object_recognition_toolkit
{
	namespace design_pattern
	{
		class Base
		{
		public:
			Base();
			virtual ~Base();

			virtual void Print() const = 0;
		};
	}
}