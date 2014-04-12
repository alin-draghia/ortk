#include "pass-through.h"

namespace object_recognition_toolkit {
	namespace feature_extraction {

		PassThrough::PassThrough() { }

		PassThrough::~PassThrough() { }

		const std::string& PassThrough::name( ) const
		{
			return name_;
		}

	}
}