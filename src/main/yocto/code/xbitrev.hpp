#ifndef YOCTO_XBITREV_INCLUDED
#define YOCTO_XBITREV_INCLUDED 1

#include "yocto/code/bswap.hpp"
namespace yocto { namespace math {

	struct xbitrev {
		static const size_t indx0004[   1];
		static const size_t jndx0004[   1];
		static const size_t indx0008[   2];
		static const size_t jndx0008[   2];
		static const size_t indx0016[   6];
		static const size_t jndx0016[   6];
		static const size_t indx0032[  12];
		static const size_t jndx0032[  12];
		static const size_t indx0064[  28];
		static const size_t jndx0064[  28];
		static const size_t indx0128[  56];
		static const size_t jndx0128[  56];
		static const size_t indx0256[ 120];
		static const size_t jndx0256[ 120];
		static const size_t indx0512[ 240];
		static const size_t jndx0512[ 240];
		static const size_t indx1024[ 496];
		static const size_t jndx1024[ 496];
		static const size_t indx2048[ 992];
		static const size_t jndx2048[ 992];
		static const size_t indx4096[2016];
		static const size_t jndx4096[2016];
		static const size_t indx8192[4032];
		static const size_t jndx8192[4032];

		template <typename T> static inline void run_safe( T arr[], const size_t size) throw() {
			 const    size_t n = size << 1;
			 register size_t j = 1;
			 for(register size_t i=1; i<n; i+=2)
			 {
			     if(j>i)
			     {
			         core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
			     }
			     register size_t m = size;
			     while( (m>=2) && (j>m) )
			     {
			        j -=  m;
			        m >>= 1;
			     }
			     j += m;
			 }
		}


		template <typename T> static inline void run_safe( T arr[], T brr[], const size_t size) throw() {
			 const    size_t n = size << 1;
			 register size_t j = 1;
			 for(register size_t i=1; i<n; i+=2)
			 {
			     if(j>i)
			     {
			         core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
			         core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );
			     }
			     register size_t m = size;
			     while( (m>=2) && (j>m) )
			     {
			        j -=  m;
			        m >>= 1;
			     }
			     j += m;
			 }
		}


		template <typename T> static inline void run( T arr[], const size_t size) throw() {
				switch(size) {
				 case    1:
					  break;
				 case    2:
					  break;
				 case    4:
					for(register size_t k=0;k<   1;++k) { core::bswap<2*sizeof(T)>( &arr[indx0004[k]], &arr[jndx0004[k]] ); }
					  break;
				 case    8:
					for(register size_t k=0;k<   2;++k) { core::bswap<2*sizeof(T)>( &arr[indx0008[k]], &arr[jndx0008[k]] ); }
					  break;
				 case   16:
					for(register size_t k=0;k<   6;++k) { core::bswap<2*sizeof(T)>( &arr[indx0016[k]], &arr[jndx0016[k]] ); }
					  break;
				 case   32:
					for(register size_t k=0;k<  12;++k) { core::bswap<2*sizeof(T)>( &arr[indx0032[k]], &arr[jndx0032[k]] ); }
					  break;
				 case   64:
					for(register size_t k=0;k<  28;++k) { core::bswap<2*sizeof(T)>( &arr[indx0064[k]], &arr[jndx0064[k]] ); }
					  break;
				 case  128:
					for(register size_t k=0;k<  56;++k) { core::bswap<2*sizeof(T)>( &arr[indx0128[k]], &arr[jndx0128[k]] ); }
					  break;
				 case  256:
					for(register size_t k=0;k< 120;++k) { core::bswap<2*sizeof(T)>( &arr[indx0256[k]], &arr[jndx0256[k]] ); }
					  break;
				 case  512:
					for(register size_t k=0;k< 240;++k) { core::bswap<2*sizeof(T)>( &arr[indx0512[k]], &arr[jndx0512[k]] ); }
					  break;
				 case 1024:
					for(register size_t k=0;k< 496;++k) { core::bswap<2*sizeof(T)>( &arr[indx1024[k]], &arr[jndx1024[k]] ); }
					  break;
				 case 2048:
					for(register size_t k=0;k< 992;++k) { core::bswap<2*sizeof(T)>( &arr[indx2048[k]], &arr[jndx2048[k]] ); }
					  break;
				 case 4096:
					for(register size_t k=0;k<2016;++k) { core::bswap<2*sizeof(T)>( &arr[indx4096[k]], &arr[jndx4096[k]] ); }
					  break;
				 case 8192:
					for(register size_t k=0;k<4032;++k) { core::bswap<2*sizeof(T)>( &arr[indx8192[k]], &arr[jndx8192[k]] ); }
					  break;
				 default: run_safe(arr,size);
				};
		}


		template <typename T> static inline void run( T arr[], T brr[], const size_t size) throw() {
				switch(size) {
				 case    1:
					  break;
				 case    2:
					  break;
				 case    4:
					for(register size_t k=0;k<   1;++k) { const size_t i=indx0004[k],j=jndx0004[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case    8:
					for(register size_t k=0;k<   2;++k) { const size_t i=indx0008[k],j=jndx0008[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case   16:
					for(register size_t k=0;k<   6;++k) { const size_t i=indx0016[k],j=jndx0016[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case   32:
					for(register size_t k=0;k<  12;++k) { const size_t i=indx0032[k],j=jndx0032[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case   64:
					for(register size_t k=0;k<  28;++k) { const size_t i=indx0064[k],j=jndx0064[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case  128:
					for(register size_t k=0;k<  56;++k) { const size_t i=indx0128[k],j=jndx0128[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case  256:
					for(register size_t k=0;k< 120;++k) { const size_t i=indx0256[k],j=jndx0256[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case  512:
					for(register size_t k=0;k< 240;++k) { const size_t i=indx0512[k],j=jndx0512[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case 1024:
					for(register size_t k=0;k< 496;++k) { const size_t i=indx1024[k],j=jndx1024[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case 2048:
					for(register size_t k=0;k< 992;++k) { const size_t i=indx2048[k],j=jndx2048[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case 4096:
					for(register size_t k=0;k<2016;++k) { const size_t i=indx4096[k],j=jndx4096[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 case 8192:
					for(register size_t k=0;k<4032;++k) { const size_t i=indx8192[k],j=jndx8192[k]; core::bswap<2*sizeof(T)>( &arr[i], &arr[j] ); core::bswap<2*sizeof(T)>( &brr[i], &brr[j] );}
					  break;
				 default: run_safe(arr,brr,size);
				};
		}

	};

} }
#endif
