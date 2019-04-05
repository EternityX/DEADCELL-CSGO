#pragma once

struct recv_proxy_data;
struct recv_table;
struct recv_prop;

using recv_var_proxy_t = void(__cdecl *)( const recv_proxy_data *, void *, void * );

struct d_variant {
	union {
		float m_float;
		long m_int;
		char *m_string;
		void *m_data;
		float m_vector[ 3 ];
		__int64 m_int64;
	};

	int m_type;
};

struct recv_proxy_data {
	const recv_prop *m_recv_prop;
	d_variant m_value;
	int m_element;
	int m_object_id;
};

struct recv_table {
	recv_prop *m_props;
	int m_num_props;
	void *m_decoder;
	char *m_net_table_name;

private:
	PAD( 0x2 )
};

struct recv_prop {
	char *m_var_name;
	int m_recv_type;
	int m_flags;
	int m_string_buffer_size;
	bool m_inside_array;
	const void *m_extra_data;
	recv_prop *m_array_prop;
	void *m_array_length_proxy;
	recv_var_proxy_t m_proxy_fn;
	void *m_data_table_proxy_fn;
	recv_table *m_data_table;
	int m_offset;
	int m_element_stride;
	int m_num_elements;
	const char *m_parent_array_prop_name;
};
