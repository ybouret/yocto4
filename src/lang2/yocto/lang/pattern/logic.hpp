#include "yocto/lang/pattern.hpp"


namespace yocto
{
    namespace lang
    {

        class logical : public pattern
        {
        public:
            p_list operands;

            virtual ~logical() throw();

        protected:
            logical(const uint32_t) throw();
            logical(const logical &other);

        private:
            YOCTO_DISABLE_ASSIGN(logical);
        };


    }

}
