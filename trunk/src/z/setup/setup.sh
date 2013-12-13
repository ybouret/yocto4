# !bash
function xerror
{
    cmake -E cmake_echo_color --red "-- $1";
    exit 1;
}

Z_DIR="zlib-1.2.8";

################################################################################
## Extract
################################################################################
function fetch
{
    Z_TGZ="$Z_DIR.tar.gz"
    Z_URL="http://zlib.net/$Z_TGZ"
    cmake -E cmake_echo_color --blue "-- Downloading $Z_URL"
    curl $Z_URL -o $Z_TGZ || xerror "Could not Download !";
    cmake -E cmake_echo_color --blue "-- Extracting Archive";
    tar xfz $Z_TGZ || xerror "Could not Extract !";
}

################################################################################
## Extract
################################################################################
[ -d "$Z_DIR" ] || fetch;

cmake -E cmake_echo_color --blue "-- Copy Public Headers";
Z_PUBLIC="zconf.h zlib.h";
for H in $Z_PUBLIC;
do
    echo "$Z_DIR/$H";
    cp "$Z_DIR/$H" ../yocto/zlib
done;

cmake -E cmake_echo_color --blue "-- Copy Private  Headers";
Z_PRIVATE="crc32.h\
    deflate.h\
    gzguts.h\
    inffast.h\
    inffixed.h\
    inflate.h\
    inftrees.h\
    trees.h\
    zutil.h";
for H in $Z_PRIVATE;
do
    echo "$Z_DIR/$H";
    cp "$Z_DIR/$H" ../yocto/zlib
done;

Z_SOURCE="adler32.c\
    compress.c\
    crc32.c\
    deflate.c\
    gzclose.c\
    gzlib.c\
    gzread.c\
    gzwrite.c\
    inflate.c\
    infback.c\
    inftrees.c\
    inffast.c\
    trees.c\
    uncompr.c\
    zutil.c";
cmake -E cmake_echo_color --blue "-- Copy Sources";
for S in $Z_SOURCE;
do
    echo "$Z_DIR/$S";
    cp "$Z_DIR/$S" ../yocto/zlib
done;

cp "$Z_DIR/test/example.c" ../yocto/zlib

rm -Rf $Z_DIR $Z_TGZ
