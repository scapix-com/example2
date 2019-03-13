# Example for `scapix::link::java` (C++ JNI) library

```cpp
#include <scapix/java_api/java/lang/System.h>
#include <scapix/java_api/java/util/Locale.h>
#include <scapix/java_api/java/text/DateFormatSymbols.h>

using namespace scapix::link::java;
using namespace scapix::java_api;

void test1()
{
    // C++ objects are automatically converted to and from corresponding Java types.
    // This works for any type supported by scapix::link::java::convert() interface,
    // which supports many STL types and can be extended for your own types.

    std::string version = java::lang::System::getProperty("java.version");
    std::vector<std::string> languages = java::util::Locale::getISOLanguages();
    std::vector<std::vector<std::string>> zone_strings = java::text::DateFormatSymbols::getInstance()->getZoneStrings();
    std::map<std::string, std::string> properties = java::lang::System::getProperties();
}

void test2()
{
    // Conversion happens only when you request it, otherwise API works with JNI references:

    ref<java::lang::String> version = java::lang::System::getProperty("java.version");
    ref<array<java::lang::String>> languages = java::util::Locale::getISOLanguages();
    ref<array<array<java::lang::String>>> zone_strings = java::text::DateFormatSymbols::getInstance()->getZoneStrings();
    ref<java::util::Map> properties = java::lang::System::getProperties();
}

void test3()
{
    // Same as above, no conversion to C++ types:

    auto version = java::lang::System::getProperty("java.version");
    auto languages = java::util::Locale::getISOLanguages();
    auto zone_strings = java::text::DateFormatSymbols::getInstance()->getZoneStrings();
    auto properties = java::lang::System::getProperties();
}
```
