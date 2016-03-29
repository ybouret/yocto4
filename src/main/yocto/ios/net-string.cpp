#include "yocto/ios/net-string.hpp"
#include "yocto/code/utils.hpp"


namespace yocto
{
    namespace ios
    {
        void net_string::write_len(const size_t len, ios::ostream &fp)
        {
            fp("%u:", unsigned(len) );
        }

        void net_string::write(const void *data, const size_t size, ios::ostream &fp)
        {
            assert( !( (data==NULL) && (size>0) ) );
            write_len(size,fp);
            const char *addr =(const char *)data;
            for(size_t i=0;i<size;++i)
            {
                fp.write(addr[i]);
            }
        }

        void net_string::format(const void *data, const size_t size, ios::ostream &fp)
        {
            assert( !( (data==NULL) && (size>0) ) );
            write_len(size,fp);
            const uint8_t *addr =(const uint8_t *)data;
            for(size_t i=0;i<size;++i)
            {
                const uint8_t B = addr[i];
                if(B<32||B>=127)
                {
                    fp << "\\x" << hexa_text[B];
                }
                else
                {
                    switch(B)
                    {
                        case 0x22: fp << "\\\""; break;
                        case 0x5c: fp << "\\\\"; break;
                        default:fp.write(B);
                    }
                }
            }
        }
        


    }
}