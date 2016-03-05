#ifndef YOCTO_XTENSOR_INCLUDED
#define YOCTO_XTENSOR_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{

    // 0 offset xtensor
    class xtensor
    {
    public:
        
        class row
        {
        public:
            explicit row(uint8_t *ptr, const size_t nc, const size_t blk) throw();
            const size_t cols;
            const size_t block_size;

            inline   void * operator[](const size_t ic) throw()
            {
                assert(ic<cols);
                return &addr[ic*block_size];
            }

            inline   const void * operator[](const size_t ic) const throw()
            {
                assert(ic<cols);
                return &addr[ic*block_size];
            }

        private:
            uint8_t *addr;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(row);
            ~row() throw();
        };

        const size_t rows;       //!< #rows
        const size_t cols;       //!< #cols
        const size_t block_size; //!< at each row,col
        const size_t pitch;      //!< cols * block_size;
        explicit xtensor(const size_t nr, const size_t nc, const size_t blk);
        virtual ~xtensor() throw();

        inline size_t allocated() const throw() { return dlen; }
        inline row & operator[](const size_t ir) throw()
        {
            assert(ir<rows);
            return pRow[ir];
        }

        inline const row & operator[](const size_t ir) const throw()
        {
            assert(ir<rows);
            return pRow[ir];
        }

        void ldz() throw();

    protected:
        row         *pRow;
    public:
        const size_t rows_size;
        const size_t data_size;
    protected:
        size_t dlen;
        void  *data;
        void  *entry;
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(xtensor);
    };

}

#endif

