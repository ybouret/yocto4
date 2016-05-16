#ifndef YOCTO_TIFFPP_INCLUDED
#define YOCTO_TIFFPP_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
    namespace graphics
    {
        class _TIFF
        {
        public:
            virtual ~ _TIFF() throw();
            
            class Raster
            {
            public:
                uint32_t    *data;
                const size_t size;
                Raster() throw();
                ~Raster() throw();
                void cleanup() throw();
                void startup(const size_t n);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Raster);
            };

            
        protected:
            explicit _TIFF() throw();
            void *handle;
            YOCTO_DISABLE_COPY_AND_ASSIGN(_TIFF);
        };
        
        class I_TIFF : public _TIFF
        {
        public:
          
                
            explicit I_TIFF(const string &filename);
            virtual ~I_TIFF() throw();

            bool   ReadDirectory();
            size_t CountDirectories();
            void   SetDirectory(const size_t n);
            int    GetWidth();
            int    GetHeight();

            
            void ReadRBGAImage(Raster &raster);
            
        private:            
            YOCTO_DISABLE_COPY_AND_ASSIGN(I_TIFF);
        };
        
        class O_TIFF : public _TIFF
        {
        public:
            static const size_t samples_per_pixel = 4;
            virtual ~O_TIFF() throw();
            explicit O_TIFF(const string &filename);
            
            void SetCompression(const char *);
            
            
            struct CompressionType
            {
                const char *name;
                unsigned    ttag;
            };
            
            static const CompressionType NamedCompression[];
            
            void WriteRGBAImage(const Raster &raster, const int w, const int h);
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(O_TIFF);
        };
    }
}

#endif
