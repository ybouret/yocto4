#ifndef YOCTO_LINGUA_PATTERN_JOKER_INCLUDED
#define YOCTO_LINGUA_PATTERN_JOKER_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
    namespace lingua
    {
     
        //______________________________________________________________________
        //
        //! smart pointer
        //______________________________________________________________________
        class joker : public pattern
        {
        public:
            virtual ~joker() throw();
            virtual void optimize() throw();
            virtual void reset() throw();
            
        protected:
            explicit joker( uint32_t t, pattern *p ) throw();
            pattern *motif;
            joker( const joker &);
            void write( ios::ostream &fp ) const;
            void __viz( const void *parent, ios::ostream &fp ) const;
            
        private:
            
            YOCTO_DISABLE_ASSIGN(joker);
        };
        
        //______________________________________________________________________
        //
        //! '?'
        //______________________________________________________________________
        class optional : public joker
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('J','K',' ','?');
            virtual ~optional() throw();
            Y_LINGUA_PATTERN_API();

            virtual bool     accept( source &src );
            
            static optional *create(pattern *p);
            
        private:
            explicit optional( pattern *p ) throw();
            optional( const optional &);
            YOCTO_DISABLE_ASSIGN(optional);
        };
        
        //______________________________________________________________________
        //
        //! '*/+'
        //______________________________________________________________________
        class at_least : public joker
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('J', 'K', '>', '=');
            
            virtual ~at_least() throw();
            Y_LINGUA_PATTERN_API();
            
            virtual bool     accept( source &src );
            
            static at_least *create(pattern *p,size_t n);
            
            const size_t count;
            
        private:
            explicit at_least( pattern *p, size_t n) throw();
            at_least( const at_least & );
            YOCTO_DISABLE_ASSIGN(at_least);
        };
        
        pattern * zero_or_more( pattern *p );
        pattern * one_or_more( pattern * p);
        
        //______________________________________________________________________
        //
        // couting
        //______________________________________________________________________
        class counting : public joker
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('J', 'K', '{', '}');

            virtual ~counting() throw();
            Y_LINGUA_PATTERN_API();

            virtual bool     accept( source &src );
            
            const size_t nmin;
            const size_t nmax;
            
            static counting *create( pattern *p, size_t a, size_t b );
            
        private:
            explicit counting( pattern *p, size_t a, size_t b) throw();
            counting( const counting &other );
            YOCTO_DISABLE_ASSIGN(counting);
        };
        
        
    }
}

#endif

