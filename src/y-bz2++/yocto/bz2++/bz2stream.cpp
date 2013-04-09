#include "yocto/bz2++/bz2stream.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace bzlib
    {
        ////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////

        bz2stream:: ~bz2stream() throw()
        {
            bzfile = 0;
        }
        
        
        bz2stream:: bz2stream() throw() :
        bzerror( BZ_OK ),
        bzfile(0)
        {
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////
        ibz2stream:: ibz2stream( const string &filename ) :
        bz2stream(),
        ios::istream(),
        fp( filename ),
        last_close(0)
        {
            bzfile = BZ2_bzReadOpen( &bzerror, fp.__get(), 0, 0, 0, 0);
            if( !bzfile )
                throw exception("BZ2_bzReadOpen(%s)", filename.c_str());
        }
        
        ibz2stream:: ~ibz2stream() throw()
        {
            int * err = &bzerror;
            if( last_close )
            {
                err = last_close;
                *err = BZ_OK;
            }
            BZ2_bzReadClose(err,bzfile);
        }
        
    }
}
