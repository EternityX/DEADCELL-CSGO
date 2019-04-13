#pragma once

class iv_model_info {
public:
	const model_t *get_model( int modelindex ) {
		return util::misc::vfunc< model_t *( __thiscall *)( decltype( this ), int ) >( this, 1 )( this, modelindex );
	}
	const int get_model_index( const char *name ){
		return util::misc::vfunc< int( __thiscall *)( decltype( this ), const char * ) >( this, 2 )( this, name );
	}

	const char *get_model_name( const model_t *model ) {
		return util::misc::vfunc< const char *( __thiscall *)( decltype( this ), const model_t * )
		>( this, 3 )( this, model );
	}

	studiohdr_t *get_studio_model( const model_t *model ) {
		return util::misc::vfunc< studiohdr_t *( __thiscall *)( decltype( this ), const model_t * )
		>( this, 32 )( this, model );
	}
};

class c_studio_hdr
{
public:
	studiohdr_t * m_studio_hdr; //0x0000 
	virtualmodel_t *	m_virtual_model; //0x0004 
	c_utl_vector< const studiohdr_t * > m_studio_hdr_cache; //0x0008 
	int					m_frame_unlock_counter; //0x001C 
	int*				m_p_frame_unlock_counter_ptr; //0x0020 
	char pad_0x0024[ 0x8 ]; //0x0024
	c_utl_vector< int >	m_bone_flags; //0x002C 
	c_utl_vector< int >	m_bone_parent; //0x0040 
	inline int			numbones( void ) const { return m_studio_hdr->numbones; };
	inline mstudiobone_t *bone( int i ) const { return m_studio_hdr->bone( i ); };
}; //Size=0x0054
