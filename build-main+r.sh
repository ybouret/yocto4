if [ $# != 2 ]; then
	cmake -E cmake_echo_color --red "build.sh Tools BUILD_TYPE=(Debug|Release)";
	exit 1;	
fi
cmake -E cmake_echo_color --blue "Building SDK for $1, BUILD_TYPE=$2..."
YOCTO_BUILD_TESTS=no YOCTO_EXCLUDE="gfx seem spade chemical lingua kr net gems lua++ z++ bz2++ ocl json chem visit fltk fame python tiff++ lua lzo z bz2 png jpeg sqlite tiff xz jbig" TARGETS=install make $1 BUILD_TYPE=$2
