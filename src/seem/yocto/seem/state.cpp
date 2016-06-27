#include "yocto/seem/state.hpp"

namespace yocto
{
    namespace Seem
    {
        Atom::~Atom() throw()
        {
            switch(type)
            {
                case ID:
                    delete id;
                    break;

                case ARGS:
                    delete args;
                    break;

                default:
                    break;
            }
        }

        Atom:: Atom(const double &v) throw() :
        type(NUMBER)
        {
            value = v;
        }

        Atom:: Atom(const OpCode &op) throw() :
        type(OP)
        {
            opcode = op;
        }

        Atom:: Atom(const string &s) :
        type(ID)
        {
            id = new string(s);
        }

        Atom:: Atom() :
        type(ARGS)
        {
            args = new Args();
        }
    }
}
