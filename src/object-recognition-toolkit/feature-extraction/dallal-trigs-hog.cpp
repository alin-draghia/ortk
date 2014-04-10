#include "object-recognition-toolkit\feature-extraction\dallal-trigs-hog.h"

#include <Poco\ClassLoader.h>

namespace object_recognition_toolkit
{
	namespace feature_extraction
	{
		DallalTriggsHog::DallalTriggsHog()
		{
		}

		DallalTriggsHog::~DallalTriggsHog()
		{
		}
		
		const std::string& DallalTriggsHog::name() const
		{
			static std::string str = typeid(*this).name();
			return str;
		}
	}
}

namespace
{

	class DallalTriggsHogRegistrar
	{
	public:
		DallalTriggsHogRegistrar()
		{
			using object_recognition_toolkit::feature_extraction::DallalTriggsHog;

			Poco::ClassLoader<object_recognition_toolkit::core::AlgorithmFactory> loader;
			loader.loadLibrary("object-recognition-toolkit" ".dll");
			auto& factory = loader.instance("object_recognition_toolkit::core::AlgorithmFactory");
			auto class_name = typeid(DallalTriggsHog).name();
			factory.registerClass<DallalTriggsHog>(class_name);
		}

		~DallalTriggsHogRegistrar()
		{
		}
	};

	static DallalTriggsHogRegistrar registrar;

}