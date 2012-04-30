

namespace yocto 
{
    namespace swamp 
    {
        template <>
        region_base<UTYPE>:: region_base( size_t n ) throw() :
        dimensions(n)
        {
        }
        
        template <>
        region_base<UTYPE>:: ~region_base() throw()
        {
            
        }
        
        template <>
        UTYPE region_base<UTYPE>:: setup( const void *pmin, const void *pmax, const void *plen)
        {
            assert(pmin);
            assert(pmax);
            assert(plen);
            UTYPE *vmin   = (UTYPE *)pmin;
            UTYPE *vmax   = (UTYPE *)pmax;
            UTYPE *length = (UTYPE *)plen;
            UTYPE ans = 1;
            for( size_t i=0; i < dimensions; ++i )
            {
                if( vmin[i] > vmax[i] ) cswap(vmin[i], vmax[i]);
                ans *= ( length[i] = vmax[i] - vmin[i] );
            }
            return ans;
        }
        
    }
}
