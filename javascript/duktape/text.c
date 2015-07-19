/* DO NOT USE, DODGY BIT FOR VINCE */

#include <dom/dom.h>

#include "utils/log.h"

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(text, struct dom_node_text *text)
{
	DUKKY_FUNC_T(character_data, __init)(ctx, &priv->parent, (struct dom_node_character_data *)text);
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
}

DUKKY_FUNC_FINI(text)
{
	/* do any text finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	DUKKY_FUNC_T(character_data, __fini)(ctx, &priv->parent);
}

static DUKKY_FUNC(text, __constructor)
{
	DUKKY_CREATE_PRIVATE(text);
	DUKKY_FUNC_T(text, __init)(ctx, priv,
				   duk_get_pointer(ctx, 1));
	duk_set_top(ctx, 1);
	return 1;
}

static DUKKY_FUNC(text, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(text, 0);
	DUKKY_FUNC_T(text, __fini)(ctx, priv);
	free(priv);
	return 0;
}

DUKKY_FUNC(text, __proto)
{
	/* Populate text's prototypical functionality */

	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(CHARACTERDATA);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, text);
	DUKKY_SET_CONSTRUCTOR(0, text, 1);
	return 1; /* The proto object */
}
