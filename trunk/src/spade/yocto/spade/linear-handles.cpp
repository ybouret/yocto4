#include "yocto/spade/linear-handles.hpp"


namespace yocto
{
    namespace spade
    {
        
        linear_handles:: linear_handles() throw() :
        vector<linear_ptr>()
        {
        }
        
        linear_handles:: linear_handles(size_t n) :
        vector<linear_ptr>(n,as_capacity)
        {
        }
        
        linear_handles:: ~linear_handles() throw()
        {
        }
        
        size_t linear_handles:: interleaved() const throw()
        {
            size_t ans = 0;
            const vector<linear_ptr> &self = *this;
            for( size_t i=size();i>0;--i)
            {
                assert(self[i]);
                ans += self[i]->item_size();
            }
            return ans;
        }

        void linear_handles:: append( linear &handle )
        {
            push_back( &handle );
        }
    }
}

