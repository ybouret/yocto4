#include "image-io.hpp"

namespace yocto
{
    namespace gfx
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
    namespace gfx
    {
        void imageIO::save(const string &filename, const pixmap4 &bmp,const char *options) const
        {
            rgba2rgba proc;
            save(filename,bmp,proc,options);
        }


        void imageIO::save(const string &filename, const pixmap3 &bmp,const char *options) const
        {
            rgb2rgba proc;
            save(filename,bmp,proc,options);
        }

        void imageIO::save(const string &filename, const pixmap1 &bmp,const char *options) const
        {
            byte2rgba proc;
            save(filename,bmp,proc,options);
        }
        
        void imageIO::save(const string &filename, const pixmapf &bmp,const char *options) const
        {
            float2rgba proc;
            save(filename,bmp,proc,options);
        }

        void imageIO::save(const string &filename, const yuvmap &bmp,const char *options) const
        {
            yuv2rgba proc;
            save(filename,bmp,proc,options);
        }

    }
    
}


