#include "yocto/fame/linear.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    namespace fame
    {
        
        void *       linear_space:: data( size_t offset )      throw()
        {
            assert(offset < bytes/itmsz );
            return static_cast<uint8_t *>(buffer) + offset*itmsz;
        }
        
        const void * linear_space:: data( size_t offset) const throw()
        {
            assert(offset < bytes/itmsz );
            return static_cast<const uint8_t *>(buffer) + offset*itmsz;
        }

        
 
        linear_space:: ~linear_space() throw()
        {
            if(buflen)
            {
                memory::kind<memory_kind>::release(buffer, buflen);
            }
            else
            {
                buflen = 0;
            }
            (size_t&)bytes =0;
        }
        
        linear_space:: linear_space(const string &user_name,
                                    const size_t  num_bytes,
                                    const size_t  item_size,
                                    void *        user_data) :
        name(user_name),
        bytes(num_bytes),
        itmsz(item_size),
        buflen(0),
        buffer(0)
        {
            if(num_bytes<=0)     throw exception("fovea::linear_space(invalid #bytes)");
            if(0!=(bytes%itmsz)) throw exception("fovea::linear_space(bytes mismatch sizeof(T))");
            if( user_data )
            {
                buffer = user_data;
            }
            else
            {
                buflen = bytes;
                buffer = memory::kind<memory_kind>::acquire(buflen);
            }
        }
        
        
        
        const string & linear_space:: key() const throw()
        {
            return name;
        }
        
        void linear_space:: check_typeid(const std::type_info &aid) const
        {
            if( aid != get_typeid() )
                throw exception("wrong type for array '%s'", name.c_str());
        }
        
        string linear_space:: make_id( unit_t idx ) const
        {
            return name + vformat("[%d]", int(idx) );
        }
        
    }
}
