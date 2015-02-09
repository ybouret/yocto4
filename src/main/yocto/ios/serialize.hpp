#ifndef YOCTO_IOS_SERIALIZE_INCLUDED
#define YOCTO_IOS_SERIALIZE_INCLUDED 1

#include "yocto/associative/set.hpp"
#include "yocto/string.hpp"

#include <typeinfo>

namespace yocto
{
    namespace ios
    {

        struct serialize
        {
            typedef void (*copy_proc)(void *target,const void *source);

            class item
            {
            public:
                ~item() throw();

                //! 1, 2, 4 or 8 bytes
                item(const string &id, size_t start,size_t bytes);
                item(const item &);

                const string    name;
                const size_t    offset;
                const size_t    length;
                const copy_proc BEcopy;

                const string &key() const throw(); //!< name

            private:
                YOCTO_DISABLE_ASSIGN(item);
            };

            class binary_data;

            class items
            {
            public:
                typedef set<string,item>         database;
                explicit items(size_t n);

                virtual ~items() throw();

                void declare(const string &field, size_t bytes);

                template <typename T>
                inline void add(const string &field)
                {
                    declare(field,sizeof(T));
                }

                template <typename T>
                inline void add(const char *field)
                {
                    const string f(field);
                    declare(f,sizeof(T));
                }

                const item & operator[](const string &field) const;
                size_t check_offset_of( const string &field, const size_t bytes) const;

                //! target: this->linear bytes ; source: this->offset bytes
                void encode(void *target, const void *source) const throw();

                //! target: this->offset bytes ; source: this->linear bytes
                void decode(void *target, const void *source) const throw();

            private:
                database     db;
            public:
                const size_t offset; //!< for binary data
                const size_t linear; //!< for I/O data
            private:
                friend class binary_data;
                const size_t nref;
                YOCTO_DISABLE_COPY_AND_ASSIGN(items);
            };

            class binary_data
            {
            public:
                explicit binary_data(const items&it);
                binary_data(const binary_data &other);
                virtual ~binary_data() throw();
                const items &layout;

                void *      get(const string &field);
                const void *get(const string &field) const;

                template <typename T>
                inline T &as(const string &field)
                {
                    return *(T *) &wksp[layout.check_offset_of(field, sizeof(T))];
                }

                template <typename T>
                inline const T &as(const string &field) const
                {
                    return *(const T *) &wksp[layout.check_offset_of(field, sizeof(T))];
                }

                void write( void *target ) const throw();
                void read( const void *source) throw();

            private:
                size_t   wlen;
                uint8_t *wksp;
                YOCTO_DISABLE_ASSIGN(binary_data);
            };
        };

    }
}

#endif

