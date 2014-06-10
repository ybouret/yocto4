#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/surface.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/lexicon.hpp"

namespace yocto
{
    
    namespace gfx
    {
        
        
        class image : public singleton<image>
        {
        public:
            typedef void   (*put_rgba_proc)(void *,const rgba_t &,const void *);
            typedef rgba_t (*get_rgba_proc)(const void *,const void *);
            
            class format : public counted_object
            {
            public:
                virtual ~format() throw();
                
                const string name;
                
                const string &key() const throw();
                
                typedef intr_ptr<string,format>         pointer;
                typedef lexicon<string,format::pointer> database;
                
                
                //______________________________________________________________
                //
                // virtual API
                //______________________________________________________________

                virtual bool     lossless() const throw() = 0;
                virtual bitmap  *load(const string         &filename,
                                      unit_t                depth,
                                      image::put_rgba_proc  proc,
                                      const void           *args) const = 0;
                
                virtual void     save(const string        &filename,
                                      const bitmap        &bmp,
                                      image::get_rgba_proc proc,
                                      const void          *args,
                                      const char          *options) const = 0;
                
                
                //______________________________________________________________
                //
                // non virtual API
                //______________________________________________________________
                surface *             load_surface(const string &filename, const pixel_format fmt ) const;
                const bitmap::pointer load_bitmap3(const string &filename) const;     //!< for pixmap<rgb_t>
                const bitmap::pointer load_bitmap4(const string &filename) const;     //!< for pixmap<rgba_t>
                const bitmap::pointer load_greyscale(const string &filename) const;   //!< for pixmap<uin8t_t>
                const bitmap::pointer load_greyscale_f(const string &filename) const; //!< for pixmap<float>
                
              
                
                void save_surface(const string &filename,
                                  const surface &surf,
                                  const char    *options) const;
                
                
            protected:
                explicit format(const char *id);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(format);
            };
            
            void declare( format *fmt );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(image);
            friend class singleton<image>;
            virtual ~image() throw();
            explicit image();
            static const threading::longevity life_time = 100;
            static const char                 name[];
            format::database db;
        };
    }
    
}

#endif
