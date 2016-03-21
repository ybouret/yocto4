#ifndef YOCTO_PARALLEL_VTK_INCLUDED
#define YOCTO_PARALLEL_VTK_INCLUDED 1

#include "yocto/parallel/field.hpp"
#include "yocto/ios/ostream.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace parallel
    {
        struct vtk
        {
            static inline void header(ios::ostream &fp, const string &title)
            {
                fp << "# vtk DataFile Version 2.0\n";
                const size_t n = min_of<size_t>(title.size(),255);
                for(size_t i=0;i<n;++i)
                {
                    char C = title[i];
                    if(C<32) C='_';
                    fp.write(C);
                }
                fp << "\n";
                fp << "ASCII\n";
            }

            template <typename COORD>
            static inline void structured_points(ios::ostream          &fp,
                                                 const patch_of<COORD> &g)
            {
                fp << "DATASET STRUCTURED_POINTS\n";

                fp << "DIMENSIONS";
                for(size_t i=0;i<patch_of<COORD>::DIM;++i)
                {
                    fp(" %u", unsigned(__coord(g.width,i)) );
                }
                fp << "\n";

                fp << "ORIGIN";
                for(size_t i=0;i<patch_of<COORD>::DIM;++i)
                {
                    fp(" %d", int(__coord(g.lower,i)) );
                }
                fp << "\n";

                fp << "SPACING";
                for(size_t i=0;i<patch_of<COORD>::DIM;++i)
                {
                    fp << " 1";
                }
                fp << "\n";
            }

            template <typename T>
            static void __output_type(ios::ostream &fp);

            template <typename FIELD>
            static inline void scalars(ios::ostream &fp,
                                       const string &label,
                                       const FIELD  &F)
            {
                fp << "SCALARS " << label << ' ';
                __output_type<typename FIELD::type>(fp);
                fp << "\n";
                fp << "LOOKUP_TABLE default\n";
            }

            template <typename PATCH>
            static inline void save_patch(ios::ostream  &fp,
                                          const string  &title,
                                          const PATCH   &F)
            {
                header(fp,title);
                structured_points<typename PATCH::coord>(fp,F);
            }

        };

        template <>
        inline void vtk::__output_type<float>(ios::ostream &fp)
        {
            fp << "float 1";
        }

        template <>
        inline void vtk::__output_type<double>(ios::ostream &fp)
        {
            fp << "double 1";
        }
        
    }
}

#endif
