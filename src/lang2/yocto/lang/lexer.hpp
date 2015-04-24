#ifndef YOCTO_LANG_LEXER_INCLUDED
#define YOCTO_LANG_LEXER_INCLUDED 1

#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"


namespace yocto
{
    namespace lang
    {

        //! lexer
        class lexer : public object
        {
        public:
            explicit lexer(const string &id, const string &root_scanner);
            explicit lexer(const char   *id, const char   *root_scanner);
            virtual ~lexer() throw();

            const string name;
            int          line;

            lexical::scanner & root() throw();

            //! get/create-on-the-fly scanner
            lexical::scanner & operator[](const string &id);

            //! get/create scanner, wrapper
            lexical::scanner & operator[](const char   *id);


            //! jump to new scanner
            void jump(const string &id);

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            typedef lexical::scanner::ptr p_scanner;
            typedef set<string,p_scanner> scannerDB;

            lexical::scanner *scan;     //!< active scanner
            lexical::scanner *Root;     //!< root scanner
            scannerDB         scanners;

            void initialize(const string &root_scanner);
        };

    }

}

#endif

