if [ -z $1 ]; then
  cmake --list-presets
  exit 1
fi

cmake --preset $1 && cmake --build build/$1 --parallel
