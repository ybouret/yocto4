#include "yocto/ios/codec.hpp"

namespace yocto
{
    namespace ios
    {
        codec:: ~codec() throw() {}
        codec::  codec() throw() {}
        
        string codec:: to_string()
        {
            string ans;
            char   C=0;
            while( query(C) ) ans.append(C);
            return ans;
        }
        
        string codec:: to_string( const void *data, size_t size )
        {
            assert( !(NULL==data && size>0) );
            reset();
            append( (const char*)data, size );
            flush();
            return to_string();
        }
        
        string codec:: to_string( const memory::ro_buffer &data )
        {
            ostream &self = *this;
            reset();
            ( self << data ).flush();
            return to_string();
        }
        
        void codec:: filter( ios::ostream &output, ios::istream &input )
        {
            //reset();
            char C = 0;
            while( input.query(C) )
            {
                this->write(C);
                while( query(C) )
                {
                    output.write(C);
                }
            }
            this->flush();
            while( query(C) )
            {
                output.write(C);
            }
            output.flush();
        }

        
    }
}
