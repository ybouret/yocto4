#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/graphix/bitmap.hpp"
#include "yocto/graphix/rgb.hpp"
//#include "yocto/gfx/imgio.hpp"

#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/string.hpp"


namespace yocto
{
    namespace graphix
    {

        class image : public singleton<image>
        {
        public:
            typedef void   (*put_rgba_proc)(void *addr, const RGBA &C, void *args);
            typedef RGBA   (*get_rgba_proc)(const void *, void *);


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
                                      void                 *args,
                                      const void           *options) const = 0;

                virtual void     save(const string        &filename,
                                      const bitmap        &bmp,
                                      image::get_rgba_proc proc,
                                      void                *args,
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
                         void                 *args,
                         const void           *options) const;
            
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
