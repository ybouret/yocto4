#include "yocto/rx/lexical/sub-lexer.hpp"

namespace yocto
{
    namespace regex
    {
        
        namespace lexical
        {
            sub_lexer:: ~sub_lexer() throw()
            {
                
            }
           
            sub_lexer:: sub_lexer( const string &id ) :
            name( id ),
            rules_()
            {
            }
            
            
            const string & sub_lexer:: key() const throw() { return name; }
            
            void sub_lexer:: make( pattern *p, const action &a )
            {
                rules_.push_back( lexical::make::create(p,a) );
            }
            
        }
    }
}