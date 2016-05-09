VERSION=-5.2.2
ARCHIVE=xz$VERSION.tar.gz
WORKDIR=xz$VERSION
OUTDIR=sandbox
OUTSRC=$OUTDIR/src
LIBXZA=$OUTDIR/lib/liblzma.a

echo "-- Fetching $ARCHIVE"
test -f $ARCHIVE || (  wget http://tukaani.org/xz/$ARCHIVE -O $ARCHIVE ) 
echo "-- Extracting $ARCHIVE"
test -d $WORKDIR || ( tar xfvz $ARCHIVE)
#echo "-- Copy files for LZMA"
#find $WORKDIR/src/liblzma -name '*.[hc]' -exec cp {} ..  \;
#cp -f $WORKDIR/src/common/sysdefs.h ..

echo "-- Configure and Build"
test -f $LIBXZA || (cd $WORKDIR && ./configure --prefix=`pwd`/../$OUTDIR --enable-threads=no --disable-xz --disable-xzdec --disable-lzmadec --disable-lzmainfo --disable-lzma-links --disable-scripts --disable-doc --disable-shared --disable-nls --disable-rpath --disable-assembler && make -s install)
SOURCE_FILES=`ar -t $LIBXZA | grep liblzma_la | sed -e 's/liblzma_la-//' | sed -e 's/\.o/\.c/'`
mkdir -p $OUTSRC
for s in $SOURCE_FILES;
do
	echo "file = $s";
	src=`find $WORKDIR -name $s`
	echo "src  = $src"
	cp -f $src $OUTSRC	
	srcdir=`dirname $src`
	cp -f $srcdir/*.h $OUTSRC
done
cp -f $WORKDIR/config.h $OUTSRC

