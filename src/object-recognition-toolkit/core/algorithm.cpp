#include "object-recognition-toolkit\core\algorithm.h"
#include <Poco\ClassLibrary.h>

namespace object_recognition_toolkit
{
	namespace core
	{
		Algorithm::Algorithm()
		{
		}

		Algorithm::~Algorithm()
		{
		}
	}
}


#if(0)
POCO_BEGIN_MANIFEST(object_recognition_toolkit::core::AlgorithmFactory)
	POCO_EXPORT_SINGLETON(object_recognition_toolkit::core::AlgorithmFactory)
POCO_END_MANIFEST
#endif

POCO_BEGIN_NAMED_MANIFEST(AlgorithmFactoryManifest, object_recognition_toolkit::core::AlgorithmFactory)
	POCO_EXPORT_SINGLETON(object_recognition_toolkit::core::AlgorithmFactory)
POCO_END_MANIFEST