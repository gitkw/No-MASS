all:
	cd Boost-Source/; ./bootstrap.sh --with-toolset=gcc --with-libraries=system
	cd Boost-Source/; ./b2 headers
	#cd Boost-Source/; ./b2 -a -j8 link=static address-model=64 toolset=gcc -std=c++11 -d2 debug-symbols=on cxxflags=-fPIC 

#bootstrap.bat gcc
#b2 -a -j8 link=static address-model=64 toolset=gcc -std=c++11 -d2 debug-symbols=on cxxflags=-fPIC --with-system --with-thread --with-chrono --with-date_time --with-filesystem --with-regex --with-serialization 

clean:
	rm -f bootstrap.log
	rm -f Boost-Source/project-config.jam*
	rm -fR Boost-Source/stage/lib/
	cd Boost-Source/; ./b2 --clean

checkout:
	# use the one belows
	git submodule update --init --recursive
	#git clone --recursive https://github.com/boostorg/boost.git Boost-Source
	#cd Boost-Source/;git checkout master
	#cd Boost-Source/;git pull
	#cd Boost-Source/;git submodule update