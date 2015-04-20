#include "yocto/lang/pattern.hpp"
#include "yocto/code/fourcc.hpp"


namespace yocto
{
    namespace lang
    {

        class logical : public pattern
        {
        public:
            p_list operands;

            virtual ~logical() throw();
            void     append( pattern *p ) throw();

        protected:
            logical(const uint32_t) throw();
            logical(const logical &other);

        private:
            YOCTO_DISABLE_ASSIGN(logical);

        };


        class AND : public logical
        {
        public:
            static const uint32_t UUID = YOCTO_FOURCC('<', '&', '&' , '>');
            virtual ~AND() throw();

            virtual pattern *clone() const;
            virtual bool     match(Y_LANG_PATTERN_MATCH_ARGS) const;
            static  logical *create();
            
        private:
            AND() throw();
            AND(const AND &);
            
            YOCTO_DISABLE_ASSIGN(AND);
        };

    }

}
