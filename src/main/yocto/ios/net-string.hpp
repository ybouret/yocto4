#ifndef YOCTO_IOS_NET_STRING_INCLUDED
#define YOCTO_IOS_NET_STRING_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/ios/ostream.hpp"

namespace yocto
{

    namespace ios
    {
        struct net_string
        {
            static void write_len(const size_t len, ios::ostream &fp);
            static void write(const void *data, const size_t size, ios::ostream &fp);
            static inline void write(const char *text, ios::ostream &fp)
            {
                write(text, length_of(text), fp);
            }
            static inline void write(const memory::ro_buffer &buf, ios::ostream &fp)
            {
                write(buf.ro(), buf.length(), fp);
            }

            //! prepare C litteral
            static void format(const void *data, const size_t size, ios::ostream &fp);
            static inline void format(const char *text, ios::ostream &fp)
            {
                format(text, length_of(text), fp);
            }
            static inline void format(const memory::ro_buffer &buf, ios::ostream &fp)
            {
                format(buf.ro(), buf.length(), fp);
            }
            
        };
    }

}

#endif

