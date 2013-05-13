#ifndef YOCTO_LINGUA_PATTERN_JOKER_INCLUDED
#define YOCTO_LINGUA_PATTERN_JOKER_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
    namespace lingua
    {
     
        class joker : public pattern
        {
        public:
            virtual ~joker() throw();
            
        protected:
            explicit joker( pattern *p ) throw();
            pattern *motif;
            joker( const joker &);
            void write( ios::ostream &fp ) const;
            
        private:
            
            YOCTO_DISABLE_ASSIGN(joker);
        };
        
        class optional : public joker
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('J','K','?',' ');
            virtual ~optional() throw();
            
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;
            virtual bool     accept( source &src );
            
            static optional *create(pattern *p);
            
        private:
            explicit optional( pattern *p ) throw();
            optional( const optional &);
            YOCTO_DISABLE_ASSIGN(optional);
        };
        
        class at_least : public joker
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('J', 'K', '>', '=');
            
            virtual ~at_least() throw();
            
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;
            virtual bool     accept( source &src );
            
            static at_least *create(pattern *p,size_t n);
            
            const size_t count;
            
        private:
            explicit at_least( pattern *p, size_t n) throw();
            at_least( const at_least & );
            YOCTO_DISABLE_ASSIGN(at_least);
        };
        
        
    }
}

#endif

