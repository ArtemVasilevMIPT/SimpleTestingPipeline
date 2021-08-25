#! /bin/bash

#Managing inputs and preparing project
source_file=$1
test_source=$2


files=$(find ./include -name "*.h" -not -name "TestCase.h" -not -name $(basename $source_file))
for file in $files
do
	rm $file
done

files=$(find ./src -name "*.cpp")
for file in $files
do
	rm $file
done

files=$(find ./tests -name "*.cpp" -not -name $(basename $test_source))
for file in $files
do
	rm $file
done

cp $source_file include/$(basename $source_file)
cp $test_source tests/$(basename $test_source)

touch src/source.cpp
printf "#include \"%s\"\n" $(basename $source_file) >> src/source.cpp
printf "int main(){\n" >> src/source.cpp
printf "return 0;}\n" >> src/source.cpp

#Building and running project
mkdir cmake
cd cmake

cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j7

make coverage_report -j7
cd ../bin
./test-test
