#ifndef DUKKY_PRIVATE_H
#define DUKKY_PRIVATE_H

struct browser_window;
struct html_content;
struct dom_node;
struct dom_element;
struct dom_document;
struct dom_html_element;
struct dom_node_character_data;
struct dom_node_text;
struct dom_node_comment;

typedef struct {
} event_target_private_t;

typedef struct {
	event_target_private_t parent;
	struct browser_window *win;
	struct html_content *htmlc;
} window_private_t;

typedef struct {
	event_target_private_t parent;
	struct dom_node *node;
} node_private_t;

typedef struct {
	node_private_t parent;
} character_data_private_t;

typedef struct {
	character_data_private_t parent;
} text_private_t;

typedef struct {
	character_data_private_t parent;
} comment_private_t;

typedef struct {
	node_private_t parent;
} element_private_t;

typedef struct {
	element_private_t parent;
} html_element_private_t;

typedef struct {
	html_element_private_t parent;
} html_unknown_element_private_t;

typedef struct {
	node_private_t parent;
} document_private_t;

#endif
