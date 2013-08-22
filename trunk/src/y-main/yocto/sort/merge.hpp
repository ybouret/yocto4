#ifndef YOCTO_SORT_MERGE_INCLUDED
#define YOCTO_SORT_MERGE_INCLUDED 1

#include "yocto/core/list.hpp"

namespace yocto 
{
    
    namespace core 
    {
        
        template <typename NODE>
        class merging
        {
        public:
            typedef int (*compare)( const NODE *lhs, const NODE *rhs, void *args );
            
            template <template <typename> class LIST_OF>
            static inline 
            void sort( LIST_OF<NODE> &source, compare proc, void *args )
            {
                if( source.size > 1 )
                {
                    core::list_of<NODE> L;
                    core::list_of<NODE> R;
                    const size_t        mid = source.size >> 1;
                    for( size_t i=source.size-mid;i>0;--i) L.push_front( source.pop_back() );
                    for( size_t i=mid;i>0;--i)             R.push_front( source.pop_back() );
                    assert(0==source.size);
                    
                    merging<NODE>::template sort<core::list_of>(L,proc,args);
                    merging<NODE>::template sort<core::list_of>(R,proc,args);
                    merging<NODE>::template fusion<LIST_OF>(source,L,R,proc,args);
                }
            }
            
            
        private:
            //! merge two sorted list
            template <template <typename> class LIST_OF>
            static inline
            void fusion( LIST_OF<NODE> &target, list_of<NODE> &L, list_of<NODE> &R, compare proc, void *args )
            {
                //! while we can compare two nodes
                assert(0==target.size);
                while( L.size > 0 && R.size > 0 )
                {
                    if( proc(L.head,R.head,args) < 0 )
                    {
                        target.push_back( L.pop_front() );
                    }
                    else
                    {
                        target.push_back( R.pop_front() );
                    }
                }
                
                //! append remaining lists
                assert(0==L.size||0==R.size);
                while( L.size > 0 ) target.push_back( L.pop_front() );
                while( R.size > 0 ) target.push_back( R.pop_front() );
            }
        };
        
        
        
    }
    
}

#endif

