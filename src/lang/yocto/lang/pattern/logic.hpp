#ifndef YOCTO_LANG_PATTERN_LOGIC_INCLUDED
#define YOCTO_LANG_PATTERN_LOGIC_INCLUDED 1

#include "yocto/lang/pattern.hpp"

namespace yocto
{
    namespace lang
    {
     
        //______________________________________________________________________
        //
        //! logical operations on operands
        //______________________________________________________________________
        class logical : public pattern
        {
        public:
            p_list operands;
            
            virtual ~logical() throw();
            virtual void reset() throw();
            
        protected:
            explicit logical(const uint32_t id) throw();
            logical(const logical &other);
            
        private:
            YOCTO_DISABLE_ASSIGN(logical);
        };
        
        
        //______________________________________________________________________
        //
        //! logical AND
        //______________________________________________________________________
        class AND : public logical
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('@', '&', '&', '@');
            virtual ~AND() throw();
            
            static  logical *create();
            virtual pattern *clone() const;
            virtual bool     match(source &src, ios::istream &fp);
            
        private:
            explicit AND() throw();
            AND(const AND &);
            YOCTO_DISABLE_ASSIGN(AND);
        };
        
        //______________________________________________________________________
        //
        //! logical OR
        //______________________________________________________________________
        class OR : public logical
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('@', '|', '|', '@');
            virtual ~OR() throw();
            
            static  logical *create();
            virtual pattern *clone() const;
            virtual bool     match(source &src, ios::istream &fp);
            
        private:
            explicit OR() throw();
            OR(const OR &);
            YOCTO_DISABLE_ASSIGN(OR);
        };
        
        //______________________________________________________________________
        //
        //! logical NOT
        //______________________________________________________________________
        class NOT : public logical
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC('@', '!', '!', '@');
            virtual ~NOT() throw();
            
            static  logical *create();
            virtual pattern *clone() const;
            
            //! always empty...
            virtual bool     match(source &src, ios::istream &fp);
            
        private:
            explicit NOT() throw();
            NOT(const NOT &);
            YOCTO_DISABLE_ASSIGN(NOT);
        };

        
    }
}

#endif
