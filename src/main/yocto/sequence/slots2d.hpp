#ifndef YOCTO_SLOTD2D_INCLUDED
#define YOCTO_SLOTS2D_INCLUDED 1


#include "yocto/container/cslot.hpp"
#include "yocto/type/args.hpp"

#include <iostream>

namespace yocto
{

    class slots2D
    {
    public:
        const size_t rows;
        const size_t cols;
        const size_t items;

        virtual ~slots2D() throw();

        size_t bytes() const throw();

    protected:
        const size_t shift; // where data may be stored
        cslot        cmem;  // data
        explicit slots2D(const size_t nr,
                         const size_t nc,
                         const size_t item_size,
                         const size_t row_bytes);

    private:
        YOCTO_DISABLE_ASSIGN(slots2D);
    };

    template <typename T>
    class slots2D_of : public slots2D
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        //! precomputed row
        class row
        {
        public:
            inline row(mutable_type *p, const size_t nc) throw() :
            base(p),
            cols(nc)
            {
                assert(base!=NULL);
                assert(cols>0);
            }

            inline type & operator[](const size_t ic) throw()
            {
                assert(ic<cols);
                return base[ic];
            }

            inline const_type & operator[](const size_t ic) const throw()
            {
                assert(ic<cols);
                return base[ic];
            }

        private:
            mutable_type *base;
            const size_t  cols;
            ~row() throw();
            YOCTO_DISABLE_COPY_AND_ASSIGN(row);
        };

        //! access rows
        inline row & operator[](const size_t ir)  throw()
        {
            assert(ir<rows);
            return Rows[ir];
        }

        //! access const rows
        inline const row & operator[](const size_t ir)  const throw()
        {
            assert(ir<rows);
            return Rows[ir];
        }

        inline type & operator()(const size_t indx) throw()
        {
            assert(indx<items);
            return Data[indx];
        }

        inline const_type & operator()(const size_t indx) const throw()
        {
            assert(indx<items);
            return Data[indx];
        }

        inline slots2D_of(const size_t nr, const size_t nc) :
        slots2D(nr,nc,sizeof(T),sizeof(row)),
        Rows(0),
        Data(0),
        constructed(0)
        {
            setup();
        }

        inline virtual ~slots2D_of() throw()
        {
            free();
        }


        //! cleanup all slots
        inline void free() throw()
        {
            while(constructed>0)
            {
                destruct(&Data[--constructed]);
            }
        }

#define YOCTO_SLOTS2D_BUILD_WITH(PROC) \
do { free(); try { PROC; } catch(...) { free(); throw; } } while(false)
        //! no args ctor
        inline void build()
        {
            YOCTO_SLOTS2D_BUILD_WITH( (ctor0()) );
        }

        template <typename U>
        inline void build_with( typename type_traits<U>::parameter_type arg1 )
        {
            YOCTO_SLOTS2D_BUILD_WITH( (ctor1<U>(arg1)) );
        }

        template <typename U,typename V>
        inline void build_with(typename type_traits<U>::parameter_type arg1,
                               typename type_traits<V>::parameter_type arg2)
        {
            YOCTO_SLOTS2D_BUILD_WITH( (ctor2<U,V>(arg1,arg2)) );
        }


        inline friend std::ostream & operator<<( std::ostream &os, const slots2D_of &s )
        {
            os << '[';
            for(size_t i=0;i<s.rows;++i)
            {
                for(size_t j=0;j<s.cols;++j)
                {
                    os << ' ' << s[i][j];
                }
                if(i<s.rows-1) os << ';';
            }
            os << ']';
            return os;
        }

    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(slots2D_of);
        row          *Rows;
        mutable_type *Data;
        size_t        constructed; //!< auxiliary build index

        inline void setup() throw()
        {
            // link level 1
            {
                uint8_t *p = (uint8_t *)(cmem.data);
                Rows = (row *)          &p[0];
                Data = (mutable_type *) &p[shift];
            }

            // no throw link level-2
            {
                mutable_type *q = Data;
                for(size_t i=0;i<rows;++i,q+=cols)
                {
                    new ( &Rows[i] ) row(q,cols);
                }
            }
        }


        inline void ctor0()
        {
            assert(constructed<=0);
            while(constructed<items)
            {
                new ( &Data[constructed] ) mutable_type();
                ++constructed;
            }
        }

        template <typename U>
        inline void ctor1(typename type_traits<U>::parameter_type arg1)
        {
            assert(constructed<=0);
            while(constructed<items)
            {
                new ( &Data[constructed] ) mutable_type(arg1);
                ++constructed;
            }
        }

        template <typename U,typename V>
        inline void ctor2(typename type_traits<U>::parameter_type arg1,
                          typename type_traits<V>::parameter_type arg2)
        {

        }
        
    };
}

#endif

