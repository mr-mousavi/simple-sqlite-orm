#Compile
Before compile make a copy of requirements_x86 or 64 as requirements

##Default Parameter
Following parameter set directly in CMakeLists.txt. Therefore, if you want to change them then you should modify CMakeLists.txt

- -std=c++98 or c++11

##Debug Mode
In order to compile project in *debug* mode (enable debug flag) you should perform following step:

1. Make sure you are in root directory of project
2. mkdir _debug
3. cd _debug
4. cmake -DCMAKE_BUILD_TYPE=Debug ..
5. make

##Release Mode
It is mostly like debug mode, except that in step 4 you should pass another parameter :

1. Make sure you are in root directory of project
2. mkdir _release
3. cd _release
4. cmake -DCMAKE_BUILD_TYPE=Debug ..
5. make


##Cross Compile For ARM
In order to cross compile the project for your arm processor you should add following parameters while configuring you cmake :

- CMAKE_FIND_ROOT_PATH : to a path where you have an exact copy of the root filesystem you have on your target device (with libraries and binaries pre-compiled for the target processor)
- CMAKE_C_COMPILER : to your ARM gcc compiler
- CMAKE_CXX_COMPILER : to your ARM g++ compiler

##Make Install
To specify the the directory to install package you should set the following paremeter:
CMAKE_INSTALL_PREFIX:PATH=path_to_install_dir

##Example
You can use the following configuration to compile project in *debug mode* for an *ARM platform* :

	
	#> cmake -DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_C_COMPILER=path_to_your_toolchain_bin/arm-linux-gcc \
	-DCMAKE_CXX_COMPILER=path_to_your_toolchain_bin/arm-linux-g++ \
	-DCMAKE_FIND_ROOT_PATH=path_to_your_toolchain_bin/path_to_sysroot \
        -DCMAKE_INSTALL_PREFIX=../_install ..



#build and run
1: create _build folder (or any folder you like to put build stuff inside)
2: inside _build folder run below command step by step
2-1 cmake ..
2-2 make
3: executable file inside _build folder is ready to use... (make sure you have no error and )
4:./simple-sqlite-orm




have fun... )

