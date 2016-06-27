#ifndef YOCTO_SEEM_COMPILER_INCLUDED 
#define YOCTO_SEEM_COMPILER_INCLUDED 1

#include "yocto/seem/types.hpp"
#include "yocto/lingua/syntax/analyzer.hpp"

namespace yocto
{
    namespace Seem
    {
        typedef lingua::syntax::analyzer Analyzer;

        class Compiler
        {
        public:
            
            virtual ~Compiler() throw();
            explicit Compiler(const bool emitFiles);

            Parser     parser;
            Analyzer   walker;

            void run( ios::ostream &bin, ios::istream &fp );
            void run( string       &bin, ios::istream &fp );

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Compiler);
            ios::ostream *out;
            void on_term( const string &label, const string &content );
            void on_rule( const string &label, const size_t  ns      );
        };

    }
}

#endif
