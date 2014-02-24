#include "yocto/pack/rle.hpp"

namespace yocto
{
    namespace pack
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////
        rle::encoder:: encoder() throw() : chr(-1), num(0)
        {
        }
        
        rle::encoder:: ~encoder() throw()
        {
            
        }
        
        void rle:: encoder:: reset() throw()
        {
            Q.free();
            chr = -1;
            num =  0;
        }
        
        void rle:: encoder:: emit()
        {
            assert(chr>=0);
            assert(chr<256);
            assert(num>0);
            
            if(num<=1)
            {
                Q.push_back(chr);
            }
            else
            {
                assert(num<=257);
                const uint8_t rep(num-2);
                Q.push_back(chr);
                Q.push_back(chr);
                Q.push_back(rep);
            }
            
            chr = -1;
            num =  0;
        }
        
        void rle:: encoder:: write( char C )
        {
            const int b = uint8_t(C);
            if(chr<0)
            {
                chr = b;
                num = 1;
            }
            else
            {
                if(b==chr)
                {
                    assert(num>0);
                    assert(num<257);
                    if(++num>=257)
                    {
                        emit();
                    }
                }
                else
                {
                    emit();
                    chr = b;
                    num = 1;
                }
            }
        }
        
        void rle:: encoder:: flush()
        {
            if(chr>=0)
            {
                emit();
            }
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        //
        //
        ////////////////////////////////////////////////////////////////////////
        rle::decoder:: decoder() throw() :
        flag(wait_for_first),
        chr(-1)
        {
        }
        
        rle::decoder:: ~decoder() throw()
        {
        }
        
        void rle::decoder:: reset() throw()
        {
            Q.free();
            chr = -1;
            flag = wait_for_first;
        }
        
        void rle::decoder:: write(char C)
        {
            const uint8_t b(C);
            switch(flag)
            {
                case wait_for_first:
                    assert(chr<0);
                    chr = b;
                    flag = wait_for_second;
                    break;
                    
                case wait_for_second:
                    assert(chr>=0);
                    assert(chr<256);
                    if(b==chr)
                    {
                        flag = wait_for_count;
                    }
                    else
                    {
                        Q.push_back(chr);
                        chr  = b;
                        flag = wait_for_second;
                    }
                    break;
                    
                case wait_for_count:
                    assert(chr>=0);
                    assert(chr<256);
                    for(size_t i=2+int(b);i>0;--i)
                    {
                        Q.push_back(chr);
                    }
                    chr  = -1;
                    flag = wait_for_first;
                    break;
            }
        }
        
        void rle::decoder:: flush()
        {
            switch(flag)
            {
                case wait_for_first:
                    assert(chr<0);
                    break;
                    
                case wait_for_second:
                    assert(chr>=0);
                    assert(chr<256);
                    Q.push_back(chr);
                    chr  = -1;
                    flag = wait_for_first;
                    break;
                    
                case wait_for_count:
                    assert(chr>=0);
                    assert(chr<256);
                    //impossible to know => assume allright
                    Q.push_back(chr);
                    Q.push_back(chr);
                    chr  = -1;
                    flag = wait_for_first;
                    break;
            }
        }
        
        
    }
}
