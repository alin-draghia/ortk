#include "object-recognition-toolkit/core/named.h"

namespace object_recognition_toolkit
{
	namespace core
	{
		Named::~Named() {}

		const std::string& Named::name() const
		{
			throw std::runtime_error("not implemented, [~pure_virtual]");
		}
	}
}