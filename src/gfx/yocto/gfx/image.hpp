#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/image-io.hpp"

#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/string.hpp"


namespace yocto
{
    namespace gfx
    {



        class image : public singleton<image>, public imageIO
        {
        public:

            //! image format
            class format : public counted_object, public imageIO
            {
            public:
                typedef intr_ptr<string,format> pointer;
                typedef set<string,pointer>     db;

                const string name;

                virtual ~format() throw();
                const string & key() const throw();

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual bool         lossless() const throw() = 0;
                virtual const char **extensions() const throw() = 0;

            protected:
                explicit format(const char *id);

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(format);
            };

            const image::format &declare( format *fmt );

            const format & operator[](const string &) const;
            const format & operator[](const char   *) const;

            //! find someone for the extension
            virtual bitmap *load(const string         &path,
                                 unit_t                depth,
                                 rgba2data            &proc,
                                 const void           *options) const;

            //! find someone for the extension
            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const;
        private:
            explicit image();
            virtual ~image() throw();
            static const char                 name[];
            static const threading::longevity life_time = 100;
            format::db   formats;
            friend class singleton<image>;
            const format & get_format_for( const string &path ) const;
        };

#define YOCTO_GFX_DECL_FORMAT(NAME) image::instance().declare( new NAME##_format() )
    }
}

#endif
