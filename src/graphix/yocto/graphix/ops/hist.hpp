#ifndef YOCTO_GRAPHIX_OPS_INCLUDED
#define YOCTO_GRAPHIX_OPS_INCLUDED 1

#include "yocto/graphix/rawpix.hpp"
#include "yocto/graphix/parallel.hpp"
#include "yocto/string.hpp"

namespace yocto
{
    namespace graphix
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
            static inline void __update(word_type *arr, const pixmap<T> &pxm, const graphix::patch &area) throw()
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


            //! a patch for histogram update
            class patch : public graphix::patch
            {
            public:
                const void *src;
                word_type   count[bins];

                explicit patch(const graphix::patch &p);
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
                               const graphix::patch &surface,
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


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(histogram);
        };
        
        
    }
}

#endif
