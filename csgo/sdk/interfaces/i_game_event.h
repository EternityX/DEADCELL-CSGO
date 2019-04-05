#pragma once

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class bf_write;
class bf_read;

class i_game_event {
public:
	virtual ~i_game_event() = 0;
	virtual const char *get_name() const = 0;

	virtual bool is_reliable() const = 0;
	virtual bool is_local() const = 0;
	virtual bool is_empty( const char *keyName = nullptr ) = 0;

	virtual bool get_bool( const char *keyName = nullptr, bool defaultValue = false ) = 0;
	virtual int get_int( const char *keyName = nullptr, int defaultValue = 0 ) = 0;
	virtual uint64_t get_uint64( const char *keyName = nullptr, unsigned long defaultValue = 0 ) = 0;
	virtual float get_float( const char *keyName = nullptr, float defaultValue = 0.0f ) = 0;
	virtual const char *get_string( const char *keyName = nullptr, const char *defaultValue = "" ) = 0;
	virtual const wchar_t *get_wstring( const char *keyName, const wchar_t *defaultValue = L"" ) = 0;

	virtual void set_bool( const char *keyName, bool value ) = 0;
	virtual void set_int( const char *keyName, int value ) = 0;
	virtual void set_uint64( const char *keyName, unsigned long value ) = 0;
	virtual void set_float( const char *keyName, float value ) = 0;
	virtual void set_string( const char *keyName, const char *value ) = 0;
	virtual void set_wstring( const char *keyName, const wchar_t *value ) = 0;
};

class i_game_event_listener {
public:
	virtual ~i_game_event_listener( void ) {}

	virtual void fire_game_event( i_game_event *event ) = 0;
	virtual int get_event_debug_id( void ) = 0;

public:
	int m_debug_id;
};

class i_game_event_manager {
public:
	virtual ~i_game_event_manager() = 0;
	virtual int load_events_from_file( const char *filename ) = 0;
	virtual void reset() = 0;
	virtual bool add_listener( i_game_event_listener *listener, const char *name, bool bServerSide ) = 0;
	virtual bool find_listener( i_game_event_listener *listener, const char *name ) = 0;
	virtual void remove_listener( i_game_event_listener *listener ) = 0;
	virtual i_game_event * create_event( const char *name, bool bForce, unsigned int dwUnknown ) = 0;
	virtual bool fire_event( i_game_event *event, bool bDontBroadcast = false ) = 0;
	virtual bool fire_event_client_side( i_game_event *event ) = 0;
	virtual i_game_event *duplicate_event( i_game_event *event ) = 0;
	virtual void free_event( i_game_event *event ) = 0;
	virtual bool serialize_event( i_game_event *event, bf_write *buf ) = 0;
	virtual i_game_event *unserialize_event( bf_read *buf ) = 0;
};
