#ifndef YOCTO_SMOOTHING_INCLUDED
#define YOCTO_SMOOTHING_INCLUDED 1

#include "yocto/sequence/list.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/sequence/array.hpp"
#include "yocto/sequence/vector.hpp"

namespace yocto
{
    namespace math
    {

        template <typename T>
        class smoothing
        {
        public:
            typedef v2d<T> vtx_t;
            virtual ~smoothing() throw();
            explicit smoothing() throw();

            T f;      //!< for each point
            T b;      //!< for each point
            T degree; //!< max degree
            vector<vtx_t> v;
            size_t        nf; //!< added  front
            size_t        nb; //!< added  back

            void reset() throw();
            void build(const array<T> &X,
                       const array<T> &Y);


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(smoothing);
        };
    }
}


#endif
