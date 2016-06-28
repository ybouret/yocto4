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
            typedef arc_ptr<Evaluator>  Pointer;

            explicit Evaluator();
            virtual ~Evaluator() throw();

            double eval(const vCode &vcode);

            inline double run(ios::istream &fp)
            {
                const vCode vcode = compile(fp);
                return eval(vcode);
            }

            inline double run(const string &fp)
            {
                const vCode vcode = compile(fp);
                return eval(vcode);
            }

            inline double run(const char *fp)
            {
                const vCode vcode = compile(fp);
                return eval(vcode);
            }


            void SetVariable(const string &name, const double value);
            void SetVariable(const char   *name, const double value);

            void SetFunction(const string &name, const Function &fn);
            void SetFunction(const char   *name, const Function &fn);

            void SetCFunction(const char  *name, CFunction proc );
            void SetCFunction2(const char *name, CFunction2 proc );


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(Evaluator);
            class VirtualMachine;
            VirtualMachine *vm;

        public:

            class Proxy
            {
            public:
                const string       vname;
                Evaluator::Pointer vexec;
                const vCode        vcode;

                ~Proxy() throw() {}
                Proxy(const string             &user_vname,
                      const Evaluator::Pointer &user_vexec,
                      const vCode              &user_vcode) :
                vname(user_vname),
                vexec(user_vexec),
                vcode(user_vcode)
                {
                }

                Proxy(const Proxy &other) :
                vname(other.vname),
                vexec(other.vexec),
                vcode(other.vcode)
                {
                }

                inline double operator()(const double value)
                {
                    vexec->SetVariable(vname,value);
                    return vexec->eval(vcode);
                }


            private:
                YOCTO_DISABLE_ASSIGN(Proxy);
            };
        };



    }

}



#endif
