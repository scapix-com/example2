export JAVA_HOME=`/usr/libexec/java_home -v 11`
cmake -B build/xcode  -G "Xcode"
cmake --build build/xcode
