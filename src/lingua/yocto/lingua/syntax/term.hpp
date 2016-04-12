#ifndef YOCTO_LINGUA_SYNTAX_TERM_INCLUDED
#define YOCTO_LINGUA_SYNTAX_TERM_INCLUDED 1

#include "yocto/lingua/syntax/rule.hpp"


namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {

            //! will accept lexeme with same label
            class terminal : public rule
            {
            public:
                static const uint32_t UUID = YOCTO_FOURCC('T','E','R','M');
                explicit terminal(const string &id);
                virtual ~terminal() throw();

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual void viz(ios::ostream &fp) const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(terminal);
            };
        }

    }
}


#endif
