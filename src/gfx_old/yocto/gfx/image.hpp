#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace gfx
    {
        
        class image : public singleton<image>
        {
        public:
            typedef void   (*put_rgba_proc)(void *addr, const rgba_t &C, const void *args);
            typedef rgba_t (*get_rgba_proc)(const void *,const void *);
            
            
            //! image format
            class format : public counted_object
            {
            public:
                const string name;
                
                virtual ~format() throw();
                
                virtual bool     lossless() const throw() = 0;
                virtual bitmap  *load(const string         &filename,
                                      unit_t                depth,
                                      image::put_rgba_proc  proc,
                                      const void           *args,
                                      const void           *options) const = 0;
                
                virtual void     save(const string        &filename,
                                      const bitmap        &bmp,
                                      image::get_rgba_proc proc,
                                      const void          *args,
                                      const void          *options) const = 0;
                
                virtual const char **extensions() const throw() = 0;
                
                
                const string & key() const throw();
                
                typedef intr_ptr<string,format> pointer;
                typedef set<string,pointer>     db;
                
            protected:
                explicit format(const char *id);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(format);
            };
            
            void declare( format *fmt );
            
            const format & operator[](const string &) const;
            const format & operator[](const char   *) const;

            // find someone for the extension
            bitmap *load(const string         &path,
                         unit_t                depth,
                         image::put_rgba_proc  proc,
                         const void           *args,
                         const void           *options) const;
            
            //! pixmap if based on rgba_t
            static rgba_t get_rgba_dup(const void *,const void*) throw();

            //! pixmap if based on rgb_t
            static rgba_t get_rgb_dup(const void *,const void*) throw();

            //! pixmap is based on float, return greyscale
            static rgba_t get_gsf(const void *,const void*) throw();

            //! pixmpa is based on float, return ramp
            static rgba_t get_rampf(const void *,const void *) throw();

            //! pixmap is based on rgba_t
            static void   put_rgba_dup(void *addr, const rgba_t &C, const void *) throw();

            //! pixmap is based on rgb_t
            static void   put_rgb_dup(void *addr, const rgba_t &C, const void *) throw();
            
        private:
            explicit image();
            virtual ~image() throw();
            static const char                 name[];
            static const threading::longevity life_time = 100;
            format::db   formats;
            friend class singleton<image>;
            const format & get_format_for( const string &path ) const;
        };
    }
}

#endif