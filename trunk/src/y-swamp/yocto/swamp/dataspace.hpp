#ifndef YOCTO_SWAMP_DATASPACE_INCLUDED
#define YOCTO_SWAMP_DATASPACE_INCLUDED 1

#include "yocto/swamp/ghosts.hpp"
#include "yocto/swamp/factory.hpp"
#include "yocto/swamp/fields.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
	namespace swamp 
	{

#if defined(_MSC_VER)
#pragma warning (push)
		// this in ctor
#pragma warning ( disable : 4355 )
#endif
		//! multiple fields manager
		template <typename LAYOUT>
		class dataspace : public LAYOUT,  public array_db
		{
		public:
			typedef typename LAYOUT::coord       coord;

			//! prepare all layouts
			explicit dataspace(const LAYOUT               &L,
				const ghosts_setup<coord>  &G,
				const fields_setup<LAYOUT> &F
				) :
			LAYOUT(L),
				array_db(),
				outline( *this ),
				nucleus( *this ),
				in_layout(),
				localGhosts(4,as_capacity),
				asyncGhosts(8,as_capacity),
				usingGhosts(),
				fieldsMaker(8)
			{
				record_ghosts(G);
				record_fields(F);
			}

			virtual ~dataspace() throw() {}


			//! manually create a new array
			/**
			\param name the unique array name
			\param async if true, will use ghosts exchange
			*/
			template <typename ARRAY>
			inline ARRAY &create( const string &name, bool async = true )
			{
				array_db  &adb = *this;
				fieldsMaker.template make<ARRAY>(name,outline,adb);
				ARRAY    &ans = adb[ name ].as<ARRAY>();
				if( async )
				{
					usingGhosts.push_back( &ans );
				}
				return ans;
			}

			//! manually create a new array, wrapper
			template <typename ARRAY>
			inline ARRAY &create( const char *id, bool async = true )
			{
				const string name(id);
				return create<ARRAY>(name);
			}

			//! allocate all data for communications
			/**
			This should be done only once, after all
			arrays are created.
			*/
			void  prepare_ghosts()
			{
				for( size_t i=1; i <= asyncGhosts.size(); ++i )
				{
					asyncGhosts[i]->allocate_for( usingGhosts );
				}
			}



			//! number of local ghosts for PBC
			inline size_t  local_ghosts_count() const throw() { return localGhosts.size(); }

			//! number of async ghosts for communication 
			inline size_t  async_ghosts_count() const throw() { return asyncGhosts.size(); }

			local_ghosts & __local_ghosts( size_t index ) throw() { return *localGhosts[index];}
			async_ghosts & __async_ghosts( size_t index ) throw() { return *asyncGhosts[index];} 

			//! handles to array using communication
			const linear_handles & handles() const throw() { return usingGhosts; }

			//! number of MPI requests for communication
			size_t num_requests() const throw() { return 2 * asyncGhosts.size() ; }

			const LAYOUT       outline;    //!< layout+ghosts
			const LAYOUT       nucleus;    //!< layout - async ghosts: always synchronous
			const offsets_list in_layout;  //!< layout offsets
			const offsets_list in_ghosts;  //!< offsets in layout BUT NOT in nucleus

		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(dataspace);
			vector<local_ghosts::ptr> localGhosts;
			vector<async_ghosts::ptr> asyncGhosts;
			vector<linear_base *>     usingGhosts;
			factory<LAYOUT>           fieldsMaker;

			//! compute outline and ghosts from the setup
			inline void record_ghosts( const ghosts_setup<coord> &G )
			{
				const unit_t *local_g = (const unit_t *) &G.local.count;
				const unit_t *lower_g = (const unit_t *) &G.lower.count;
				const unit_t *upper_g = (const unit_t *) &G.upper.count;

				//==============================================================
				//
				// First pass: compute outline, sync zone and register ghosts
				//
				//==============================================================
				{
					const unit_t *lower_p = (const unit_t *) &G.lower.peer;
					const unit_t *upper_p = (const unit_t *) &G.upper.peer;
					const unit_t *w       = (const unit_t *) & this->width;
					coord         outLo( this->lower );
					coord         outUp( this->upper );
					unit_t  *     pLower = (unit_t *) &outLo;
					unit_t  *     pUpper = (unit_t *) &outUp;
					coord         syncLo( this->lower );
					coord         syncUp( this->upper );

					for( unsigned dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
					{
						const unit_t local_ng = local_g[dim];
						if( local_ng > 0 )
						{
							//--------------------------------------------------
							// check layout width
							//--------------------------------------------------
							if( local_ng > w[dim] )
								throw exception("too many local ghosts in dimension %u", dim);

							//--------------------------------------------------
							// create the local ghosts
							//--------------------------------------------------
							local_ghosts::ptr pG( new local_ghosts(local_ng,dim) );
							localGhosts.push_back( pG );
							pLower[dim] -= local_ng;
							pUpper[dim] += local_ng;
						}

						{
							const unit_t lower_ng = lower_g[dim];
							if( lower_ng > 0 )
							{
								if( local_ng > 0 )
									throw exception("ghosts conflict in dimension %u", dim );
								async_ghosts::ptr pG( new async_ghosts(lower_ng, ghost::get_lower_position(dim), lower_p[dim] ) );
								asyncGhosts.push_back( pG );
								pLower[dim] -= lower_ng;
								__coord(syncLo,dim) += lower_ng;
							}
						}

						{
							const unit_t upper_ng = upper_g[dim];
							if( upper_ng > 0 )
							{
								if( local_ng > 0 )
									throw exception("ghosts conflict in dimension %u", dim );
								async_ghosts::ptr pG( new async_ghosts(upper_ng, ghost::get_upper_position(dim), upper_p[dim] ) );
								asyncGhosts.push_back( pG );
								pUpper[dim] += upper_ng;
								__coord(syncUp,dim) -= upper_ng;
							}
						}

						if(  __coord(syncUp,dim) <  __coord(syncLo,dim))
							throw exception("too many async ghosts in dimension %u", dim );

					}

					//==========================================================
					//
					// recreate outline and nucleus zone
					//
					//==========================================================
					new ((void*)&outline) LAYOUT( outLo,  outUp );
					new ((void*)&nucleus) LAYOUT( syncLo, syncUp);
					outline.load_offsets( this->__layout(), (offsets_list &)in_layout);
					offsets_list &goff = (offsets_list &)in_ghosts;
					assert(this->__layout().items >= nucleus.items );
					goff.reserve(this->__layout().items - nucleus.items );
					collect_in_ghosts(goff, int2type<LAYOUT::DIMENSIONS>() );
				}

				{
					//==========================================================
					//
					// second pass: attribute offsets with sub layouts
					//
					//==========================================================
					const coord outLo( outline.lower );
					const coord outUp( outline.upper );
					const coord layLo( this->lower );
					const coord layUp( this->upper );

					size_t iLocal=0, iAsync=0;
					for( unsigned dim=0; dim < LAYOUT::DIMENSIONS; ++dim )
					{

						//------------------------------------------------------
						//
						// local ghosts
						//
						//------------------------------------------------------
						const unit_t local_ng = local_g[dim];
						if( local_ng > 0 )
						{
							const unit_t ns = local_ng - 1;
							local_ghosts &g = * localGhosts[ ++iLocal ];
							//--------------------------------------------------
							//-- lower.inside
							//--------------------------------------------------
							{
								coord subLo( outLo );
								coord subUp( outUp );
								__coord(subLo,dim) = __coord(layLo,dim);
								__coord(subUp,dim) = __coord(subLo,dim) + ns;
								const LAYOUT sub(subLo,subUp);
								outline.load_offsets( sub, g.lower.inside.offsets );
								assert( sub.items == g.lower.inside.offsets.size() );
								//std::cerr << "local.lower.inside: " << sub << std::endl;
								//std::cerr << "@" << g.lower.inside.offsets << std::endl;
							}

							//--------------------------------------------------
							//-- lower.mirror = upper.outside
							//--------------------------------------------------
							{
								coord subLo( outLo );
								coord subUp( outUp );
								__coord(subLo,dim) = __coord(layUp,dim) + 1;
								const LAYOUT sub(subLo,subUp);
								outline.load_offsets( sub, g.lower.mirror.offsets );
								assert( sub.items == g.lower.mirror.offsets.size() );
								// std::cerr << "local.lower.mirror: " << sub << std::endl;
								// std::cerr << "@" << g.lower.mirror.offsets << std::endl;
							}

							//--------------------------------------------------
							//-- upper.inside
							//--------------------------------------------------
							{
								coord subLo( outLo );
								coord subUp( outUp );
								__coord(subUp,dim) = __coord(layUp,dim);
								__coord(subLo,dim) = __coord(subUp,dim) - ns;
								const LAYOUT sub(subLo,subUp);
								outline.load_offsets( sub, g.upper.inside.offsets );
								assert( sub.items == g.upper.inside.offsets.size() );
								//std::cerr << "local.upper.inside: " << sub << std::endl;
								//std::cerr << "@" << g.upper.inside.offsets << std::endl;

							}

							//--------------------------------------------------
							//-- upper.mirror = lower.outside
							//--------------------------------------------------
							{
								coord subLo( outLo );
								coord subUp( outUp );
								__coord(subUp,dim) = __coord(layLo,dim) - 1;
								const LAYOUT sub(subLo,subUp);
								outline.load_offsets( sub, g.upper.mirror.offsets );
								assert( sub.items == g.upper.mirror.offsets.size() );
								//std::cerr << "local.upper.mirror: " << sub << std::endl;
								//std::cerr << "@" << g.upper.mirror.offsets << std::endl;

							}

							//--------------------------------------------------
							// finalize
							//--------------------------------------------------
							(size_t &)(g.num_offsets) = g.lower.inside.offsets.size();
							assert( g.lower.mirror.offsets.size() == g.num_offsets);
							assert( g.upper.inside.offsets.size() == g.num_offsets);
							assert( g.upper.mirror.offsets.size() == g.num_offsets);
						}

						//------------------------------------------------------
						//
						// lower ghosts
						//
						//------------------------------------------------------
						{
							const unit_t lower_ng = lower_g[dim];
							if( lower_ng > 0 )
							{
								async_ghosts &g = * asyncGhosts[ ++iAsync ];
								const unit_t ns = lower_ng - 1;
								//----------------------------------------------
								// lower.inner
								//----------------------------------------------
								{
									coord subLo( outLo );
									coord subUp( outUp );
									__coord(subLo,dim) = __coord(layLo,dim);
									__coord(subUp,dim) = __coord(subLo,dim) + ns;
									const LAYOUT sub(subLo,subUp);
									outline.load_offsets( sub, g.self.inner.offsets );
									assert( sub.items == g.self.inner.offsets.size() );
								}

								//----------------------------------------------
								// lower.outer
								//----------------------------------------------
								{
									coord subLo( outLo );
									coord subUp( outUp );
									__coord(subUp,dim) = __coord(layLo,dim) - 1;
									const LAYOUT sub(subLo,subUp);
									outline.load_offsets( sub, g.self.outer.offsets );
									assert( sub.items == g.self.outer.offsets.size() );
								}

								//----------------------------------------------
								// finalize
								//----------------------------------------------
								(size_t&)(g.num_offsets) = g.self.inner.offsets.size();
								assert( g.self.outer.offsets.size() == g.num_offsets);
							}
						}

						//------------------------------------------------------
						//
						// upper ghosts
						//
						//------------------------------------------------------
						{
							const unit_t upper_ng = upper_g[dim];
							if( upper_ng > 0 )
							{
								async_ghosts &g = * asyncGhosts[ ++iAsync ];
								const unit_t ns = upper_ng - 1;

								//----------------------------------------------
								// upper.inner
								//----------------------------------------------
								{
									coord subLo( outLo );
									coord subUp( outUp );
									__coord(subUp,dim) = __coord(layUp,dim);
									__coord(subLo,dim) = __coord(subUp,dim) - ns;
									const LAYOUT sub(subLo,subUp);
									outline.load_offsets(sub, g.self.inner.offsets );
									assert( sub.items ==  g.self.inner.offsets.size() );
								}

								//----------------------------------------------
								// upper.outer
								//----------------------------------------------
								{
									coord subLo( outLo );
									coord subUp( outUp );
									__coord(subLo,dim) = __coord(layUp,dim) + 1;
									const LAYOUT sub(subLo,subUp);
									outline.load_offsets(sub, g.self.outer.offsets );
									assert( sub.items ==  g.self.outer.offsets.size() );
								}

								//----------------------------------------------
								// finalize
								//----------------------------------------------
								(size_t&)(g.num_offsets) = g.self.inner.offsets.size();
								assert( g.self.outer.offsets.size() == g.num_offsets);
							}
						}


					}
				}

			}

			inline void collect_in_ghosts( offsets_list &goff, int2type<1> )
			{
				for( unit_t i=this->lower;i<=this->upper;++i)
				{
					if( !nucleus.has(i) )
					{
						goff.store( outline.offset_of(i) );
					}
				}
			}


			inline void collect_in_ghosts( offsets_list &goff, int2type<2> )
			{
				for( unit_t j=this->lower.y;j<=this->upper.y;++j)
				{
					for( unit_t i=this->lower.x;i<=this->upper.x;++i)
					{
						const coord c(i,j);
						if( !nucleus.has(c) )
						{
							goff.store( outline.offset_of(c) );
						}
					}
				}
			}

			inline void collect_in_ghosts( offsets_list &goff, int2type<3> )
			{
				for( unit_t k=this->lower.z;k<=this->upper.z;++k)
				{
					for( unit_t j=this->lower.y;j<=this->upper.y;++j)
					{
						for( unit_t i=this->lower.x;i<=this->upper.x;++i)
						{
							const coord c(i,j,k);
							if( !nucleus.has(c) )
							{
								goff.store( outline.offset_of(c) );
							}
						}
					}
				}

			}


			inline void record_fields( const fields_setup<LAYOUT> &F )
			{
				for( typename fields_setup<LAYOUT>::iterator i = F.begin(); i != F.end(); ++i )
				{
					array_db &adb = *this;
					const field_info<LAYOUT> &f = *i;
					fieldsMaker.record(  f.spec, f.held, f.ctor,  f.dtor );
					fieldsMaker.produce( f.name, outline, f.spec,  adb );
					if( f.async )
					{
						usingGhosts.push_back( adb[ f.name ].handle() );
					}
				}
				//fieldsMaker.clear();
			}
		};
#if defined(_MSC_VER)
#pragma warning (pop)
#endif

	}
}


#endif
