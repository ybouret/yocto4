#ifndef YOCTO_SEEM_EVALUATOR_INCLUDED
#define YOCTO_SEEM_EVALUATOR_INCLUDED 1

#include "yocto/seem/types.hpp"
#include "yocto/lingua/syntax/analyzer.hpp"

namespace yocto
{
    namespace Seem
    {

        class Evaluator
        {
        public:
            explicit Evaluator();
            virtual ~Evaluator() throw();

            double run(ios::istream &fp);


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Evaluator);
            class VirtualMachine;
            Parser                    parser;
            auto_ptr<VirtualMachine>  vm;

            //void on_term( const string &label, const string &content );
            //void on_rule( const string &label, const size_t  ns      );
        };

    }

}



#endif
