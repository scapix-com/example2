export JAVA_HOME=`/usr/libexec/java_home`
cmake -B build/xcode -G "Xcode"
cmake --build build/xcode
