#ifndef YOCTO_SEEM_EVALUATOR_INCLUDED
#define YOCTO_SEEM_EVALUATOR_INCLUDED 1

#include "yocto/seem/compiler.hpp"
#include "yocto/lingua/syntax/analyzer.hpp"

namespace yocto
{
    namespace Seem
    {

        //! Generic Function Interface
        typedef functor<double,TL1(const array<double>&)> Function;

        //! Evaluator
        class Evaluator : public Compiler
        {
        public:
            typedef double (*CFunction)(double);
            typedef double (*CFunction2)(double,double);

            explicit Evaluator();
            virtual ~Evaluator() throw();

            double eval(const vCode &vcode);

            


            void SetVariable(const string &name, const double value);
            void SetVariable(const char   *name, const double value);

            void SetFunction(const string &name, const Function &fn);
            void SetFunction(const char   *name, const Function &fn);

            void SetCFunction(const char  *name, CFunction proc );
            void SetCFunction2(const char *name, CFunction2 proc );


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Evaluator);
            class VirtualMachine;
            auto_ptr<VirtualMachine>  vm;
        };

    }

}



#endif
