#ifndef YOCTO_LANG_PATTERN_JOKER_INCLUDED
#define YOCTO_LANG_PATTERN_JOKER_INCLUDED 1

#include "yocto/lang/pattern.hpp"

namespace yocto
{
    namespace lang
    {
        //______________________________________________________________________
        //
        //! a joker is basically a smart pointer to another pattern
        //______________________________________________________________________
        class joker : public pattern
        {
        public:
            virtual ~joker() throw();
            virtual void reset() throw();

            virtual void refactor();
            
        protected:
            explicit joker( const uint32_t id, pattern *p ) throw();
            pattern *motif;
            void __viz( const void *parent, ios::ostream &fp ) const;
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(joker);
        };
        
        //______________________________________________________________________
        //
        //! '?'
        //______________________________________________________________________
        class optional : public joker
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('@', '?', '@',' ');
            virtual ~optional() throw();
            
            //! the pointer is taken care of
            static  pattern *create( pattern *p );
            
            //! clone
            virtual pattern *clone() const;
            virtual void     viz( ios::ostream & ) const;

            //! 0 or 1 exactly
            virtual bool match( source &, ios::istream &fp );
            
        private:
            explicit optional(pattern *p ) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(optional);
        };
        
        
        //______________________________________________________________________
        //
        //! '*' or '+'
        //______________________________________________________________________
        class at_least : public joker
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('@', '>', '=', '@');
            const size_t value;
            
            virtual ~at_least() throw();
            
            //! the pointer is taken care of
            static pattern *create(pattern *p, size_t n);
            
            //! clone
            virtual pattern *clone() const;
            virtual void     viz( ios::ostream & ) const;

            //! true is count >= value
            virtual bool match( source &, ios::istream &);
            
        private:
            explicit at_least(pattern *p, size_t n) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(at_least);
        };
        
        pattern *zero_or_more(pattern *p);
        pattern *one_or_more(pattern *p);
        
        //______________________________________________________________________
        //
        //! {n,m}
        //______________________________________________________________________
        class counting : public joker
        {
        public:
            static const uint32_t tag =  YOCTO_FOURCC('@', '{', '}', '@');
            
            const size_t n;
            const size_t m;
            virtual ~counting() throw();
            
            //! the pointer is taken care of
            static pattern *create(pattern *p, const size_t lo, const size_t up);
            
            //! clone
            virtual pattern *clone() const;
            virtual void     viz( ios::ostream & ) const;

            //! true if a count of at least n and max m is found
            virtual bool match(source &, ios::istream &);
            
        private:
            explicit counting(pattern *p, const size_t lo, const size_t up) throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(counting);
        };
        
        
    }
}

#endif

