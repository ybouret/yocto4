#include "yocto/string/base64.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    base64:: common:: ~common() throw()
    {
        
    }
    
    base64:: common:: common() throw() :
    fifo()
    {
        
    }
    
    bool base64:: common:: query( char &C )
    {
        if( fifo.size() )
        {
            C = fifo.front();
            fifo.pop_front();
            return true;
        }
        else
            return false;
    }
    
    void base64:: common:: store( char C )
    {
        fifo.push_front( C );
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    //
    //
    ////////////////////////////////////////////////////////////////////////////
    const char base64:: encoder:: table_iso[64] = 
    {
        'A','B','C','D','E','F','G','H',
        'I','J','K','L','M','N','O','P',
        'Q','R','S','T','U','V','W','X',
        'Y','Z','a','b','c','d','e','f',
        'g','h','i','j','k','l','m','n',
        'o','p','q','r','s','t','u','v',
        'w','x','y','z','0','1','2','3',
        '4','5','6','7','8','9','+','/'
    };
    
    const char base64:: encoder:: table_url[64] = 
    {
        'A','B','C','D','E','F','G','H',
        'I','J','K','L','M','N','O','P',
        'Q','R','S','T','U','V','W','X',
        'Y','Z','a','b','c','d','e','f',
        'g','h','i','j','k','l','m','n',
        'o','p','q','r','s','t','u','v',
        'w','x','y','z','0','1','2','3',
        '4','5','6','7','8','9','-','_'
    };
    
    
    
    base64:: encoder:: encoder() throw() :
    buf(),
    len(0),
    tab( table_iso ),
    padding(true)
    {
    }
    
    void base64:: encoder:: set_iso() throw()
    {
        tab = table_iso;
    }
    
    void base64:: encoder:: set_url() throw()
    {
        tab = table_url;
    }
    
    base64:: encoder:: ~encoder() throw()
    {
    }
    
    void base64:: encoder:: write( char C )
    {
        if( len >= 3 )
        {
            emit();
        }
        buf[len++] = C;
    }
    
    
    void base64:: encoder:: flush()
    {
        emit();
    }
    
#define __MASK6     0x3F
#define __MASK4     0x0F 
#define __MASK2     0x03
    
#define __SHL(A,n) ( (A) << (n) )
#define __SHR(A,n) ( (A) >> (n) )
    
    //-- byte 0: 6 bits
#define __BYTE0(X)   ( __SHR(X,2) & __MASK6 )  
    
    //-- byte 1: 2 + 4 bits
#define __BYTE1X(X)  ( __SHL( (X) & __MASK2,4) )
#define __BYTE1Y(Y)  ( __SHR(Y,4) & __MASK4    )
#define __BYTE1(X,Y) ( __BYTE1X(X) | __BYTE1Y(Y) )   
    
    //-- byte 2: 4 + 2 bits
#define __BYTE2Y(Y)  ( __SHL((Y)&__MASK4,2)   )
#define __BYTE2Z(Z)  ( __SHR((Z),6) & __MASK2 )
#define __BYTE2(Y,Z) ( __BYTE2Y(Y) | __BYTE2Z(Z) )
    
    //-- byte 1: 6 bits
#define __BYTE3(Z)   ( (Z) & __MASK6 )
    
    void base64:: encoder:: emit()
    {
        switch( len )
        {
            case 0:
                return;
                
            case 3:
            {
                const uint8_t A0 = __BYTE0(buf[0]);         assert(A0<64);
                const uint8_t A1 = __BYTE1(buf[0],buf[1]);  assert(A1<64);
                const uint8_t A2 = __BYTE2(buf[1],buf[2]);  assert(A2<64);
                const uint8_t A3 = __BYTE3(buf[2]);         assert(A3<64);
                fifo.push_back( tab[A0] );
                fifo.push_back( tab[A1] );
                fifo.push_back( tab[A2] );
                fifo.push_back( tab[A3] );
            }
                break;
                
            case 2:
            {
                const uint8_t A0 = __BYTE0(buf[0]);         assert(A0<64);
                const uint8_t A1 = __BYTE1(buf[0],buf[1]);  assert(A1<64);
                const uint8_t A2 = __BYTE2(buf[1],0);       assert(A2<64);
                fifo.push_back( tab[A0] );
                fifo.push_back( tab[A1] );
                fifo.push_back( tab[A2] );
                if( padding ) fifo.push_back( '=' );
            }
                break;
                
            case 1:
            {
                const uint8_t A0 = __BYTE0(buf[0]);         assert(A0<64);
                const uint8_t A1 = __BYTE1(buf[0],0);       assert(A1<64);
                fifo.push_back( tab[A0] );
                fifo.push_back( tab[A1] );
                if( padding ) 
                {
                    fifo.push_back( '=' );
                    fifo.push_back( '=' );
                }
            }
                break;
                
            default:
                throw exception("base64::encoder(internal error)");
        }
        
        
        len = 0;
    }
    
    
    const char base64:: decoder:: table[256] =
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   62, 0xff,   62, 0xff,   63,
        52,   53,   54,   55,   56,   57,   58,   59,   60,   61, 0xff, 0xff, 0xff, 0x40, 0xff, 0xff,
        0xff,    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
        15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25, 0xff, 0xff, 0xff, 0xff,   63,
        0xff,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
        41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    
}
