#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/pixmap.hpp"
#include "yocto/gfx/surface.hpp"

#include "yocto/threading/singleton.hpp"

#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace gfx
    {
        
        typedef pixmap<rgb_t> bitmap32;
        
        //! image formats handling
        class image : public singleton<image>
        {
        public:
            class format : public object, public counted
            {
            public:
                typedef intr_ptr<string,format>     pointer;
                typedef set<string,format::pointer> database;
                
                const string name;
                
                virtual ~format() throw();
                const string &key() const throw();
                
                //______________________________________________________________
                //
                // Saving
                //______________________________________________________________
                virtual void save(const string &filename,
                                  const bitmap &bmp,
                                  addr2rgba    &proc,
                                  const char   *options) const = 0;
                
                void save_surface(const string  &filename,
                                  const surface &surf,
                                  const char    *options) const;
                
                //______________________________________________________________
                //
                // Loading
                //______________________________________________________________
                virtual surface *load(const string      &filename,
                                      const pixel_format fmt) const =0;
                
                //______________________________________________________________
                //
                // some info
                //______________________________________________________________
                virtual bool lossless() const throw() = 0;
                
            protected:
                explicit format( const char *id );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(format);
            };
            
            
            //! register a new format
            /**
             \param format a raw pointer, taken care of in case of error
             */
            void operator()( format *fmt );
            
            bool has( const string &id) const throw();
            bool has( const char   *id) const;
            
            const format & operator[](const string &id) const;
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(image);
            
            //__________________________________________________________________
            //
            // singleton API
            //__________________________________________________________________
            virtual ~image() throw();
            explicit image();
            friend class singleton<image>;
            static const char name[];
			static const threading::longevity life_time = 100; // TODO: check
            
            //__________________________________________________________________
            //
            // format mgmt
            //__________________________________________________________________
            format::database formats;
            
        public:
            typedef format::database::const_iterator iterator;
            
            size_t   size()  const throw();
            iterator begin() const throw();
            iterator end()   const throw();
        };
        
    }
}

#endif
