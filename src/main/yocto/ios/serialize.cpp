#include "yocto/ios/serialize.hpp"
#include "yocto/code/endian.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace ios
    {
        static inline void copy_proc1(void *target, const void *source) throw()
        {
            assert(target); assert(source); assert(target!=source);
            *(uint8_t *)target = *(const uint8_t *)source;
        }


        static inline void copy_proc2(void *target, const void *source) throw()
        {
            assert(target); assert(source); assert(target!=source);
            uint8_t       *t = (uint8_t *)target;
            const uint8_t *s = (const uint8_t *)source;
#if YOCTO_BYTE_ORDER == YOCTO_BIG_ENDIAN
            t[0] = s[0];
            t[1] = s[1];
#else
            t[0] = s[1];
            t[1] = s[0];
#endif
        }


        static inline void copy_proc4(void *target, const void *source) throw()
        {
            assert(target); assert(source); assert(target!=source);
            uint8_t       *t = (uint8_t *)target;
            const uint8_t *s = (const uint8_t *)source;
#if YOCTO_BYTE_ORDER == YOCTO_BIG_ENDIAN
            t[0] = s[0];
            t[1] = s[1];
            t[2] = s[2];
            t[3] = s[3];
#else
            t[0] = s[3];
            t[1] = s[2];
            t[2] = s[1];
            t[3] = s[0];
#endif
        }

        static inline void copy_proc8(void *target, const void *source) throw()
        {
            assert(target); assert(source); assert(target!=source);
            uint8_t       *t = (uint8_t *)target;
            const uint8_t *s = (const uint8_t *)source;
#if YOCTO_BYTE_ORDER == YOCTO_BIG_ENDIAN
            t[0] = s[0];
            t[1] = s[1]
            t[2] = s[2];
            t[3] = s[3];
            t[4] = s[4];
            t[5] = s[5];
            t[6] = s[6];
            t[7] = s[7];
#else
            t[0] = s[7];
            t[1] = s[6];
            t[2] = s[5];
            t[3] = s[4];
            t[4] = s[3];
            t[5] = s[2];
            t[6] = s[1];
            t[7] = s[0];
#endif
        }


        serialize::copy_proc __dispatch_BEcopy(size_t length)
        {
            switch (length)
            {
                case 1:
                    return copy_proc1;

                case 2:
                    return copy_proc2;

                case 4:
                    return copy_proc4;

                case 8:
                    return copy_proc8;

                default:
                    break;
            }
            throw exception("serialize item: invalid length=%u",unsigned(length));
        }

        serialize::item:: item( const string &id, const size_t start, const size_t bytes) :
        name(id),
        offset(start),
        length(bytes),
        BEcopy(__dispatch_BEcopy(length) )
        {
        }

        serialize:: item:: ~item() throw()
        {
        }
        
        serialize:: item:: item(const item &it) :
        name( it.name ),
        offset(it.offset),
        length(it.length),
        BEcopy(it.BEcopy)
        {
        }

        const string & serialize:: item:: key() const throw()
        {
            return name;
        }


    }
    
}

#include "yocto/code/round.hpp"

namespace yocto
{
    namespace ios
    {

        serialize::items:: items(size_t n) :
        db(n,as_capacity),
        offset(0),
        linear(0),
        nref(0)
        {
        }

        serialize::items:: ~items() throw()
        {
        }

        void serialize:: items:: declare(const string &field, size_t bytes)
        {
            if(nref>0)
            {
                throw exception("serialize: items in use");
            }
            assert(0==offset%4);
            const item it(field,offset,bytes);
            if(!db.insert(it))
            {
                throw exception("serialize: mutliple items '%s'", field.c_str());
            }
            (size_t &)offset += bytes;
            (size_t &)linear += bytes;
            (size_t &)offset  = YOCTO_ROUND4(offset);
        }

        const serialize::item & serialize:: items:: operator[](const string &field) const
        {
            const item *p = db.search(field);
            if(!p)
                throw exception("no serialize.item '%s'", field.c_str());
            return *p;
        }

        size_t serialize::items:: check_offset_of( const string &field, const size_t bytes) const
        {
            const item &it = (*this)[field];
            if(it.length!=bytes)
                throw exception("serialize['%s']: length=%u!=%u",field.c_str(),unsigned(it.length),unsigned(bytes));
            return it.offset;
        }

        void serialize::items:: encode(void *target, const void *source) const throw()
        {
            assert(target);
            assert(source);
            assert(target!=source);
            uint8_t       *t = (uint8_t       *)target;
            const uint8_t *s = (const uint8_t *)source;
            database::const_iterator p = db.begin();
            for(register size_t i=db.size();i>0;--i,++p)
            {
                const item  &it = *p;
                it.BEcopy(t,&s[it.offset]);
                t += it.length;
            }

        }

        void serialize::items:: decode(void *target, const void *source) const throw()
        {
            assert(target);
            assert(source);
            assert(target!=source);
            uint8_t       *t = (uint8_t       *)target;
            const uint8_t *s = (const uint8_t *)source;
            database::const_iterator p = db.begin();
            for(register size_t i=db.size();i>0;--i,++p)
            {
                const item  &it = *p;
                it.BEcopy( &t[it.offset], s);
                s += it.length;
            }
        }



    }

}


namespace yocto
{
    namespace ios
    {

        serialize::binary_data::binary_data( const items &it ) :
        layout(it),
        wlen(layout.offset),
        wksp(memory::kind<memory::global>::acquire_as<uint8_t>(wlen))
        {
            ++(size_t&)(layout.nref);
        }

        serialize::binary_data:: ~binary_data() throw()
        {
            memory::kind<memory::global>::release_as(wksp,wlen);
            --(size_t&)(layout.nref);
        }

        void * serialize:: binary_data::get(const string &field)
        {
            return &wksp[ layout[field].offset ];
        }

        const void *serialize:: binary_data::get(const string &field) const
        {
            return &wksp[ layout[field].offset ];
        }

        serialize::binary_data::binary_data(const binary_data &other) :
        layout(other.layout),
        wlen(layout.offset),
        wksp(memory::kind<memory::global>::acquire_as<uint8_t>(wlen))
        {
            ++(size_t&)(layout.nref);
        }


        void serialize:: binary_data:: write( void *target ) const throw()
        {
            layout.encode(target,wksp);
        }

        void serialize:: binary_data:: read( const void *source ) throw()
        {
            layout.decode(wksp,source);
        }
    }

}

