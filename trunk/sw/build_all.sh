########################################################################
#
# System settings
#
########################################################################
BUILD_SYS=`uname -s`
echo "Building for [${BUILD_SYS}]"

########################################################################
# system compilers
########################################################################
case ${BUILD_SYS} in
 "Darwin") export CC=/usr/bin/gcc CXX=/usr/bin/g++;;
 *) export CC=gcc CXX=g++;;
esac;
echo "Using [${CC}|${CXX}]"

########################################################################
# install dir
########################################################################
INSTALL_DIR=`pwd`
case ${BUILD_SYS} in
 "MINGW*") INSTALL_DIR=`pwd -W`;;
 *) ;;
esac;
echo "Install in [${INSTALL_DIR}]"

function xerror
{
	echo "********************************";
	echo "*** ERROR: $@";
	echo "********************************";
	exit 1;
}

########################################################################
## SDL2
########################################################################
VERSION=2.0.3
ARCHIVE=SDL2-${VERSION}.tar.gz
BUILD_DIR=SDL2-${VERSION}

function configure_SDL2
{
	echo "Configuring SDL2";
	(cd ${BUILD_DIR} && ./configure --prefix=${INSTALL_DIR} --disable-shared) || xerror "SDL2 Configure Failure";
	return 0;
}

function build_SDL2
{
	#Test Archive
	test -e ${ARCHIVE} || wget http://www.libsdl.org/release/${ARCHIVE} || xerror "Couldn't Download ${ARCHIVE}";

	#Test Directory
	test -d ${BUILD_DIR} || tar xfvz ${ARCHIVE} ||xerror "Couldn't Extract ${ARCHIVE}";


	#Test Makefile and configure if needed
	(test -e ${BUILD_DIR}/Makefile || configure_SDL2 )

	#Build...
	make -s -C ${BUILD_DIR} all || xerror "Couldn Build ${BUILD_DIR}";
}

build_SDL2;
