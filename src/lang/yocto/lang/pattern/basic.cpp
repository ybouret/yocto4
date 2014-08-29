#include "yocto/lang/pattern/basic.hpp"

namespace yocto
{
    namespace lang
    {
        
        one_char:: ~one_char() throw() {}
        
        one_char:: one_char( const uint32_t user_type ) throw() :
        pattern(user_type)
        {
        }
        
        
        bool one_char:: match(source &src,ios::istream &fp)
        {
            assert( 0 == size );
            t_char *ch = src.get(fp);
           
            if(ch)
            {
                if( is_valid(ch->code) )
                {
                    push_back(ch);
                    return true;
                }
                else
                {
                    src.unget(ch);
                    return false;
                }
            }
            else
                return false;
        }
        
        
        single:: ~single() throw()
        {
        }
        
        single:: single(const int which) throw() :
        one_char(0),
        value(which)
        {
            data = (void*)&value;
        }
        
        
    }

}
