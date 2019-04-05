#pragma once

using create_client_class_t = void *( *)( int ent_num, int serial_num );
using create_event_t = void *( *)();

class client_class {
public:
	create_client_class_t m_create_fn;
	create_event_t m_create_event_fn;
	char *m_network_name;
	recv_table *m_recv_table;
	client_class *m_next;
	int m_class_id;
};
