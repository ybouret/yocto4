#include "yocto/pack/q-codec.hpp"

namespace yocto
{
    namespace pack
    {
        q_codec:: q_codec() throw() : Q()
        {
        }
        
        q_codec:: ~q_codec() throw() {}
        
        bool q_codec:: query(char &C)
        {
            if( Q.size() > 0 )
            {
                C = Q.front();
                Q.pop_front();
                return true;
            }
            else
            {
                return false;
            }
        }
        
        void q_codec:: store(char C)
        {
            Q.push_front(C);
        }

        q_codec::const_iterator q_codec:: begin() const throw()
        {
            return Q.begin();
        }

        q_codec::const_iterator q_codec:: end() const throw()
        {
            return Q.end();
        }

    }
}
