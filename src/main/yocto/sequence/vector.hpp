#ifndef YOCTO_SEQUENCE_VECTOR_INCLUDED
#define YOCTO_SEQUENCE_VECTOR_INCLUDED 1


#include "yocto/container/sequence.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/bitwise.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/round.hpp"
#include "yocto/code/cast.hpp"
#include "yocto/sequence/array.hpp"

#include <cstring>

namespace yocto
{
    namespace hidden { extern const char vector_name[]; }

    template <typename T, typename ALLOCATOR = memory::global::allocator >
    class vector  : public virtual sequence<T>, public virtual array<T>
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        enum
        {
            bitwise_ops = support_bitwise_ops<mutable_type>::value,
            no_destruct = support_no_destruct<mutable_type>::value,
            c_style_ops = support_c_style_ops<mutable_type>::value
        };


        //! default no throw constructor
        explicit vector() throw() : hmem_(),  size_(0), maxi_(0), addr_(0), item_(addr_-1) {}

        //! destructor
        virtual ~vector() throw() { _release(); }

#define YOCTO_VECTOR_IMPL(COUNT) \
hmem_(), size_(0), maxi_(COUNT), \
addr_(hmem_.template acquire_as<mutable_type>( maxi_ ) ),\
item_(addr_-1)

        //! copy constructor
        vector( const vector &other ) :
        YOCTO_VECTOR_IMPL(other.size_)
        {
            assert(maxi_>=other.size_);
            _duplicate(other);
        }


        explicit vector(size_t n, const as_capacity_t &) :
        YOCTO_VECTOR_IMPL(n)
        {
            assert(maxi_>=n);
        }

        //! a copy of n objects
        explicit vector( size_t n, param_type obj ) :
        YOCTO_VECTOR_IMPL(n)
        {
            assert(maxi_>=n);
            try
            {
                for(size_t i=1;i<=n;++i)
                {
                    new ( &item_[i] ) mutable_type(obj);
                    ++size_;
                }
            }
            catch(...)
            {
                _release();
                throw;
            }
        }

        //! assuming an empty ctor is valid
        explicit vector(size_t n) :
        YOCTO_VECTOR_IMPL(n)
        {
            try { ctor0( int2type<c_style_ops>(), n ); } catch(...) { _release(); throw; }
        }


        inline vector & operator=( const vector &other )
        {
            if(this!=&other)
            {
                vector tmp( other );
                swap_with(tmp);
            }
            return *this;
        }

        //======================================================================
        // container interface
        //======================================================================
        virtual const char *name()     const throw() { return hidden::vector_name; }
        virtual size_t      size()     const throw() { return size_; }
        virtual size_t      capacity() const throw() { return maxi_; }

#define YOCTO_VECTOR_XCH(FIELD) cswap_const(FIELD,other.FIELD)
        inline void swap_with( vector &other ) throw()
        {
            YOCTO_VECTOR_XCH(size_);
            YOCTO_VECTOR_XCH(maxi_);
            YOCTO_VECTOR_XCH(addr_);
            YOCTO_VECTOR_XCH(item_);
        }
#undef YOCTO_VECTOR_XCH

        virtual void free()    throw() { _free( int2type<no_destruct>() ); }

        virtual void release() throw() { _release(); }

        virtual void reserve( size_t n )
        {
            if(n>0)
            {
                vector tmp(*this,n);
                swap_with(tmp);
            }
        }

        //======================================================================
        // sequence interface
        //======================================================================

        //! push back, obj can be part of vector
        virtual void push_back( param_type obj )
        {
            if( size_ < maxi_ )
            {
                new ( &addr_[size_] ) mutable_type(obj);
                ++size_;
            }
            else {
                vector tmp( *this, this->next_increase(maxi_) );
                tmp.push_back(obj);
                swap_with(tmp);
            }
        }

        //! push_back with enought memory
        inline void __push_back(param_type obj)
        {
            assert(size_<maxi_);
            new ( &addr_[size_] ) mutable_type(obj);
            ++size_;
        }

        //! fast pop_back
        virtual void pop_back() throw()           { assert(size_>0); destruct( &item_[size_--] ); }

        //! push front, slow if bitwise ops are not supported
        virtual void push_front( param_type obj ) { _push_front(obj,int2type<bitwise_ops>()); }

        //! pop front, slow if no bitwise ops are not supported
        virtual void pop_front()                  { assert(size_>0); _pop_front( int2type<bitwise_ops>() ); }



        //! make: free and make a copy
        inline void make( size_t n, param_type obj )
        {
            if( n > size_ )
            {
                vector tmp(n,obj);
                swap_with(tmp);
            }
            else
            {
                _free( int2type<no_destruct>() );
                for( size_t i=1; i<=n; ++i )
                {
                    new ( &item_[i] ) mutable_type(obj);
                    ++size_;
                }
            }
        }

        //! free and make 'zero' object
        inline void make(const size_t n)
        {
            if(n>size_)
            {
                vector tmp(n);
                swap_with(tmp);
            }
            else
            {
                _free( int2type<no_destruct>() );
                make0( int2type<support_c_style_ops<mutable_type>::value>(),n);
            }
            assert(size_==n);
        }


        inline void keep( size_t n ) throw()
        {
            while( size_ > n )
            {
                pop_back();
            }
        }

        //! Warning, bitwise operator !
        virtual void reverse() throw()
        {
            mreverse<mutable_type>(addr_, size_);
        }

        //! possible if empty type constructor
        inline void append()
        {
            if(size_<maxi_)
            {
                _append( int2type<support_c_style_ops<mutable_type>::value>() );
            }
            else
            {
                vector tmp( *this, this->next_increase(maxi_) );
                tmp.append();
                swap_with(tmp);
            }
        }


        //! one parameter ctor append
        template <typename U>
        void append(typename type_traits<U>::parameter_type u)
        {
            if(size_<maxi_)
            {
                new ( &addr_[size_] ) mutable_type(u);
                ++size_;
            }
            else
            {
                vector tmp( *this, this->next_increase(maxi_) );
                tmp.append<U>(u);
                swap_with(tmp);
            }
        }

        template <typename U,typename V>
        void append(typename type_traits<U>::parameter_type u,
                    typename type_traits<V>::parameter_type v)
        {
            if(size_<maxi_)
            {
                new ( &addr_[size_] ) mutable_type(u,v);
                ++size_;
            }
            else
            {
                vector tmp( *this, this->next_increase(maxi_) );
                tmp.append<U,V>(u,v);
                swap_with(tmp);
            }
        }

        template <typename U,typename V,typename W>
        void append(typename type_traits<U>::parameter_type u,
                    typename type_traits<V>::parameter_type v,
                    typename type_traits<W>::parameter_type w)
        {
            if(size_<maxi_)
            {
                new ( &addr_[size_] ) mutable_type(u,v,w);
                ++size_;
            }
            else
            {
                vector tmp( *this, this->next_increase(maxi_) );
                tmp.append<U,V,W>(u,v,w);
                swap_with(tmp);
            }
        }





    private:
        ALLOCATOR     hmem_;
        size_t        size_;
        size_t        maxi_;
        mutable_type *addr_; //! [0..size-1]
        mutable_type *item_; //! [1..size_]

        virtual const type &get_data(size_t indx) const throw()
        {
            assert(indx>0);
            assert(indx<=size());
            return item_[indx];
        }

        //! copy with extra memory
        vector( const vector &other, size_t extra ) :
        YOCTO_VECTOR_IMPL(other.maxi_+extra)
        {
            assert(maxi_>=other.maxi_+extra);
            _duplicate(other);
        }


        //______________________________________________________________________
        //
        // empty ctor | int2type<support_c_style_ops>
        //______________________________________________________________________

        //! C style
        void ctor0( int2type<true>, const size_t n) throw()
        {
            size_ = n;
        }

#define YOCTO_VECTOR_CTOR0(n) do{ assert(0==size_); while(size_<n) { new (&item_[size_+1]) mutable_type(); ++size_; } } while(false)
        //! C++ style
        void ctor0( int2type<false>, const size_t n) throw()
        {
            YOCTO_VECTOR_CTOR0(n);
        }

        //______________________________________________________________________
        //
        // empty make | int2type<support_c_style_ops>
        //______________________________________________________________________


        void make0(int2type<true>,const size_t n) throw()
        {
            assert(0==size_);
            memset(addr_,0x00,(size_=n)*sizeof(T));
        }

        void make0(int2type<false>,const size_t n)
        {
            YOCTO_VECTOR_CTOR0(n);
        }
#undef YOCTO_VECTOR_CTOR0


        //______________________________________________________________________
        //
        // empty append | int2type<support_c_style_ops>
        //______________________________________________________________________
        // C style
        void _append(int2type<true>()) throw()
        {
            assert(size_<maxi_);
            mutable_type *target = addr_+size_;
            bzset(target);
            ++size_;
        }

        // C++ style
        void _append(int2type<false>())
        {
            assert(size_<maxi_);
            mutable_type *target = addr_+size_;
            new (target) mutable_type();
            ++size_;
        }



        //______________________________________________________________________
        //
        // _duplicate
        //______________________________________________________________________
        inline void _duplicate( const vector &other )
        {
            assert(0==size_);
            assert(maxi_>=other.size_);
            try
            {
                for( size_t i=1; i <= other.size_; ++i )
                {
                    new ( &item_[i] ) mutable_type( other.item_[i] );
                    ++size_;
                }
            }
            catch(...)
            {
                _release();
                throw;
            }
        }

        //----------------------------------------------------------------------
        // _free( int2type<no_destruct> )
        //----------------------------------------------------------------------
        inline void _free( const int2type<true>  ) throw() { size_=0; }
        inline void _free( const int2type<false> ) throw()
        {
            while( size_>0 )
            {
                destruct( &item_[size_--] );
            }
        }

        //----------------------------------------------------------------------
        // release memory
        //----------------------------------------------------------------------
        inline void _release() throw()
        {
            _free( int2type<no_destruct>() );
            assert(0==size_);
            hmem_.template release_as<mutable_type>(addr_,maxi_);
            assert(NULL==addr_);
            assert(0==maxi_);
            item_ = addr_ - 1;
        }


        //----------------------------------------------------------------------
        // _pop_front | int2type<bitwise_ops>
        //----------------------------------------------------------------------
        inline void _pop_front( int2type<true> ) throw()
        {
            mutable_type *target = addr_;
            destruct(target);
            memmove( (void*)target, (void*)(target+1), --size_ * sizeof(T));
        }

        inline void _pop_front( int2type<false> )
        {
            vector tmp( size_-1, as_capacity );
            for(size_t i=1; i<size_; ++i )
            {
                new ( &tmp.item_[i] ) mutable_type( item_[i+1] );
                ++tmp.size_;
            }
            swap_with(tmp);
        }

        //----------------------------------------------------------------------
        // _push_front | int2type<bitwise_ops>
        //----------------------------------------------------------------------
        inline void _push_front( param_type obj, int2type<true> )
        {
            if( size_ < maxi_ )
            {
                //--------------------------------------------------------------
                // keep a binary ghost if obj is in this vector
                //--------------------------------------------------------------
                uint64_t      wksp[ YOCTO_U64_FOR_ITEM(T) ];
                void         *addr = _cast::load<uint64_t>( wksp );
                memcpy( &wksp[0], (void*)&obj, sizeof(T));
                mutable_type *target = addr_;
                const size_t  nbytes = size_ * sizeof(T);

                //--------------------------------------------------------------
                // move and construct
                //--------------------------------------------------------------
                memmove( (void*)(target+1), (void*)target, nbytes );
                try
                {
                    new (target) mutable_type( *_cast::from<mutable_type>(addr) );
                }
                catch (...) {
                    memmove( (void*)target, (void*)(target+1), nbytes );
                    throw;
                }
                ++size_;
            }
            else
            {
                vector tmp( this->next_capacity(maxi_), as_capacity );
                _front_store(obj,tmp);
            }
        }

        inline void _push_front( param_type obj, int2type<false> )
        {
            const size_t n = size_ < maxi_ ? maxi_ : this->next_capacity(maxi_);
            vector tmp( n, as_capacity );
            _front_store(obj,tmp);
        }

        inline void _front_store( param_type obj, vector &tmp)
        {
            assert( 0 == tmp.size_ ); assert( tmp.maxi_ > size_ );
            //--------------------------------------------------------------
            // new first item
            //--------------------------------------------------------------
            mutable_type *target = &tmp.item_[1];
            new(target) mutable_type(obj);
            tmp.size_ = 1;

            //--------------------------------------------------------------
            // other items
            //--------------------------------------------------------------
            for( size_t i=1; i <= size_; ++i )
            {
                new ( ++target ) mutable_type( item_[i] );
                ++tmp.size_;
            }

            //--------------------------------------------------------------
            // ok, everything is fine !
            //--------------------------------------------------------------
            swap_with(tmp);
        }

        //----------------------------------------------------------------------
        // access
        //----------------------------------------------------------------------
        virtual const_type &get_front() const throw() { assert(size_>0); return item_[1];     }
        virtual const_type &get_back()  const throw() { assert(size_>0); return item_[size_]; }
        virtual const_type *get_item()  const throw() { return item_; }
        
    };
}

#endif
