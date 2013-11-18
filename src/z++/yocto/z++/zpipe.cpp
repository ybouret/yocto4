#include "yocto/z++/zpipe.hpp"

#include "yocto/zlib/zlib.h"
#include "yocto/exception.hpp"

#include "yocto/code/round.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/memory/global.hpp"

#include <cstring>
#include <cstdlib>

namespace yocto
{
	
    namespace zlib
    {
		
		
		
        static voidpf _zpipe_acquire( voidpf , uInt items, uInt size )
        {
            return memory::global:: __calloc(items,size);
        }
		
        static void   _zpipe_release( voidpf, voidpf address ) throw()
        {
			memory::global:: __free( address );
        }
		
        static void _zpipe_init( z_stream & strm )
        {
            memset( & strm, 0, sizeof(strm) );
            strm.zalloc = _zpipe_acquire;
            strm.zfree  = _zpipe_release;
            strm.opaque = NULL;
			
        }
		
        zpipe:: ~zpipe() throw()
        {
			
        }
		
        zpipe:: zpipe(const size_t chunk_size ) :
		CHUNK( YOCTO_MEMALIGN(chunk_size) ),
		wksp_( CHUNK << 1 ),
		in(  wksp_()     ),
		out( in + CHUNK  ),
		eof_( true )
        {
        }
		
		
        size_t zpipe:: _read( ios::istream &source )
        {
            eof_       = false;
            size_t ans = 0;
            while( ans < CHUNK ) {
                if( source.query( in[ans] ) ) {
                    ++ans;
                    continue;
                } else {
                    eof_ = true;
                    return ans;
                }
				
            }
            return ans;
        }
		
        void zpipe:: _save( ios::ostream &target, size_t len ) const
        {
            assert( len <= CHUNK );
            const char *C = out;
            YOCTO_LOOP( len, target.write( *(C++) ) );
        }
		
        void zpipe:: def( ios::ostream &target, ios::istream &source, size_t level, memory::blender &blend)
        {
            z_stream     strm;
            _zpipe_init( strm );
            if( deflateInit(&strm, level % 10) != Z_OK )
                throw exception( "deflateInit" );
			
            try {
				
                /* compress until end of file */
                int flush = Z_FINISH;
                do {
                    strm.avail_in = _read( source );
                    flush         = eof_ ? Z_FINISH : Z_NO_FLUSH;
                    strm.next_in  = (Bytef*)in;
					
                    /* run deflate() on input until output buffer not full, finish
                     compression if all of source has been read in */
                    do {
                        strm.avail_out = CHUNK;
                        strm.next_out  = (Bytef*)out;
                        (void)deflate(&strm, flush);      /* no bad return value */
                        //assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
                        const size_t length = CHUNK - strm.avail_out;
                        blend.scramble(out,length);
                        _save( target, length );
                    } while (strm.avail_out == 0);
                    assert(strm.avail_in == 0);     /* all input will be used */
					
                    /* done when last data in file processed */
                } while (flush != Z_FINISH);
                //assert(ret == Z_STREAM_END);        /* stream will be complete */
				
                /* clean up and return */
                deflateEnd( &strm );
            } catch (...) {
                deflateEnd( &strm );
                throw;
            }
			
        }
		
		
        void zpipe:: inf( ios::ostream &target, ios::istream &source, memory::blender &blend)
        {
            z_stream     strm;
            _zpipe_init( strm );
            if( inflateInit( &strm ) != Z_OK ) {
                throw exception( "inflateInit" );
            }
			
            try {
                int ret = Z_OK;
                /* decompress until deflate stream ends or end of file */
                do {
                    strm.avail_in = _read( source );
                    blend.scramble(in,strm.avail_in);
                    if (strm.avail_in == 0)
                        break;
                    strm.next_in = (Bytef *)in;
					
                    /* run inflate() on input until output buffer not full */
                    do {
                        strm.avail_out = CHUNK;
                        strm.next_out  = (Bytef *)out;
                        ret = inflate(&strm, Z_NO_FLUSH);
                        switch( ret ) {
							case Z_OK:
							case Z_STREAM_END:
								break;
								
							default:
								throw exception( "inflate(%s)", zError( ret ) );
                        }
                        const size_t length = CHUNK-strm.avail_out;
                        _save( target, length );
                    } while (strm.avail_out == 0);
					
                    /* done when inflate() says it's done */
                } while (ret != Z_STREAM_END);
				
                /* clean up and return */
                (void)inflateEnd(&strm);
				
            } catch(...) {
                inflateEnd( &strm );
                throw;
            }
        }
		
    }
	
	
}
