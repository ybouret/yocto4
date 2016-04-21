#include "yocto/string/conv.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/code/utils.hpp"

#include <cerrno>

namespace yocto
{

    void   strconv:: append_cchar(const char c, string &ans)
    {
        const uint8_t C(c);
        if(C>=32&&C<127)
        {
            switch(C)
            {
                case '\\': ans += "\\\\"; break;
                case '"' : ans += "\\\""; break;
                case '\'': ans += "\\'";  break;
                default:
                    ans += C;
            }
        }
        else
        {
            switch(C)
            {
                case '\n': ans += "\\r"; break;
                case '\r': ans += "\\n"; break;
                case '\t': ans += "\\t"; break;
                default:
                    ans += "\\";
                    ans += hexa_text[C];
            }
        }
    }

    string strconv:: to_cstring(const string &src)
    {
        string ans;
        const size_t n = src.size();

        for(size_t i=0;i<n;++i)
        {
            append_cchar(src[i],ans);
        }

        return ans;
    }



    size_t strconv:: to_size( const string &txt, const char *ctx  )
    {
        static const char fmt[] = "strconv::to_size(invalid char '%c') for <%s>";
        if( !ctx )        ctx   = "";
        if( txt.size() <= 0 ) throw libc::exception( EINVAL, "strconv::to_size(empty string)");
        size_t ans = 0;
        for( size_t i=0; i < txt.size(); ++i )
        {
            const char C = txt[i];
            ans *= 10;
            switch( C )
            {
                case '0': break;
                case '1': ++ans;    break;
                case '2': ans += 2; break;
                case '3': ans += 3; break;
                case '4': ans += 4; break;
                case '5': ans += 5; break;
                case '6': ans += 6; break;
                case '7': ans += 7; break;
                case '8': ans += 8; break;
                case '9': ans += 9; break;

                default:
                    throw libc::exception( EINVAL, fmt, C, ctx );

            }
        }
        return ans;
    }

    int strconv:: to_int( const string &txt, const char *ctx  )
    {
        static const char fmt[] = "strconv::to_int(invalid char '%c') for <%s>";
        if( !ctx )        ctx   = "";
        if( txt.size() <= 0 ) throw libc::exception( EINVAL, "strconv::to_int(empty string)");
        const size_t n   =  txt.size();
        size_t       i   = 0;
        bool         neg = false;

        switch( txt[i] )
        {
            case '-':
                neg = true;
            case '+':
                if( ++i >= n ) throw libc::exception( EINVAL, "strconv::to_int(no char after '%c') for <%s>", txt[i], ctx);
            default:
                break;
        }
        assert( i < n );
        int ans = 0;
        for( ; i < n; ++i )
        {
            const char C = txt[i];
            ans *= 10;
            switch( C )
            {
                case '0': break;
                case '1': ++ans;    break;
                case '2': ans += 2; break;
                case '3': ans += 3; break;
                case '4': ans += 4; break;
                case '5': ans += 5; break;
                case '6': ans += 6; break;
                case '7': ans += 7; break;
                case '8': ans += 8; break;
                case '9': ans += 9; break;

                default:
                    throw libc::exception( EINVAL, fmt, C, ctx );

            }
        }
        return neg ? -ans: ans;
    }
    
    
    
#define real_t float
#include "conv.cxx"
    
#undef real_t
#define real_t double
#include "conv.cxx"
    
    
    template <>
    int strconv::to<int>( const string &txt, const char *ctx )
    {
        return strconv::to_int(txt,ctx);
    }
    
    template <>
    size_t strconv::to<size_t>( const string &txt, const char *ctx )
    {
        return strconv::to_size(txt,ctx);
    }
    
    template <>
    float strconv::to<float>( const string &txt, const char *ctx )
    {
        return strconv::to_real<float>(txt,ctx);
    }
    
    template <>
    double strconv::to<double>( const string &txt, const char *ctx )
    {
        return strconv::to_real<double>(txt,ctx);
    }
    
}


