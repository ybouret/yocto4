#ifndef YOCTO_LINGUA_PATTERN_LOGIC_INCLUDED
#define YOCTO_LINGUA_PATTERN_LOGIC_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
    namespace lingua
    {
        
        class logical : public pattern
        {
        public:
            virtual ~logical() throw();
            
            p_list operands;
            
            virtual void reset() throw();
            
            
            static pattern *EQUAL(const string &s);
            static pattern *EQUAL(const char   *s);
            static pattern *AMONG(const string &s);
            static pattern *AMONG(const char   *s);

            void append( pattern *p ) throw();
            
        protected:
            explicit logical(uint32_t t) throw();
            logical( const logical & );
            void write( ios::ostream &fp ) const;
            
            //! apply optimize to all operands
            void optimize_all() throw();
            
        private:
            YOCTO_DISABLE_ASSIGN(logical);
        };
        
        
        //! logical AND: must accept all operands
        class AND : public logical
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC(' ','&', '&', ' ' );
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;
            virtual bool     accept( source &src );
            
            virtual ~AND() throw();
            
            static AND *create();
            
            virtual void optimize() throw();
            
        private:
            explicit AND() throw();
            AND( const AND & );
            YOCTO_DISABLE_ASSIGN(AND);
        };
        
        //! logical OR: must accept one operand, lazy from left to right
        class OR : public logical
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC(' ','|', '|', ' ' );
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;
            virtual bool     accept( source &src );
            
            virtual ~OR() throw();
            
            static OR *create();
            
            virtual void optimize() throw();

        private:
            explicit OR() throw();
            OR( const OR & );
            YOCTO_DISABLE_ASSIGN(OR);
        };
        
        
        //! logical NOT: must reject every operand
        class NOT : public logical
        {
        public:
            static const uint32_t tag = YOCTO_FOURCC(' ','!', '=', ' ' );
            virtual pattern *clone() const;
            virtual void     save( ios::ostream &fp ) const;
            virtual bool     accept( source &src );
            
            virtual ~NOT() throw();
            
            static NOT *create();
            virtual void optimize() throw();

        private:
            explicit NOT() throw();
            NOT( const NOT & );
            YOCTO_DISABLE_ASSIGN(NOT);
        };

        
        
        
    }
}

#endif

