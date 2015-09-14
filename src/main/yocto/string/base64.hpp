#ifndef YOCTO_STRING_BASE64_INCLUDED
#define YOCTO_STRING_BASE64_INCLUDED 1

#include "yocto/pack/q-codec.hpp"
#include "yocto/sequence/list.hpp"

namespace yocto
{
    
    struct base64
    {
        
        class common : public pack::q_codec
        {
        public:
            virtual ~common() throw();
            
            virtual void       reset() throw();
            
        protected:
            explicit common() throw();
            uint8_t     buf[4]; //!< only 3 out of 4 are used
            size_t      len;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(common);
        };
        
        class encoder : public common
        {
        public:
            static const char table_iso[64];
            static const char table_url[64];
            
            explicit encoder() throw();
            virtual ~encoder() throw();
            
            virtual void write( char C );
            virtual void flush();
            
            void set_url() throw();
            void set_iso() throw();
            
            
            
        private:
            const char *tab;
            void    emit();
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(encoder);
            
        public:
            bool padding;
        };
        
        
        class decoder : public common
        {
        public:
            static const uint8_t table[256];
            explicit decoder() throw();
            virtual ~decoder() throw();
            
            virtual void write( char C );
            virtual void flush();
            
        private:
            void emit();
            YOCTO_DISABLE_COPY_AND_ASSIGN(decoder);
        };
        
        class IO : public singleton<IO>
        {
        public:
            mutable encoder Encoder;
            mutable decoder Decoder;

            string Encode(const string &) const;
            string Decode(const string &) const;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(IO);
            explicit IO() throw();
            virtual ~IO() throw();
            friend class singleton<IO>;

            static const char                 name[];
            static const threading::longevity life_time=0;
        };

    };
    
}

#endif
