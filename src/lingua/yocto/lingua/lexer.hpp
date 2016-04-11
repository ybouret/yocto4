#ifndef YOCTO_LINGUA_LEXER_INCLUDED
#define YOCTO_LINGUA_LEXER_INCLUDED 1

#include "yocto/lingua/lexical/scanner.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/addr-list.hpp"

namespace yocto
{
    namespace lingua
    {

        class lexer : public object
        {
        public:
            explicit lexer(const string &id, const  string &root_id);
            explicit lexer(const char   *id, const  char   *root_id);
            virtual ~lexer() throw();
            const string name;
            int          line;

            void restart() throw();

            lexical::scanner & declare(const string &);


        private:
            typedef set<string,lexical::scanner::ptr> scanner_db;
            typedef addr_list<lexical::scanner>       history_type;

            lexical::scanner     *curr; //!< current scanner
            history_type          hist; //!< history for sub-scanners
            lexical::scanner::ptr base; //!< root scanner
            scanner_db            scdb; //!< database

            YOCTO_DISABLE_COPY_AND_ASSIGN(lexer);
            void setup();

        public:
            lexical::scanner &root;
            p_dict            dict;

        };
    }
}

#endif
