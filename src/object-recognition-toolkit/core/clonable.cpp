#include "precomp.h"
#include "object-recognition-toolkit/core/clonable.h"

#include <exception>
#include <stdexcept>

namespace object_recognition_toolkit
{
	namespace core
	{
		Clonable::~Clonable() { }

		Clonable* Clonable::Clone()
		{
			throw std::runtime_error("not implemented, [~pure_virtual]");
		}
	}
}
