#ifndef YOCTO_SEEM_STATE_INCLUDED
#define YOCTO_SEEM_STATE_INCLUDED 1

#include "yocto/seem/compiler.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/memory/pooled.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace Seem
    {

        //! variant class
        class Atom : public counted_object
        {
        public:
            enum Type
            {
                NUMBER,
                ID,
                OP,
                ARGS
            };
            typedef arc_ptr<Atom> Ptr;
            typedef vector<Atom::Ptr,memory::pooled::allocator> Args;

            virtual ~Atom() throw();

            const Type type;

        private:
            explicit Atom(const double &v) throw();
            explicit Atom(const OpCode &op) throw();
            explicit Atom(const string &s);
            explicit Atom();
            
            YOCTO_DISABLE_ASSIGN(Atom);
            union
            {
                double            value;
                string           *id;
                OpCode            opcode;
                Args             *args;
            };
        };

        class State
        {
        public:
            explicit State() throw();
            virtual ~State() throw();

            

        private:

            YOCTO_DISABLE_COPY_AND_ASSIGN(State);
        };
    }
}

#endif
