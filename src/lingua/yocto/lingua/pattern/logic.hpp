#ifndef YOCTO_LINGUA_PATTERN_LOGIC_INCLUDED
#define YOCTO_LINGUA_PATTERN_LOGIC_INCLUDED 1

#include "yocto/lingua/pattern.hpp"

namespace yocto
{
    namespace lingua
    {

        class logical : public pattern
        {
        public:
            p_list operands;

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~logical() throw();
            virtual void __out(ios::ostream &fp) const;


        protected:
            explicit logical(const uint32_t t) throw();
            logical(const logical &other);

        private:
            YOCTO_DISABLE_ASSIGN(logical);
        };

    }
}

namespace yocto
{
    namespace lingua
    {

        class AND : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '&', '&', '>');

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~AND() throw();
            virtual pattern *clone() const;
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;

            static AND *create();

        protected:
            explicit AND() throw();
            AND(const AND &other);

        private:
            YOCTO_DISABLE_ASSIGN(AND);
        };
        
    }

}


namespace yocto
{
    namespace lingua
    {

        class OR : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '|', '|', '>');

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~OR() throw();
            virtual pattern *clone() const;
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;

            static OR *create();

        protected:
            explicit OR() throw();
            OR(const OR &other);

        private:
            YOCTO_DISABLE_ASSIGN(OR);
        };
        
    }
    
}


namespace yocto
{
    namespace lingua
    {

        class NONE : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '!', '!', '>');

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~NONE() throw();
            virtual pattern *clone() const;
            virtual bool     match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const;

            static NONE *create();

        protected:
            explicit NONE() throw();
            NONE(const NONE &other);

        private:
            YOCTO_DISABLE_ASSIGN(NONE);
        };
        
    }
    
}




#endif
