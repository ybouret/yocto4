#ifndef YOCTO__MATRIX_INCLUDED
#define YOCTO__MATRIX_INCLUDED 1

#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/bitwise.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/unroll.hpp"
#include <cerrno>
#include <typeinfo>

namespace yocto
{
    namespace math
    {

        enum matrix_memory
        {
            matrix_tight_memory,
            matrix_large_memory
        };

#define YOCTO_MATRIX_TRANSPOSE 0x0001
#define YOCTO_MATRIX_TIGHTEN   0x0100

#define YOCTO_MATRIX_NUM_EXTRA 2

#define YOCTO_MATRIX _matrix
#define YOCTO_MATRIX_CTOR(MEMORY_KIND) \
rows(0), \
cols(0), \
items(0),\
data(0),\
pRow(0),\
num_objects(0),\
ctor(0), \
num_scalars(0),\
stor(0), \
wksp(0), \
wlen(0),\
indices(0),\
scalars(0),\
memory_kind(MEMORY_KIND)

        //! row-major matrix
        /**
         square matrices may have extra memory
         */
        template <typename T>
        class YOCTO_MATRIX : public object
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef typename real_of<mutable_type>::type scalar_type;

            enum
            {
                is_primitive = type_traits<mutable_type>::is_primitive
            };

            //! a row, acting as an array
            class row : public array<T>
            {
            public:
                const size_t cols;
                inline virtual type       & operator[]( size_t c ) throw()       { assert(c>0); assert(c<=cols); return _itm[c]; }
                inline virtual const_type & operator[]( size_t c ) const throw() { assert(c>0); assert(c<=cols); return _itm[c]; }

                inline friend std::ostream & operator<<( std::ostream &os, const row &r )
                {
                    for( size_t j=1; j <= r.cols; ++j ) os << r[j] << ' ';
                    return os;
                }

                inline virtual const char *name() const throw() { return "matrix::row"; }
                inline virtual size_t size() const throw()      { return cols; }
                inline virtual size_t capacity() const throw()  { return cols; }
                inline virtual void   free() throw()            {}
                inline virtual void   release() throw()         {}
                inline virtual void   reserve(size_t)           {}


            private:
                inline row( T *data, size_t nc ) throw() : cols(nc), _itm(data-1) { assert(cols>0); assert(data!=NULL); }
                inline virtual ~row() throw() {}

                T *_itm; //! offset-1 pointer
                YOCTO_DISABLE_COPY_AND_ASSIGN(row);
                friend class YOCTO_MATRIX<T>;
                virtual const_type * get_item() const throw() { return _itm; }
            };


            const size_t rows;
            const size_t cols;
            const size_t items;

            inline bool   is_square() const throw() { return rows==cols; }

            inline explicit YOCTO_MATRIX() throw() :
            YOCTO_MATRIX_CTOR(matrix_tight_memory)
            {
                // do nothing
            }

#define YOCTO_MATRIX_BUILD_WITH(CODE) do { try { CODE; } catch(...) { release(); throw; } } while(false)

            inline explicit YOCTO_MATRIX(const size_t nr, const size_t nc) :
            YOCTO_MATRIX_CTOR(matrix_tight_memory)
            {
                build(nr,nc);
                YOCTO_MATRIX_BUILD_WITH(ctor0( int2type<is_primitive>() ) );
            }

            inline explicit YOCTO_MATRIX(const size_t n) :
            YOCTO_MATRIX_CTOR(matrix_large_memory)
            {
                build(n,n);
                YOCTO_MATRIX_BUILD_WITH(ctor0( int2type<is_primitive>() ) );
            }


            //! default copy constructor
            inline YOCTO_MATRIX(const YOCTO_MATRIX<T> &other) :
            YOCTO_MATRIX_CTOR(other.memory_kind)
            {
                build(other.rows,other.cols);
                YOCTO_MATRIX_BUILD_WITH(ctor1(other,int2type<is_primitive>() ) );
            }

            //! constructor with options
            inline YOCTO_MATRIX(const YOCTO_MATRIX<T> &other, const int flags) :
            YOCTO_MATRIX_CTOR( (0!=(flags&YOCTO_MATRIX_TIGHTEN)) ? matrix_tight_memory : other.memory_kind )
            {
                if(0!=(flags&YOCTO_MATRIX_TRANSPOSE))
                {
                    build(other.cols,other.rows);
                    YOCTO_MATRIX_BUILD_WITH(ctor1_transpose(other,int2type<is_primitive>() ) );
                }
                else
                {
                    build(other.rows,other.cols);
                    YOCTO_MATRIX_BUILD_WITH(ctor1(other,int2type<is_primitive>() ) );
                }
            }




            inline virtual ~YOCTO_MATRIX() throw()
            {
                release();
            }

            inline row       & operator[](const size_t ir) throw()       { assert(ir>0); assert(ir<=rows); return pRow[ir]; }
            inline const row & operator[](const size_t ir) const throw() { assert(ir>0); assert(ir<=rows); return pRow[ir]; }

            friend std::ostream & operator<<( std::ostream &os, const YOCTO_MATRIX<T> &M )
            {
                os << "[ ";
                if( M.rows > 0 )
                {
                    for( size_t i=1; i < M.rows; ++i ) os << M[i] << "; ";
                    os << M[ M.rows ];
                }
                os << "]";
                return os;
            }

#define YOCTO_MATRIX_XCH(FIELD) cswap_const(FIELD,M.FIELD)
            inline void swap_with( YOCTO_MATRIX<T> &M ) throw()
            {
                YOCTO_MATRIX_XCH(rows);
                YOCTO_MATRIX_XCH(cols);
                YOCTO_MATRIX_XCH(items);
                YOCTO_MATRIX_XCH(data);
                YOCTO_MATRIX_XCH(pRow);
                YOCTO_MATRIX_XCH(num_objects);
                cswap(ctor,M.ctor);
                YOCTO_MATRIX_XCH(num_scalars);
                cswap(stor,M.stor);
                YOCTO_MATRIX_XCH(wksp);
                YOCTO_MATRIX_XCH(wlen);
                YOCTO_MATRIX_XCH(indices);
                YOCTO_MATRIX_XCH(scalars);
                YOCTO_MATRIX_XCH(memory_kind);
            }

#define YOCTO_MATRIX_COPY(I) data[I] = other.data[I]
            //! assign if same sizes
            inline void assign( const YOCTO_MATRIX<T> &other )
            {
                assert(rows==other.rows);
                assert(cols==other.cols);
                YOCTO_LOOP_FUNC(items,YOCTO_MATRIX_COPY,0);
            }

            //! check sizes and assign
            YOCTO_MATRIX<T> & operator=( const YOCTO_MATRIX<T> &other )
            {
                if(this!=&other)
                {
                    if(rows==other.rows&&cols==other.cols)
                    {
                        assign(other);
                    }
                    else
                    {
                        YOCTO_MATRIX<T> tmp(other);
                        swap_with(tmp);
                    }
                }
                return *this;
            }

            inline size_t bytes() const throw() { return wlen; }

            inline void swap_rows(size_t r1, size_t r2) throw()
            {
                row &R1 = (*this)[r1];
                row &R2 = (*this)[r2];
                for(size_t c=cols;c>0;--c) bswap(R1[c],R2[c]);
            }

            inline void swap_cols(size_t c1, size_t c2) throw()
            {
                for(size_t r=rows;r>0;--r) bswap((*this)[r][c1],(*this)[r][c2]);
            }



        private:
            mutable_type *data;
            row          *pRow;
            const size_t  num_objects; //!< total number of types (items+other)
            size_t        ctor;        //!< number of constructed items
            const size_t  num_scalars; //!< total number of scalars
            size_t        stor;        //!< number of constructed scalars
            void         *wksp;
            size_t        wlen;
            size_t       *indices;
            scalar_type  *scalars;

            //! support no destruct
            inline void __clear( int2type<true> ) throw()
            {
            }

            //! must destruct
            inline void __clear( int2type<false> ) throw()
            {
                while(stor>0)
                {
                    destruct( &scalars[--stor] );
                }

                while(ctor>0)
                {
                    destruct( &data[--ctor] );
                }
            }

            inline void clear() throw()
            {
                __clear( int2type< support_no_destruct<mutable_type>::value >() );
            }

            //__________________________________________________________________
            //
            // release all memory
            //__________________________________________________________________
            void release() throw()
            {
                clear();
                memory::kind<memory::global>::release(wksp,wlen);
                data = 0;
                pRow = 0;
                (size_t&)rows  = 0;
                (size_t&)cols  = 0;
                (size_t&)items = 0;
                (size_t&)num_objects = 0;
                (size_t&)num_scalars = 0;
                indices = 0;
                scalars = 0;
            }


            //__________________________________________________________________
            //
            // build all memory
            //__________________________________________________________________
            void build(size_t nr, size_t nc)
            {
                if(nr<=0)
                {
                    if(nc>0) throw libc::exception( EINVAL, "matrix<%s>: no row for #colums=%u",  typeid(T).name(), unsigned(nc) );
                    return;
                }

                if(nc<=0)
                {
                    if(nr>0) throw libc::exception( EINVAL, "matrix<%s>: no column for #rows=%u", typeid(T).name(), unsigned(nr) );
                    return;
                }

                assert(nc>0);
                assert(nr>0);
                const bool is_large = (memory_kind == matrix_large_memory);

                //______________________________________________________________
                //
                // Preparing Memory Requirements
                //______________________________________________________________
                const size_t n           = nr*nc;
                const size_t nextra      = is_large ? YOCTO_MATRIX_NUM_EXTRA * nr : 0;
                const size_t n_objects   = n+nextra;

                const size_t data_offset = 0;
                const size_t data_length = n_objects * sizeof(T);

                const size_t rows_offset = memory::align(data_offset+data_length);
                const size_t rows_length = nr * sizeof(row);

                const size_t indx_offset = memory::align(rows_offset+rows_length);
                const size_t indx_length = is_large ? nr * sizeof(size_t) : 0;

                const size_t scal_offset = memory::align(indx_offset+indx_length);
                const size_t n_scalars   = is_large ? nr : 0;
                const size_t scal_length = n_scalars * sizeof(scalar_type);

                //______________________________________________________________
                //
                // Allocate Memory
                //______________________________________________________________
                wlen = memory::align(scal_offset+scal_length);
                wksp = memory::kind<memory::global>::acquire(wlen);


                //______________________________________________________________
                //
                // Link Memory
                //______________________________________________________________
                uint8_t *p = static_cast<uint8_t*>(wksp);
                data = (mutable_type *) &p[data_offset];
                pRow = (row          *) &p[rows_offset];
                --pRow;
                {
                    mutable_type *q = data;
                    for(size_t r=1;r<=nr;++r,q+=nc)
                    {
                        new (pRow+r) row(q,nc);
                    }
                }

                indices = (size_t      *) &p[indx_offset];
                scalars = (scalar_type *) &p[scal_offset];

                //______________________________________________________________
                //
                // finalizing
                //______________________________________________________________
                (size_t &)rows  = nr;
                (size_t &)cols  = nc;
                (size_t &)items = n;
                (size_t &)num_objects = n_objects;
                (size_t &)num_scalars = n_scalars;

            }

            //! primitive type
            inline void ctor0( int2type<true> ) throw()
            {
                assert(ctor<=0);
            }

            //! C++ type: initialize other values...
            inline void init0()
            {
                assert(stor<=0);
                while(ctor<num_objects)
                {
                    new (data+ctor) mutable_type();
                    ++ctor;
                }
                while(stor<num_scalars)
                {
                    new (scalars+stor) scalar_type();
                    ++stor;
                }
            }

            //! compound type
            inline void ctor0( int2type<false> )
            {
                assert(ctor<=0);
                init0();
            }


            //! primitive type
            inline void ctor1( const YOCTO_MATRIX<T> &other, int2type<true> ) throw()
            {
                assert(ctor<=0);
                assert(items==other.items);
                YOCTO_LOOP_FUNC(items,YOCTO_MATRIX_COPY,0);
            }

            //! C++ type
            inline void ctor1( const YOCTO_MATRIX<T> &other, int2type<false> ) throw()
            {
                assert(ctor<=0);
                assert(items==other.items);
                while(ctor<items)
                {
                    new (data+ctor) mutable_type(other.data[ctor]);
                    ++ctor;
                }
                init0();
            }

            //! copy transpose, primitive type
            inline void ctor1_transpose( const YOCTO_MATRIX<T> &other, int2type<true> ) throw()
            {
                assert(ctor<=0);
                assert(items==other.items);
                for(size_t i=1;i<=rows;++i)
                {
                    for(size_t j=1;j<=cols;++j)
                    {
                        data[ctor] = other[j][i];
                        ++ctor;
                    }
                }
            }

            //! copy transpose, C++ type
            inline void ctor1_transpose( const YOCTO_MATRIX<T> &other, int2type<false> ) throw()
            {
                assert(ctor<=0);
                assert(items==other.items);
                for(size_t i=1;i<=rows;++i)
                {
                    for(size_t j=1;j<=cols;++j)
                    {
                        new (&data[ctor]) mutable_type(other[j][i]);
                        ++ctor;
                    }
                }
                init0();
            }


        public:
            const matrix_memory memory_kind;
#if 0
            inline size_t       *__indices() const throw()
            {
                assert(memory_kind==matrix_large_memory);
                return (size_t *)indices;
            }

            inline mutable_type *__aux(const size_t k) const throw()
            {
                assert(memory_kind==matrix_large_memory);
                assert(k<YOCTO_MATRIX_NUM_EXTRA);
                const_type *addr = &data[items+k*rows];
                return (mutable_type *)addr;
            }

            inline scalar_type  *__scalars() const throw()
            {
                return (scalar_type *)scalars;
            }
#endif

        };
        
    }
}

#endif
