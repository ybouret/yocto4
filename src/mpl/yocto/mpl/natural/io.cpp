#include "yocto/mpl/natural.hpp"
#include "yocto/string.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/bswap.hpp"
#include <iostream>

namespace yocto
{
    namespace mpl
    {

        static inline bool is0(char C) throw() { return '0' == C; }

        void natural:: display( std::ostream &os ) const
        {
            YOCTO_CHECK_MPN(*this);
            std::ios_base::fmtflags flags = os.flags();
            if( flags &  std::ios::hex)
            {
                if(size<=0)
                {
                    os << "0x0";
                }
                else
                {
                    string s;
                    for(size_t i=0;i<size;++i)
                    {
                        const uint8_t B  = byte[i];
                        const uint8_t lo = B      & 0x0f;
                        s.append( hexa_char[lo]);
                        const uint8_t hi = (B>>4) & 0x0f;
                        s.append(hexa_char[hi]);
                    }
                    s.trim(is0);
                    mreverse((char*)(s.rw()),s.length());
                    os << s;
                }
            }
            else
            {
                //assume decimal
                if(size<=0)
                {
                    os << "0";
                }
                else
                {
                    string s;
                    const natural ten(10);
                    natural       q;
                    natural       r;
                    natural       x = *this;
                    while(x.size>0)
                    {
                        natural::split(q,r,x,ten);
                        assert(r.size<=1);
                        s.append('0'+r.byte[0]);
                        x=q;
                    }
                    s.trim(is0);
                    mreverse((char*)(s.rw()),s.length());
                    os << s;
                }
            }
        }

        double natural:: to_real() const
        {
            double ans = 0;
            uint8_t *q=byte+size;
            for(size_t i=size;i>0;--i)
            {
                ans *= 256.0;
                ans += double(*(--q));
            }
            return ans;
        }

        void natural::save(ios::ostream &fp) const
        {
            fp.emit<uint32_t>(size);
            fp.save(byte,size);
        }

        natural natural::load(ios::istream &fp)
        {
            const size_t sz = fp.read<uint32_t>();
            natural      ans(sz,as_capacity);
            fp.load(ans.byte,sz);
            ans.update();
            return ans;
        }

    }

}

