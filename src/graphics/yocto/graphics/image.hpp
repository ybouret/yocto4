#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"
#include "yocto/graphics/rgba2data.hpp"
#include "yocto/graphics/data2rgba.hpp"

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

            //! image format
            class format : public counted_object
            {
            public:
                typedef intr_ptr<string,format> pointer;
                typedef set<string,pointer>     db;

                const string name;

                virtual ~format() throw();

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual bool     lossless() const throw() = 0;
                virtual bitmap  *load(const string         &filename,
                                      unit_t                depth,
                                      rgba2data            &proc,
                                      const void           *options) const = 0;

                virtual void     save(const string        &filename,
                                      const bitmap        &bmp,
                                      data2rgba           &proc,
                                      const void          *options) const = 0;

                virtual const char **extensions() const throw() = 0;
               
                const string & key() const throw();

               
                //______________________________________________________________
                //
                // Non virtual interface
                //______________________________________________________________
                void save(const string &filename, const pixmap4 &bmp, const char *options) const;
                void save(const string &filename, const pixmap3 &bmp, const char *options) const;
                void save(const string &filename, const pixmap1 &bmp, const char *options) const;
                void save(const string &filename, const pixmapf &bmp, const char *options) const;

                
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
                         rgba2data            &proc,
                         const void           *options) const;
            
            
            bitmap *load4(const string &path, const void *options) const;
            bitmap *load3(const string &path, const void *options) const;
            bitmap *loadf(const string &path, const void *options) const;
            bitmap *load1(const string &path, const void *options) const;

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
