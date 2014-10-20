#include "yocto/fame/types.hpp"

namespace yocto
{
    namespace fame
    {
        
        
        const char *get_axis_name(size_t dim) throw()
        {
            switch(dim)
            {
                case 0: return "X";
                case 1: return "Y";
                case 2: return "Z";
                default:
                    break;
            }
            return "";
        }
        
        offsets_list:: ~offsets_list() throw()
        {
        }
        
        offsets_list:: offsets_list() throw() : sorted_offsets() {}
        
        offsets_list:: offsets_list( size_t num ) : sorted_offsets( num, as_capacity)
        {
        }
        
        offsets_list:: offsets_list( const offsets_list &other ) : sorted_offsets( other )
        {
        }
        
        void offsets_list:: store( size_t indx )
        {
            (void) insert(indx);
        }
        
        
    }
}
