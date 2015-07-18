#ifndef DUKTAPE_PROTOTYPES_H
#define DUKTAPE_PROTOTYPES_H

DUKKY_DECLARE_INTERFACE(event_target);
DUKKY_DECLARE_INTERFACE(window, struct browser_window *, struct html_content *);
DUKKY_DECLARE_INTERFACE(node, struct dom_node *);
DUKKY_DECLARE_INTERFACE(document, struct dom_document *);
DUKKY_DECLARE_INTERFACE(element, struct dom_element *);
DUKKY_DECLARE_INTERFACE(html_element, struct dom_html_element *);
DUKKY_DECLARE_INTERFACE(html_unknown_element, struct dom_html_element *);

#endif

