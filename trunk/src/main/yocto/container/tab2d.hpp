#ifndef YOCTO_TABLEAU2D_INCLUDED
#define YOCTO_TABLEAU2D_INCLUDED 1

#include "yocto/container/tab1d.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    
    template <typename T>
    class Tableau2D : public ITableau
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        
        const size_t rmin;
        const size_t rmax;
        const size_t rows;
        const size_t items;
        
        typedef TableauOf<T> Row;
        
        
        Row & operator[](size_t j) throw()
        {
            assert(j>=rmin);
            assert(j<=rmax);
            return row[j];
        }
        
        const Row & operator[](size_t j) const throw()
        {
            assert(j>=rmin);
            assert(j<=rmax);
            return row[j];
        }
        
        explicit Tableau2D(size_t   __jmin,
                           size_t   __jmax,
                           size_t   __imin,
                           size_t   __imax,
                           param_type args) :
        ITableau(__imin,__imax),
        rmin( min_of(__jmin,__jmax) ),
        rmax( max_of(__jmin,__jmax) ),
        rows( rmax+1-rmin ),
        items( this->cols*rows),
        row(0),
        ptr(0),
        wlen(0),
        wksp(0)
        {
            const size_t row_offset = 0;
            const size_t row_length = sizeof(Row) * rows;
            
            const size_t itm_offset = memory::align(row_offset+row_length);
            const size_t itm_length = items * sizeof(T);
            
            wlen = memory::align(itm_offset+itm_length);
            wksp = memory::kind<memory::global>::acquire(wlen);
            
            uint8_t *p = (uint8_t*)wksp;
            row = (Row *)          &p[row_offset];
            ptr = (mutable_type *) &p[itm_offset];
            
            // copy or args
            size_t i=0;
            try
            {
                while(i<items)
                {
                    new (ptr+i) mutable_type(args);
                    ++i;
                }
            }
            catch(...)
            {
                for(size_t j=0;j<i;++j)
                {
                    ptr[j].~mutable_type();
                }
                kill();
                throw;
            }
            
            // build the rows, no throw
            size_t c = 0;
            for(size_t r=0;r<rows;++r,c+=cols)
            {
                Row *rr = new (row+r) Row(this->cmin,this->cmax);
                rr->attach( &ptr[c] );
            }
            row -= rmin;
        }
        
        
        virtual ~Tableau2D() throw()
        {
            for(size_t i=0;i<items;++i)
            {
                ptr[i].~mutable_type();
            }
            kill();
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Tableau2D);
        Row          *row;
        mutable_type *ptr;
        size_t wlen;
        void  *wksp;
        void kill() throw() { memory::kind<memory::global>::release(wksp, wlen); }
    };
    
    
}
#endif
