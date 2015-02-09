#ifndef YOCTO_GEMS_ATOM_INCLUDED
#define YOCTO_GEMS_ATOM_INCLUDED 1

#include "yocto/gems/properties.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/math/v3d.hpp"

namespace yocto
{
    namespace gems
    {
        class library;
        using namespace math;

        //! basic class for library to handle
        class atom_info : public counted_object
        {
        public:
            const word_t      uuid;
            const properties &ppty;

            virtual ~atom_info() throw();

            typedef intr_ptr<word_t,atom_info> tag;

            const word_t & key() const throw();
            
        protected:
            explicit atom_info(const word_t u, const properties &p ) throw();
            

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(atom_info);
        };


        template <typename T>
        class atom : public atom_info
        {
        public:
            inline virtual ~atom() throw() {}

            const T               mass;
            const T               invm;
            v3d<T>                r;

            typedef intr_ptr<word_t,atom> pointer;

        private:
            friend class library;

            inline atom(const word_t u, const properties &p) throw() :
            atom_info(u,p),
            mass(p.mass),
            invm(T(1)/mass),
            r()
            {
            }

            
            YOCTO_DISABLE_COPY_AND_ASSIGN(atom);
        };
    }
}

#endif
