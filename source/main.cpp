/*
	This example builds native C++ executable which creates JVM using JNI_CreateJavaVM() function.
	The same functionality works in native C++ library loaded into JVM with System.loadLibrary().

	JNI_CreateJavaVM() issues:

	macOS:
	- Oracle's JDK doesn't advertise itself as supporting JNI. Edit /Library/Java/JavaVirtualMachines/<version>.jdk/Contents/Info.plist and add JNI as an option in JVMCapabilities.
	- When debugging in XCode, if you get SIGSEGV exception inside JNI_CreateJavaVM(), enter into debugger window: "pr h -s false SIGSEGV".

	Windows:
	- jvm.dll needs to be in PATH (copying jvm.dll doesn't work). You may need to adjust VS_DEBUGGER_ENVIRONMENT in CMakeLists.txt.
	- When debugging in VS, if you get "0xC0000005: Access violation" exception inside JNI_CreateJavaVM(), ignore it and press "Continue".
*/

#include <iostream>
#include <scapix/link/java/vm.h>
#include <scapix/java_api/java/lang/System.h>
#include <scapix/java_api/java/util/Locale.h>
#include <scapix/java_api/java/text/DateFormatSymbols.h>

using namespace scapix::link::java;
using namespace scapix::java_api;

void string_example()
{
    // Prototype: ref<java::lang::String> getProperty(ref<java::lang::String>).
    // Notice how C++ objects (strings in this case) are automatically converted to and from corresponding Java types.
    // This works for any type supported by scapix::link::java::convert() interface, which supports many STL types and can be extended for your own types.

    std::string version = java::lang::System::getProperty("java.version");

    std::cout << "java.version = " << version << "\n";
}

void array_example()
{
    // Prototype: ref<array<java::lang::String>> getISOLanguages()
    // Here, there is no conversion, you get JNI references:

    auto a1 = java::util::Locale::getISOLanguages();
    ref<array<java::lang::String>> a2 = java::util::Locale::getISOLanguages();

    // Additional supported array type syntax (not for nested arrays, C++ doesn't allow T[][] type):

    ref<java::lang::String[]> a3 = java::util::Locale::getISOLanguages();

    // It is converted only if you assign such a reference to a corresponding C++ type:

    std::vector<std::string> languages = java::util::Locale::getISOLanguages();

    for (auto lang : languages)
        std::cout << lang << "\n";

    // This works for any convertable types and any depth:
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
		auto length = i->length();
		i = java::lang::String::new_object();
	}

	// primitive array: ref<jbyte[]>

	for (auto&& i : java::lang::System::getProperty("java.version")->getBytes()->elements())
	{
		i = 5;
	}
}

// This convert function is needed because java:util::Map uses generics and generated headers currently lack generics info.
// I plan to improve this in the future, so conversion for any generics type will also be automatic (like in string/array examples above).

std::map<std::string, std::string> cvt(ref<java::util::Map> map)
{
    return convert_cpp<std::map<std::string, std::string>>(ref<generic_type<map_class_name, string::class_name, string::class_name>>(map));
}

void map_example()
{
    // Prototype: ref<java::util::Properties> getProperties()

    std::map<std::string, std::string> properties = cvt(java::lang::System::getProperties());

    for (auto p : properties)
        std::cout << p.first << " = " << p.second << "\n";
}

int main()
{
    JavaVMInitArgs vm_args;

    vm_args.version = JNI_VERSION_1_6;
	vm_args.nOptions = 0;
	vm_args.options = nullptr;
	vm_args.ignoreUnrecognized = false;

	auto res = scapix::link::java::create_vm(&vm_args);

	if (res != JNI_OK)
	{
		std::cout << "create vm failed: " << res << "\n";
		return 1;
	}

    string_example();
    array_example();
    map_example();

    scapix::link::java::destroy_vm();

    return 0;
}
