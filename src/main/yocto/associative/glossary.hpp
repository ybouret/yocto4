#ifndef YOCTO_ASSOCIATIVE_GLOSSARY_INCLUDED
#define YOCTO_ASSOCIATIVE_GLOSSARY_INCLUDED 1

#include "yocto/hashing/mph.hpp"
#include "yocto/container/container.hpp"

namespace yocto
{
    namespace hidden
    {
        extern const char glossary_name[];
    }

    template <typename T>
    class glossary : public container
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;

        inline explicit glossary():
        hash_(),
        maxi_(0),
        size_(0),
        data_(0)
        {
        }

        inline explicit glossary(const size_t n) :
        hash_(),
        maxi_(n),
        size_(0),
        data_( memory::kind<memory::global>::acquire_as<mutable_type>(maxi_) )
        {

        }

        virtual ~glossary() throw()
        {
            release_();
        }

        virtual const char *name() const throw() { return hidden::glossary_name; }
        virtual void        free()     throw()    { free_();    }
        virtual void        release()  throw() { release_(); }
        virtual size_t      size()     const throw() { return size_; }
        virtual size_t      capacity() const throw() { return maxi_; }
        virtual void        reserve(size_t n) { if(n>0) reserve_(n); }
        //______________________________________________________________________
        //
        // search by id
        //______________________________________________________________________
        inline type *search(const string &id) throw()
        {
            const int h = hash_(id);
            if(h<0)
            {
                return 0;
            }
            else
            {
                assert(size_t(h)<size_);
                return data_+h;
            }
        }

        inline type *search(const char *id) throw()
        {
            const string ID(id);
            return search(ID);
        }

        inline const_type *search(const string &id) const throw()
        {
            const int h = hash_(id);
            if(h<0)
            {
                return 0;
            }
            else
            {
                assert(size_t(h)<size_);
                return data_+h;
            }
        }

        inline const_type *search(const char *id) const throw()
        {
            const string ID(id);
            return search(ID);
        }

        inline void insert(const string &id, param_type arg)
        {
            if(size_<maxi_)
            {
                assert(data_);
                //______________________________________________________________
                //
                // append arg to the end
                //______________________________________________________________
                new (&data_[size_]) mutable_type(arg);
                ++size_;

            }
            else
            {
                //______________________________________________________________
                //
                // acquire new data
                //______________________________________________________________
                size_t        new_maxi = container::next_capacity(maxi_);
                mutable_type *new_data = memory::kind<memory::global>::acquire_as<mutable_type>(new_maxi);
                size_t        new_size = 0;

                //______________________________________________________________
                //
                // construct new data
                //______________________________________________________________
                try
                {
                    while(new_size<size_)
                    {
                        new ( &new_data[new_size] ) mutable_type(data_[new_size]);
                        ++new_size;
                    }
                    new ( &new_data[new_size] ) mutable_type(arg);
                    ++new_size;
                }
                catch(...)
                {
                    while(new_size>0)
                    {
                        destruct<mutable_type>(&new_data[--new_size]);
                    }
                    memory::kind<memory::global>::release_as<mutable_type>(new_data,new_maxi);
                }

                //______________________________________________________________
                //
                // update (no-throw)
                //______________________________________________________________
                while(size_>0) pop_();
                memory::kind<memory::global>::release_as<mutable_type>(data_,maxi_);
                maxi_ = new_maxi;
                data_ = new_data;
                size_ = new_size;

            }

            //__________________________________________________________________
            //
            // register id
            //__________________________________________________________________
            try
            {
                hash_.insert(id,size_-1);
            }
            catch(...)
            {
                pop_();
                throw;
            }

        }


        inline void insert(const char *id, param_type arg)
        {
            const string ID(id);
            insert(ID,arg);
        }


        //! for types with a const string key() const throw() method
        inline void insert(param_type arg)
        {
            insert(arg->key(),arg);
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(glossary);
        hashing::mperf hash_;
        size_t         maxi_;
        size_t         size_;
        mutable_type  *data_;

        inline void pop_() throw()
        {
            assert(size_>0);
            destruct<mutable_type>(&data_[--size_]);
        }

        inline void free_() throw()
        {
            while(size_>0)
            {
                pop_();
            }
            hash_.release();
        }

        inline void release_() throw()
        {
            free_();
            memory::kind<memory::global>::release_as<mutable_type>(data_, maxi_);
        }

        inline void reserve_(size_t n)
        {
            assert(n>0);
            //______________________________________________________________
            //
            // acquire new data
            //______________________________________________________________
            size_t        new_maxi = maxi_ + n;
            mutable_type *new_data = memory::kind<memory::global>::acquire_as<mutable_type>(new_maxi);
            size_t        new_size = 0;

            //______________________________________________________________
            //
            // construct new data
            //______________________________________________________________
            try
            {
                while(new_size<size_)
                {
                    new ( &new_data[new_size] ) mutable_type(data_[new_size]);
                    ++new_size;
                }
            }
            catch(...)
            {
                while(new_size>0)
                {
                    destruct<mutable_type>(&new_data[--new_size]);
                }
                memory::kind<memory::global>::release_as<mutable_type>(new_data,new_maxi);
            }

            //______________________________________________________________
            //
            // update (no-throw)
            //______________________________________________________________
            while(size_>0) pop_();
            memory::kind<memory::global>::release_as<mutable_type>(data_,maxi_);
            maxi_ = new_maxi;
            data_ = new_data;
            size_ = new_size;
        }

    };
}

#endif
