#include <object-recognition-toolkit\core\core.h>
#include <object-recognition-toolkit\tmp\tmp.h>
#include <Poco\ClassLoader.h>
#include <Poco\Path.h>
#include <Poco\Platform.h>
#include <Poco\Environment.h>
#include <Poco\Logger.h>
#include <Poco\AutoPtr.h>
#include <Poco\ConsoleChannel.h>

using object_recognition_toolkit::tmp::MyClass;
using MyClassLoader = Poco::ClassLoader<MyClass>;

static auto& logger = Poco::Logger::root();

int main(int argc, char* argv[])
{
	Poco::AutoPtr<Poco::Channel> channel(new Poco::ConsoleChannel());
	logger.setChannel(channel);
	logger.setLevel("trace");

	std::string working_directory = Poco::Path::current();
	std::string env_path = Poco::Environment::get("path");
	MyClassLoader loader;
	//std::string library_path = "object-detection-toolkit" + Poco::SharedLibrary::suffix();
	std::string library_path = "object-recognition-toolkit" ".dll";
	loader.loadLibrary(library_path);

	for (auto lib_it = std::begin(loader); lib_it != std::end(loader); lib_it++) 
	{
		poco_trace_f1(logger, "lib: %s", lib_it->first);
		auto& manif = *lib_it->second;
		for (auto manif_it = std::begin(manif); manif_it != std::end(manif); manif_it++) 
		{
			std::string manif_name = manif_it->name();
			poco_trace_f1(logger, "\tmanif: %s", manif_name);
		}
	}


	Poco::AutoPtr<MyClass> myclass = loader.create("object_recognition_toolkit::tmp::MyClass");
	myclass->Name();
	return 0;
}