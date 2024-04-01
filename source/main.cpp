/*
	Access violation when running in debugger:

	JVM intentionally raises (and catches) access violation exceptions.

 	Configure debugger to ignore such exceptions or continue using these steps:

	Visual Studio: press "Continue".
	XCode: enter into debugger window: "pr h -s false SIGSEGV", then press "Continue".
*/

#include <iostream>

#include <scapix/jni/vm.h>
#include <scapix/jni/throwable.h>

#include <scapix/java_api/java/lang/String.h>
#include <scapix/java_api/java/lang/System.h>
#include <scapix/java_api/java/util/Locale.h>
#include <scapix/java_api/java/text/DateFormatSymbols.h>

#include <scapix/java_api/java/io/File.h>
#include <scapix/java_api/java/io/FileReader.h>
#include <scapix/java_api/java/io/FileNotFoundException.h>

namespace jni = scapix::jni;
using namespace scapix::java_api;

void string_example()
{
	// Prototype: ref<java::lang::String> getProperty(ref<java::lang::String>).
	// Notice how C++ objects (strings in this case) are automatically converted to and from corresponding Java types.
	// This works for any type supported by scapix::jni::convert() interface, which supports many STL types and can be extended for your own types.

	std::string version = java::lang::System::getProperty("java.version");

	std::cout << "java.version = " << version << "\n";
}

void array_example()
{
	// Prototype: ref<array<java::lang::String>> getISOLanguages()
	// Here, there is no conversion, you get JNI references:

	auto a1 = java::util::Locale::getISOLanguages();
	jni::ref<jni::array<java::lang::String>> a2 = java::util::Locale::getISOLanguages();

	// Additional supported array type syntax (not for nested arrays, C++ doesn't allow T[][] type):

	jni::ref<java::lang::String[]> a3 = java::util::Locale::getISOLanguages();

	// It is converted only if you assign such a reference to a corresponding C++ type:

	std::vector<std::string> languages = java::util::Locale::getISOLanguages();

	for (auto lang : languages)
		std::cout << lang << "\n";

	// This works for any convertible types and any depth:
	// Prototype: ref<array<array<java::lang::String>>> getZoneStrings();

	std::vector<std::vector<std::string>> zone_strings = java::text::DateFormatSymbols::getInstance()->getZoneStrings();

	for (auto zone : zone_strings)
		for (auto string : zone)
			std::cout << string << "\n";
}

void array_enumeration_example()
{
	// object array: ref<java::lang::String[]>

	for (auto&& i : java::util::Locale::getISOLanguages()->elements())
	{
		[[maybe_unused]] auto length = i->length();
		i = jni::new_object<java::lang::String>();
	}

	// primitive array: ref<jbyte[]>

	for (auto&& i : java::lang::System::getProperty("java.version")->getBytes()->elements())
	{
		i = 5;
	}
}

// This convert function is needed because java:util::Map uses generics and generated headers currently lack generics info.
// In a future version, generated headers will include generics info, so conversion for any generic type will also be automatic (like in string/array examples above).

std::map<std::string, std::string> cvt(jni::ref<java::util::Map> map)
{
	return jni::convert_cpp<std::map<std::string, std::string>>(jni::ref<jni::generic_type<jni::java_util_map, java::lang::String, java::lang::String>>(map));
}

void map_example()
{
	// Prototype: ref<java::util::Properties> getProperties()

	std::map<std::string, std::string> properties = cvt(java::lang::System::getProperties());

	for (auto p : properties)
		std::cout << p.first << " = " << p.second << "\n";
}

void exception_example()
{
	try
	{
		auto file = jni::new_object<java::io::File>("/file.txt");
		auto reader = jni::new_object<java::io::FileReader>(file);
	}
	catch (const jni::vm_exception& e)
	{
		if (auto file_not_found = e.instance_of<java::io::FileNotFoundException>())
		{
			// handle specifically java::io::FileNotFoundException
		}
		else if (auto io = e.instance_of<java::io::IOException>())
		{
			// handle any java::io::IOException
		}

		std::cout << e.what() << "\n";
	}
}

int main()
{
	JavaVMOption options[]
	{
		{ const_cast<char*>("-Djava.class.path=" SCAPIX_JNI_JAR_FILE) },
	};

	JavaVMInitArgs vm_args
	{
		.version = JNI_VERSION_1_6,
		.nOptions = std::size(options),
		.options = options,
		.ignoreUnrecognized = false,
	};

	try
	{
		jni::create_vm(&vm_args);

		string_example();
		array_example();
		map_example();
		exception_example();

		jni::destroy_vm();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
		return 1;
	}

	return 0;
}
