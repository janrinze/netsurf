/* DO NOT USE, DODGY BIT FOR VINCE */

#include <dom/dom.h>

#include "utils/log.h"
#include "utils/corestrings.h"
#include "render/html_internal.h"
#include "utils/libdom.h"

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(document, struct dom_document *document)
{
	DUKKY_FUNC_T(node, __init)(ctx, &priv->parent, (struct dom_node *)document);
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
}

DUKKY_FUNC_FINI(document)
{
	/* do any document finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	DUKKY_FUNC_T(node, __fini)(ctx, &priv->parent);
}

static DUKKY_FUNC(document, __constructor)
{
	DUKKY_CREATE_PRIVATE(document);
	DUKKY_FUNC_T(document, __init)(ctx, priv,
				      duk_get_pointer(ctx, 1));
	duk_set_top(ctx, 1);
	return 1;
}

static DUKKY_FUNC(document, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(document, 0);
	DUKKY_FUNC_T(document, __fini)(ctx, priv);
	free(priv);
	return 0;
}

static DUKKY_FUNC(document, write)
{
	DUKKY_GET_METHOD_PRIVATE(document);
	struct html_content *htmlc;
	duk_size_t text_len;
	const char *text = duk_get_lstring(ctx, 0, &text_len);
	dom_exception err;
	err = dom_node_get_user_data(priv->parent.node,
				     corestring_dom___ns_key_html_content_data,
				     &htmlc);
	if (err == DOM_NO_ERR && htmlc->parser != NULL) {
		dom_hubbub_parser_insert_chunk(htmlc->parser, (uint8_t *)text, text_len);
	}
	return 0;
}

static DUKKY_FUNC(document, createTextNode)
{
	DUKKY_GET_METHOD_PRIVATE(document);
	dom_node *newnode;
	dom_exception err;
	duk_size_t text_len;
	const char *text = duk_safe_to_lstring(ctx, 0, &text_len);
	dom_string *text_str;
	
	err = dom_string_create((const uint8_t*)text, text_len, &text_str);
	if (err != DOM_NO_ERR) return 0; /* coerced to undefined */
	
	err = dom_document_create_text_node(priv->parent.node,
					    text_str,
					    &newnode);
	if (err != DOM_NO_ERR) {
		dom_string_unref(text_str);
		return 0; /* coerced to undefined */
	}
	
	dom_string_unref(text_str);
	
	dukky_push_node(ctx, newnode);
	
	dom_node_unref(newnode);

	return 1;
}

static DUKKY_GETTER(document, body)
{
	DUKKY_GET_METHOD_PRIVATE(document);
	struct dom_nodelist *nodes;
	struct dom_node *retnode;
	dom_exception err;
	err = dom_document_get_elements_by_tag_name(priv->parent.node,
						    corestring_dom_BODY,
						    &nodes);
	if (err != DOM_NO_ERR) return 0; /* coerced to undefined */
	
	err = dom_nodelist_item(nodes, 0, &retnode);
	
	if (err != DOM_NO_ERR) {
		dom_nodelist_unref(nodes);
		return 0; /* coerced to undefined */
	}
	
	dom_nodelist_unref(nodes);
	
	if (retnode == NULL) return 0; /* coerced to undefined */
	
	dukky_push_node(ctx, retnode);
	
	dom_node_unref(retnode);
	
	return 1;
}

DUKKY_FUNC(document, __proto)
{
	/* Populate document's prototypical functionality */
	DUKKY_ADD_METHOD(document, write, 1);
	DUKKY_ADD_METHOD(document, createTextNode, 1);
	DUKKY_POPULATE_READONLY_PROPERTY(document, body);
	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(node);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, document);
	DUKKY_SET_CONSTRUCTOR(0, document, 1);
	return 1; /* The proto object */
}
