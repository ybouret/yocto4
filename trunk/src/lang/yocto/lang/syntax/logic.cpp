#include "yocto/lang/syntax/logic.hpp"


namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            
            logical:: ~logical() throw()
            {
                
            }
            
            logical:: logical( const string &id ) :
            rule(id),
            operands()
            {
            }
            
            void logical:: append( rule &sub )
            {
                operands.push_back( new operand(sub) );
            }

            logical:: operand:: operand( rule &r ) throw() :
            sub(r),
            next(0),
            prev(0)
            {
            }
            
            logical:: operand:: ~operand() throw() {}
            
            logical & logical:: operator<<(rule&sub)
            {
                append(sub);
                return *this;
            }
            
        }
        
    }
    
}

namespace yocto
{
    namespace lang
    {
        
        namespace syntax
        {
            aggregate:: aggregate(const string &id) :
            logical(id)
            {
            }
            
            aggregate:: ~aggregate() throw()
            {
                
            }
            

            aggregate & aggregate:: operator+=( rule &r )
            {
                append(r);
                return *this;
            }

        }
    }
}
