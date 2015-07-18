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

DUKKY_FUNC(document, __proto)
{
	/* Populate document's prototypical functionality */
	DUKKY_ADD_METHOD(document, write, 1);
	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(node);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, document);
	DUKKY_SET_CONSTRUCTOR(0, document, 1);
	return 1; /* The proto object */
}
