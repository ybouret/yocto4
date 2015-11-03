#ifndef YOCTO_GRAPHIX_OPS_INCLUDED
#define YOCTO_GRAPHIX_OPS_INCLUDED 1

#include "yocto/graphics/rawpix.hpp"
#include "yocto/graphics/parallel.hpp"
#include "yocto/string.hpp"
#include "yocto/code/bzset.hpp"
#include "yocto/container/tuple.hpp"



namespace yocto
{
    namespace graphics
    {

        
        //! one histogram of data
        class histogram : public object
        {
        public:
            static const size_t bins=256;
            typedef uint64_t    word_type;

            word_type count[256];

            explicit histogram() throw();
            virtual ~histogram() throw();
            
            //! for parallel code
            template <typename T>
            static inline void __update(word_type *arr, const pixmap<T> &pxm, const graphics::patch &area) throw()
            {
                assert(arr);
                assert(pxm.contains(area));
                for(unit_t j=area.lower.y;j<=area.upper.y;++j)
                {
                    const typename pixmap<T>::row &pj = pxm[j];
                    for(unit_t i=area.lower.x;i<=area.upper.x;++i)
                    {
                        const T &px = pj[i];
                        ++arr[ project<T>(px) ];
                    }
                }
            }

            //! sequential code
            template <typename T>
            inline void update(const pixmap<T> &pxm)
            {
                __update(count, pxm, pxm);
            }

            static void __reset(word_type *arr) throw();
            void reset() throw();                   //! everyting to 0
            void save(const string &hname) const;   //!< save to check
            void save(const char   *hname) const;

            //! a patch for histogram update
            class patch : public graphics::patch
            {
            public:
                const void *src;
                word_type   count[bins];

                explicit patch(const graphics::patch &p);
                virtual ~patch() throw();

                template <typename T>
                void compute(lockable &) throw()
                {
                    assert(src);
                    histogram::__reset(count);
                    const pixmap<T> &pxm = *static_cast<const pixmap<T>*>(src);
                    histogram::__update(count,pxm,*this);
                }


            private:
                YOCTO_DISABLE_ASSIGN(patch);
            };

            //! patches
            typedef vector<patch> patches;


            //! create the patches of a given area/servers
            /**
             \param hp computed patches
             \param surface the whole surface, as patch
             */
            static void create(patches              &hp,
                               const graphics::patch &surface,
                               threading::engine    *server);
            

            //! launch computation
            /**
             \param patches corresponding to sub areas
             \param pxm     the pixmap
             \param server  a server...
             */
            template <typename T>
            static inline void launch(patches           &patches,
                                     const pixmap<T>   &pxm,
                                     threading::engine *server)
            {
                if(server)
                {
                    for(size_t i=patches.size();i>0;--i)
                    {
                        patch &sub = patches[i];
                        sub.src    = &pxm;
                        server->enqueue( &patches[i], & patch::compute<T> );
                    }
                }
                else
                {
                    faked_lock access;
                    for(size_t i=patches.size();i>0;--i)
                    {
                        patch &sub = patches[i];
                        sub.src    = &pxm;
                        patches[i].compute<T>(access);
                    }
                }
            }

            //! finish computation, merge data into counts
            /**
             \param patches containing sub histograms
             \param server  needs to be flushed before computation
             */
            void finish(const patches &patches,threading::engine *server);


            //! Ostu threshold
            size_t threshold() const throw();
            
            typedef point2d<double> cbin;
            bool build_cdf(vector<cbin> &cdf, uint8_t *lut) const;
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram);
        };

        struct threshold
        {
            enum mode_type
            {
                keep_foreground,
                keep_background
            };

            template <typename T>
            inline static
            void apply(pixmap<T>      &target,
                       const size_t     t,
                       const pixmap<T> &source,
                       mode_type        m ) throw()
            {
                assert(source.w==target.w);
                assert(source.h==target.h);
                const unit_t w = source.w;
                const unit_t h = source.h;

                for(unit_t j=0;j<h;++j)
                {
                    const typename pixmap<T>::row &Sj = source[j];
                    typename       pixmap<T>::row &Tj = target[j];

                    for(unit_t i=0;i<w;++i)
                    {
                        const T       &src = Sj[i];
                        const size_t   lvl = project<T>(src);
                        T             &tgt = Tj[i];
                        switch(m)
                        {
                            case keep_foreground:
                                if(lvl<=t)
                                {
                                    bzset(tgt);
                                }
                                else
                                {
                                    tgt = src;
                                }
                                break;

                            case keep_background:
                                if(t<=lvl)
                                {
                                    bzset(tgt);
                                }
                                else
                                {
                                    tgt = invert_color(src);
                                }
                                break;
                        }
                    }
                }
                
            }
        };

        
    }
}

#endif
