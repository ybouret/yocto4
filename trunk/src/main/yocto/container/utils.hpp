#ifndef YOCTO_CONTAINER_UTILS_INCLUDED
#define YOCTO_CONTAINER_UTILS_INCLUDED 1

#include "yocto/exception.hpp"

namespace yocto {

    namespace core
    {

        template <typename T,typename ITER>
        T scan_min( ITER first, const ITER last) throw()
        {
            assert(first!=last);
            T ans( *first );
            for(++first;first!=last;++first)
            {
                const T tmp(*first);
                if(tmp<ans)
                {
                    ans = tmp;
                }
            }
            return ans;
        }

        template <typename T,typename ITER>
        T scan_max( ITER first, const ITER last) throw()
        {
            assert(first!=last);
            T ans( *first );
            for(++first;first!=last;++first)
            {
                const T tmp(*first);
                if(tmp>ans)
                {
                    ans = tmp;
                }
            }
            return ans;
        }
    }

    template <typename SEQ>
    typename SEQ::type find_min_of( const SEQ &seq ) throw()
    {
        assert(seq.size()>0);
        return core::scan_min<typename SEQ::mutable_type,typename SEQ::const_iterator>(seq.begin(),seq.end());
    }

    template <typename SEQ>
    typename SEQ::type find_max_of( const SEQ &seq ) throw()
    {
        assert(seq.size()>0);
        return core::scan_max<typename SEQ::mutable_type,typename SEQ::const_iterator>(seq.begin(),seq.end());
    }

}

#endif
