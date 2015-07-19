/* DO NOT USE, DODGY BIT FOR VINCE */

#include <dom/dom.h>

#include "utils/log.h"

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(node, struct dom_node *node)
{
	DUKKY_FUNC_T(event_target, __init)(ctx, &priv->parent);
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
	priv->node = dom_node_ref(node);
}

DUKKY_FUNC_FINI(node)
{
	/* do any node finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	dom_node_unref(priv->node);
	DUKKY_FUNC_T(event_target, __fini)(ctx, &priv->parent);
}

static DUKKY_FUNC(node, __constructor)
{
	DUKKY_CREATE_PRIVATE(node);
	DUKKY_FUNC_T(node, __init)(ctx, priv,
				   duk_get_pointer(ctx, 1));
	duk_set_top(ctx, 1);
	return 1;
}

static DUKKY_FUNC(node, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(node, 0);
	DUKKY_FUNC_T(node, __fini)(ctx, priv);
	free(priv);
	return 0;
}

static DUKKY_FUNC(node, appendChild)
{
	DUKKY_GET_METHOD_PRIVATE(node);

	if (!dukky_instanceof(ctx, PROTO_NAME(node))) return 0;

	DUKKY_SAFE_GET_ANOTHER(other,node,0);

	dom_exception err;
	dom_node *spare;

	err = dom_node_append_child(priv->node, other->node, &spare);
	if (err != DOM_NO_ERR) return 0;
	dom_node_unref(spare);
	
	return 0;
}

DUKKY_FUNC(node, __proto)
{
	/* Populate node's prototypical functionality */
	DUKKY_ADD_METHOD(node, appendChild, 1);
	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(event_target);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, node);
	DUKKY_SET_CONSTRUCTOR(0, node, 1);
	return 1; /* The proto object */
}
