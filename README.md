# Example for [Scapix JNI](https://www.scapix.com/jni/)

[Scapix JNI](https://www.scapix.com/jni/) is a modern C++20 wrapper for Java Native Interface ([JNI](https://docs.oracle.com/en/java/javase/21/docs/specs/jni/)).
It provides type-safe APIs and automatic resource management, with ZERO runtime overhead compared to manually written JNI code.

```cpp
// Generated C++ headers for all JDK/Android classes:

#include <scapix/java_api/java/lang/System.h>
#include <scapix/java_api/java/util/Locale.h>
#include <scapix/java_api/java/text/DateFormatSymbols.h>

namespace jni = scapix::jni;
using namespace scapix::java_api;

void example()
{
    // call any Java functions from any Java classes

    auto version = java::lang::System::getProperty("java.version");
    auto languages = java::util::Locale::getISOLanguages();
    auto zone_strings = java::text::DateFormatSymbols::getInstance()->getZoneStrings();
    auto properties = java::lang::System::getProperties();
}
```

### Install and build this example (Windows, macOS, Linux)

```bash
$ git clone https://github.com/scapix-com/example2
$ cd example2
$ ./default.sh
```
