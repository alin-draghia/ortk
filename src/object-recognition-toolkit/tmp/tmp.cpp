#include <object-recognition-toolkit\tmp\tmp.h>
#include <Poco\ClassLibrary.h>
#include <Poco\Logger.h>


namespace object_recognition_toolkit
{
	namespace tmp
	{

		static auto& logger = Poco::Logger::get(typeid(MyClass).name());

		MyClass::MyClass()
			: name("Alin")
		{
			poco_trace(logger, __FUNCTION__);
		}


		MyClass::~MyClass()
		{
			poco_trace(logger, __FUNCTION__);
		}

		const std::string& MyClass::Name() const
		{
			poco_trace(logger, __FUNCTION__);
			return name;
		}

	}
}

POCO_BEGIN_MANIFEST(object_recognition_toolkit::tmp::MyClass)
	POCO_EXPORT_CLASS(object_recognition_toolkit::tmp::MyClass)
POCO_END_MANIFEST