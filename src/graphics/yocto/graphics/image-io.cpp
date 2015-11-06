#include "image-io.hpp"

namespace yocto
{
    namespace graphics
    {
        imageIO:: ~imageIO() throw()
        {
        }

        imageIO:: imageIO() throw()
        {
        }

        bitmap * imageIO:: load4(const string &path, const void *options) const
        {
            put_rgba proc;
            return load(path,4,proc,options);
        }
        
        bitmap * imageIO:: load3(const string &path, const void *options) const
        {
            put_rgb proc;
            return load(path,3,proc,options);
        }

        bitmap * imageIO:: loadf(const string &path, const void *options) const
        {
            put_gsf proc;
            return load(path,4,proc,options);
        }


        bitmap * imageIO:: load1(const string &path, const void *options) const
        {
            put_gsu proc;
            return load(path,1,proc,options);
        }

    }
}
