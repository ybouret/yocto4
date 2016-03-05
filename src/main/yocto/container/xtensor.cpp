#include "yocto/container/xtensor.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/code/round.hpp"

#include <cstring>

namespace yocto
{
    xtensor:: row:: row(uint8_t *ptr, const size_t nc,const size_t blk) throw() :
    cols(nc),
    block_size(blk),
    addr(ptr)
    {
        assert(nc>0);
        assert(block_size>0);
        assert(addr!=NULL);
    }

    xtensor:: ~xtensor() throw()
    {
        memory::kind<memory::global>::release(data, dlen);
    }

    xtensor:: xtensor(const size_t nr, const size_t nc, const size_t blk) :
    rows( max_of<size_t>(nr,1) ),
    cols( max_of<size_t>(nc,1) ),
    block_size(max_of<size_t>(blk,1) ),
    pitch( cols * block_size ),
    pRow(0),
    rows_size( memory::align(sizeof(row)*rows) ),
    data_size( memory::align(rows*pitch)       ),
    dlen(rows_size+data_size),
    data( memory::kind<memory::global>::acquire(dlen) ),
    entry(0)
    {
        // set rows address
        uint8_t *p = static_cast<uint8_t *>(data);
        pRow = (row *) &p[0];

        // set data address
        p += rows_size;
        entry = p;
        // build rows
        row *R = pRow;
        for(size_t r=0;r<rows;++r,++R,p+=pitch)
        {
            new (R) row(p,cols,block_size);
        }

    }


    void xtensor:: ldz() throw()
    {
        memset(entry,0,data_size);
    }
    

}
