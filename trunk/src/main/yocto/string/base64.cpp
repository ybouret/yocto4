#include "yocto/string/base64.hpp"
#include "yocto/exception.hpp"
#include <cstring>

namespace yocto
{
    base64:: common:: ~common() throw()
    {
        
    }
    
#if defined(_MSC_VER)
	// init of buf
#pragma warning ( disable : 4351 )
#endif
    base64:: common:: common() throw() :
    q_codec(),
    buf(),
    len(0)
    {
        reset();
    }
    
    
    void base64::common:: reset() throw()
    {
        len = 0;
        memset(buf,0,sizeof(buf));
        Q.free();
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
#define __BYTE0(X)   uint8_t( __SHR(X,2) & __MASK6 )
    
    //-- byte 1: 2 + 4 bits
#define __BYTE1X(X)  ( __SHL( (X) & __MASK2,4) )
#define __BYTE1Y(Y)  ( __SHR(Y,4) & __MASK4    )
#define __BYTE1(X,Y) uint8_t( __BYTE1X(X) | __BYTE1Y(Y) )
    
    //-- byte 2: 4 + 2 bits
#define __BYTE2Y(Y)  ( __SHL((Y)&__MASK4,2)   )
#define __BYTE2Z(Z)  ( __SHR((Z),6) & __MASK2 )
#define __BYTE2(Y,Z) uint8_t( __BYTE2Y(Y) | __BYTE2Z(Z) )
    
    //-- byte 1: 6 bits
#define __BYTE3(Z)   uint8_t( (Z) & __MASK6 )
    
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
                Q.push_back( tab[A0] );
                Q.push_back( tab[A1] );
                Q.push_back( tab[A2] );
                Q.push_back( tab[A3] );
            }
                break;
                
            case 2:
            {
                const uint8_t A0 = __BYTE0(buf[0]);         assert(A0<64);
                const uint8_t A1 = __BYTE1(buf[0],buf[1]);  assert(A1<64);
                const uint8_t A2 = __BYTE2(buf[1],0);       assert(A2<64);
                Q.push_back( tab[A0] );
                Q.push_back( tab[A1] );
                Q.push_back( tab[A2] );
                if( padding ) Q.push_back( '=' );
            }
                break;
                
            case 1:
            {
                const uint8_t A0 = __BYTE0(buf[0]);         assert(A0<64);
                const uint8_t A1 = __BYTE1(buf[0],0);       assert(A1<64);
                Q.push_back( tab[A0] );
                Q.push_back( tab[A1] );
                if( padding )
                {
                    Q.push_back( '=' );
                    Q.push_back( '=' );
                }
            }
                break;
                
            default:
                throw exception("base64::encoder(internal error)");
        }
        
        reset();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    //
    ////////////////////////////////////////////////////////////////////////////
    
    const uint8_t base64:: decoder:: table[256] =
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
    
    base64:: decoder:: decoder() throw()
    {
    }
    
    base64:: decoder:: ~decoder() throw()
    {
    }
    
    void base64:: decoder:: write( char C )
    {
        
        if( len >= 4 )
            emit();
        
        const int B = table[ uint8_t(C) ];
        if( B >= 0xff )
            return;
        else
        {
            if( 3 == len && 0x40 == buf[2] && 0x40 != B )
                throw exception( "base64::decoder(invalid '=')");
            buf[len++] = uint8_t(B);
        }
    }
    
    void base64:: decoder:: flush()
    {
        emit();
    }
    
    
#define __CODE1X(X)  ( ((X)&63) << 2 )
#define __CODE1Y(Y)  ( ((Y)&63) >> 4 )
#define __CODE1(X,Y) uint8_t( __CODE1X(X) | __CODE1Y(Y) )
    
#define __CODE2Y(Y)  ( ( (Y)&15 ) << 4 )
#define __CODE2Z(Z)  ( ( (Z)&63 ) >> 2 )
#define __CODE2(Y,Z) uint8_t( __CODE2Y(Y) | __CODE2Z(Z) )
    
#define __CODE3Z(Z)  ( ( (Z) & 3 ) << 6 )
#define __CODE3(Z,W) uint8_t( __CODE3Z(Z) | (W&63) )
    
    
    void base64:: decoder:: emit()
    {
        switch( len )
        {
            case 0:
            case 1: // junk
                return;
                
            case 2:
            {
                const uint8_t C0 = __CODE1(buf[0],buf[1]);
                Q.push_back(C0);
            }
                break;
                
            case 3:
            {
                const uint8_t C0 = __CODE1(buf[0],buf[1]);
                const uint8_t C1 = __CODE2(buf[1],buf[2]);
                Q.push_back(C0);
                Q.push_back(C1);
            }
                break;
                
            case 4:
            {
                const uint8_t C0 = __CODE1(buf[0],buf[1]);
                const uint8_t C1 = __CODE2(buf[1],buf[2]);
                const uint8_t C2 = __CODE3(buf[2],buf[3]);
                
                Q.push_back(C0);
                Q.push_back(C1);
                Q.push_back(C2);
            }
                break;
                
            default:
                throw exception("base64::decoder(internal error)");
        }
        reset();
    }
    
    
    
    
}
