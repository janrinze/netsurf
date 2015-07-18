/* DO NOT USE, DODGY BIT FOR VINCE */

#include <dom/dom.h>

#include "utils/log.h"

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(element, struct dom_element *element)
{
	DUKKY_FUNC_T(node, __init)(ctx, &priv->parent, (struct dom_node *)element);
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
}

DUKKY_FUNC_FINI(element)
{
	/* do any element finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	DUKKY_FUNC_T(node, __fini)(ctx, &priv->parent);
}

static DUKKY_FUNC(element, __constructor)
{
	DUKKY_CREATE_PRIVATE(element);
	DUKKY_FUNC_T(element, __init)(ctx, priv,
				      duk_get_pointer(ctx, 1));
	duk_set_top(ctx, 1);
	return 1;
}

static DUKKY_FUNC(element, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(element, 0);
	DUKKY_FUNC_T(element, __fini)(ctx, priv);
	free(priv);
	return 0;
}

DUKKY_FUNC(element, __proto)
{
	/* Populate element's prototypical functionality */

	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(node);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, element);
	DUKKY_SET_CONSTRUCTOR(0, element, 1);
	return 1; /* The proto object */
}
