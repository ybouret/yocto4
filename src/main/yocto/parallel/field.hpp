#ifndef YOCTO_PARALLEL_FIELD_INCLUDED
#define YOCTO_PARALLEL_FIELD_INCLUDED 1

#include "yocto/parallel/patch.hpp"
//#include "yocto/memory/global.hpp"
#include "yocto/object.hpp"

namespace yocto
{

    namespace parallel
    {

        //______________________________________________________________________
        //
        //
        //! Agnostic field info
        //
        //______________________________________________________________________
        class field_info : public object
        {
        public:
            virtual ~field_info() throw()
            {
            }

            virtual size_t dimensions()   const throw() = 0;
            const size_t owned_memory; //!< owned bytes

        protected:
            explicit field_info() throw() :
            owned_memory(0)
            {
            }


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(field_info);
        };

        //______________________________________________________________________
        //
        //
        //! base class: type+dimension
        //
        //______________________________________________________________________
        template <typename T, typename COORD>
        class field_of : public field_info, public patch_of<COORD>
        {
        public:
            YOCTO_ARGUMENTS_DECL_T;
            typedef patch_of<COORD>   patch_type;
            static const size_t DIM = patch_type::DIM;

            inline virtual ~field_of() throw()
            {

            }

            virtual size_t dimensions() const throw() { return DIM; }

            inline T & operator()(const COORD C) throw()
            {
                return *(T*)item_at(C);
            }

            inline const T & operator()(const COORD C) const throw()
            {
                return *(const T*)item_at(C);
            }

            inline T & get(const size_t i) throw()
            {
                assert(i<this->items);
                return * ( ( (T*)base_of() ) + i);
            }

            inline const T & get(const size_t i) const throw()
            {
                assert(i<this->items);
                return * ( ( (const T*)base_of() ) + i);
            }


        protected:
            explicit field_of( const patch_type &p ) :
            field_info(),
            patch_type(p)
            {

            }

            virtual const void *item_at(const COORD &C) const throw() = 0;
            virtual const void *base_of()               const throw() = 0;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(field_of);
        };


        //______________________________________________________________________
        //
        //
        //! 1D field
        //
        //______________________________________________________________________
        template <typename T>
        class field1D : public field_of<T,coord1D>
        {
        public:
            typedef  field_of<T,coord1D> field_type;

            //! create with own memory
            explicit field1D(const patch1D p) :
            field_type(p),
            count(this->items),
            entry( memory::kind<memory::global>::acquire_as<T>(count) ),
            data(entry)
            {
                try
                {
                    chunk_ops<T>::setup(entry,this->items);
                }
                catch (...)
                {
                    memory::kind<memory::global>::release_as<T>(entry,count);
                    throw;
                }
                entry -= this->lower;
                (size_t &)(this->owned_memory) = count * sizeof(T);
            }

            //! create with user's memory
            explicit field1D(const patch1D p, T *user_entry) :
            field_type(p),
            count(0),
            entry(user_entry-this->lower),
            data(user_entry)
            {
                assert(NULL!=user_entry);
                chunk_ops<T>::setup(user_entry,this->items);
            }

            //! destruct all, with control
            virtual ~field1D() throw()
            {
                entry += this->lower;
                chunk_ops<T>::clear(entry,this->items);
                if(count)
                {
                    memory::kind<memory::global>::release_as<T>(entry,count);
                }
                data = 0;
            }


            inline T & operator[](const unit_t x) throw()
            {
                assert(x>=this->lower);
                assert(x<=this->upper);
                return entry[x];
            }

            inline const T & operator[](const unit_t x) const throw()
            {
                assert(x>=this->lower);
                assert(x<=this->upper);
                return entry[x];
            }


        private:
            size_t      count;
            T          *entry;
            const void *data;

            YOCTO_DISABLE_COPY_AND_ASSIGN(field1D);
            virtual const void *item_at(const coord1D &C) const throw()
            {
                assert(this->has(C));
                return &entry[C];
            }

            virtual const void *base_of() const throw() { return data; }
        };

        //______________________________________________________________________
        //
        //
        //! 2D field
        //
        //______________________________________________________________________
        template <typename T>
        class field2D : public field_of<T,coord2D>
        {
        public:
            typedef  field_of<T,coord2D> field_type;
            typedef  field1D<T>          row_type;

            inline explicit field2D(const patch2D p) :
            field_type(p),
            rows(0),
            data(0),
            wlen(0),
            wksp(0)
            {
                const size_t nr          = size_t(this->width.y);
                const size_t rows_offset = 0;
                const size_t rows_length = nr * sizeof(row_type);
                const size_t data_offset = memory::align(rows_offset+rows_length);
                const size_t data_length = this->items * sizeof(T);

                wlen = memory::align(data_offset+data_length);
                wksp = memory::kind<memory::global>::acquire(wlen);

                uint8_t *q = static_cast<uint8_t *>(wksp);
                rows       = (row_type *)&q[rows_offset];
                data       = (T        *)&q[data_offset];

                //______________________________________________________________
                //
                // map rows
                //______________________________________________________________
                const size_t nc =size_t(this->width.x);
                try
                {
                    map_rows(nr,nc);
                }
                catch(...)
                {
                    clean_up();
                    throw;
                }

                (size_t &)(this->owned_memory) = wlen;

            }

            inline explicit field2D(const patch2D p, row_type *user_rows, T *user_data ) :
            field_type(p),
            rows(user_rows),
            data(user_data),
            wlen(0),
            wksp(0)
            {
                assert(0!=user_rows);
                assert(0!=user_data);
                const size_t nr         = size_t(this->width.y);
                const size_t nc         = size_t(this->width.x);
                map_rows(nr,nc);
            }


            inline virtual ~field2D() throw()
            {
                rows += this->lower.y;
                size_t nr = size_t(this->width.y);
                while(nr>0) { destruct<row_type>( &rows[--nr] ); }
                clean_up();
            }

            inline row_type & operator[](const unit_t y) throw()
            {
                assert(y>=this->lower.y);
                assert(y<=this->upper.y);
                return rows[y];
            }

            inline const row_type & operator[](const unit_t y) const throw()
            {
                assert(y>=this->lower.y);
                assert(y<=this->upper.y);
                return rows[y];
            }

        private:
            row_type *rows; //!< width.y * sizeof(row_type)
            T        *data; //!< items   * sizeof(T)
            size_t    wlen;
            void     *wksp;

            YOCTO_DISABLE_COPY_AND_ASSIGN(field2D);

            inline void clean_up() throw()
            {
                if(wlen)
                {
                    memory::kind<memory::global>::release(wksp, wlen);
                }
            }
            
            inline void map_rows(const size_t nr,const size_t nc)
            {
                size_t        ir = 0;
                const patch1D p1(this->lower.x,this->upper.x);
                T            *pr = data;
                try
                {
                    while(ir<nr)
                    {
                        new ( &rows[ir] ) row_type(p1,pr);
                        ++ir;
                        pr += nc;
                    }
                }
                catch(...)
                {
                    while(ir>0)
                    {
                        destruct<row_type>(&rows[--ir]);
                    }
                    throw;
                }
                rows -= this->lower.y;
            }

            virtual const void *item_at(const coord2D &C) const throw()
            {

                assert(this->has(C));
                return &rows[C.y][C.x];
            }

            virtual const void *base_of() const throw() { return data; }

        };


        //______________________________________________________________________
        //
        //
        //! 2D field
        //
        //______________________________________________________________________
        template <typename T>
        class field3D : public field_of<T,coord3D>
        {
        public:
            typedef  field_of<T,coord3D> field_type;
            typedef  field2D<T>          slice_type;
            typedef  field1D<T>          row_type;

            inline explicit field3D( const patch3D p ) :
            field_type(p),
            slices(0),
            data(0),
            wlen(0),
            wksp(0)
            {
                const size_t num_slices     = this->width.z;
                const size_t rows_per_slice = this->width.y;
                const size_t num_rows       = rows_per_slice * num_slices;

                const size_t slice_offset = 0;
                const size_t slice_length = num_slices * sizeof(slice_type);

                const size_t rows_offset  = memory::align(slice_offset+slice_length);
                const size_t rows_length  = num_rows * sizeof(row_type);

                const size_t data_offset  = memory::align(rows_offset+rows_length);
                const size_t data_length  = this->items * sizeof(T);

                wlen = data_offset+data_length;
                wksp = memory::kind<memory::global>::acquire(wlen);

                uint8_t      *q  = (uint8_t *)wksp;
                slices           = (slice_type *) &q[slice_offset];
                row_type     *pr = (row_type   *) &q[rows_offset ];
                T            *pd = (T          *) &q[data_offset ];
                data = pd;
                const coord2D c2lo(this->lower.x,this->lower.y);
                const coord2D c2hi(this->upper.x,this->upper.y);
                const patch2D p2(c2lo,c2hi);
                const size_t  items_per_slice = p2.items;
                size_t islice = 0;
                try
                {
                    while(islice<num_slices)
                    {
                        new ( &slices[islice] ) slice_type(p2,pr,pd);
                        ++islice;
                        pr +=  rows_per_slice;
                        pd += items_per_slice;
                    }
                }
                catch(...)
                {
                    while(islice>0)
                    {
                        destruct<slice_type>( &slices[--islice] );
                    }
                    memory::kind<memory::global>::release(wksp,wlen);
                    throw;
                }
                slices -= this->lower.z;
                (size_t &)(this->owned_memory)  = wlen;
            }

            inline virtual ~field3D() throw()
            {
                slices += this->lower.z;
                size_t islice = this->width.z;
                while(islice>0)
                {
                    destruct<slice_type>( &slices[--islice] );
                }
                
                memory::kind<memory::global>::release(wksp,wlen);
                data = 0;
            }
            
            inline slice_type & operator[](const unit_t z) throw()
            {
                assert(z>=this->lower.z);
                assert(z<=this->upper.z);
                return slices[z];
            }
            
        private:
            slice_type *slices;
            T          *data;
            size_t      wlen;
            void       *wksp;
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(field3D);
            virtual const void *item_at(const coord3D &C) const throw()
            {

                assert(this->has(C));
                return &slices[C.z][C.y][C.x];
            }

            virtual const void *base_of() const throw() { return data; }

        };
        
    }
    
    
}


#endif

