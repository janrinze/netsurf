/* DO NOT USE, DODGY BIT FOR VINCE */

#include <dom/dom.h>

#include "utils/log.h"

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(character_data, struct dom_node_character_data *character_data)
{
	DUKKY_FUNC_T(node, __init)(ctx, &priv->parent, (struct dom_node *)character_data);
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
}

DUKKY_FUNC_FINI(character_data)
{
	/* do any character_data finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	DUKKY_FUNC_T(node, __fini)(ctx, &priv->parent);
}

static DUKKY_FUNC(character_data, __constructor)
{
	DUKKY_CREATE_PRIVATE(character_data);
	DUKKY_FUNC_T(character_data, __init)(ctx, priv,
					     duk_get_pointer(ctx, 1));
	duk_set_top(ctx, 1);
	return 1;
}

static DUKKY_FUNC(character_data, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(character_data, 0);
	DUKKY_FUNC_T(character_data, __fini)(ctx, priv);
	free(priv);
	return 0;
}

DUKKY_FUNC(character_data, __proto)
{
	/* Populate character_data's prototypical functionality */

	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(node);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, character_data);
	DUKKY_SET_CONSTRUCTOR(0, character_data, 1);
	return 1; /* The proto object */
}
