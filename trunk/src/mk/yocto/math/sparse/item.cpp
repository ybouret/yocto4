#include "yocto/math/sparse/item.hpp"
#include "yocto/code/endian.hpp"

namespace yocto
{
    namespace math
    {
        
        sp_key::  sp_key(size_t r, size_t c) throw() : row(r), col(c) {}
        sp_key:: ~sp_key() throw() {}
        sp_key::  sp_key(const sp_key &k) throw() : row(k.row), col(k.col) {}
        
        sp_key::hasher:: hasher() throw() {}
        sp_key::hasher::~hasher() throw() {}
        size_t sp_key::hasher::operator()(const sp_key &k) const throw()
        {
            return swap_le_as<size_t>(k.row) | swap_be_as<size_t>(k.col);
        }
        
    }
}

