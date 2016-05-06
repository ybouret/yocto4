VERSION=-5.2.2
ARCHIVE=xz$VERSION.tar.gz
WORKDIR=xz$VERSION

echo "-- Fetching $ARCHIVE"
test -f $ARCHIVE || (  wget http://tukaani.org/xz/$ARCHIVE -O $ARCHIVE ) 
echo "-- Extracting $ARCHIVE"
test -d $WORKDIR || ( tar xfvz $ARCHIVE)
echo "-- Configuring..."
cd $WORKDIR && ./configure --prefix=`pwd`/../sandbox --disable-threads --disable-xz --disable-xzdec --disable-lzmadec --disable-lzmainfo --disable-lzma-links --disable-scripts --disable-doc --disable-shared --disable-nls --disable-rpath && make install



