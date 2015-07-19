/* DO NOT USE, DODGY EXAMPLE FOR VINCE */

#include "utils/log.h"

#include <dom/dom.h>

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(node_list, struct dom_nodelist *nodes)
{
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
	priv->nodes = nodes;
	dom_nodelist_ref(nodes);
}

DUKKY_FUNC_FINI(node_list)
{
	/* do any node_list finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	dom_nodelist_unref(priv->nodes);
}

static DUKKY_FUNC(node_list, __constructor)
{
	DUKKY_CREATE_PRIVATE(node_list);
	DUKKY_FUNC_T(node_list, __init)(ctx, priv, duk_get_pointer(ctx, 1));
	duk_pop(ctx);
	return 1;
}

static DUKKY_FUNC(node_list, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(node_list, 0);
	DUKKY_FUNC_T(node_list, __fini)(ctx, priv);
	free(priv);
	return 0;
}

static DUKKY_GETTER(node_list, length)
{
	DUKKY_GET_METHOD_PRIVATE(node_list);
	dom_exception err;
	unsigned long len;
	
	err = dom_nodelist_get_length(priv->nodes, &len);
	
	if (err != DOM_NO_ERR) return 0; /* coerced to undefined */
	
	duk_push_uint(ctx, (duk_uint_t)len);
	return 1;
}

static DUKKY_FUNC(node_list, item)
{
	DUKKY_GET_METHOD_PRIVATE(node_list);
	unsigned long i = duk_to_uint(ctx, 0);
	dom_exception err;
	dom_node *node;
	
	LOG("Looking up %u in %p", i, priv->nodes);
	
	err = dom_nodelist_item(priv->nodes, i, &node);
	
	if (err != DOM_NO_ERR) return 0; /* coerced to undefied */
	
	dukky_push_node(ctx, node);
	dom_node_unref(node);
	
	return 1;
}

/** TODO: ARRAY-LIKE behaviour missing */

DUKKY_FUNC(node_list, __proto)
{
	/* Populate node_list's prototypical functionality */
	DUKKY_POPULATE_READONLY_PROPERTY(node_list, length);
	DUKKY_ADD_METHOD(node_list, item, 1);
	/* Set this prototype's prototype (left-parent)*/
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, node_list);
	DUKKY_SET_CONSTRUCTOR(0, node_list, 1);
	return 1; /* The proto object */
}
