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

namespace yocto
{
    namespace graphics
    {
        void imageIO::save(const string &filename, const pixmap4 &bmp,const char *options) const
        {
            get_rgba proc;
            save(filename,bmp,proc,options);
        }


        void imageIO::save(const string &filename, const pixmap3 &bmp,const char *options) const
        {
            get_rgb proc;
            save(filename,bmp,proc,options);
        }

        void imageIO::save(const string &filename, const pixmap1 &bmp,const char *options) const
        {
            get_gsu proc;
            save(filename,bmp,proc,options);
        }
        
        void imageIO::save(const string &filename, const pixmapf &bmp,const char *options) const
        {
            get_gsf proc;
            save(filename,bmp,proc,options);
        }
        
    }
    
}


