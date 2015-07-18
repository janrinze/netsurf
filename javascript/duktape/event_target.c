/* DO NOT USE, DODGY EXAMPLE FOR VINCE */

#include "utils/log.h"

#include <dom/dom.h>

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(event_target)
{
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
}

DUKKY_FUNC_FINI(event_target)
{
	/* do any event_target finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
}

static DUKKY_FUNC(event_target, __constructor)
{
	DUKKY_CREATE_PRIVATE(event_target);
	DUKKY_FUNC_T(event_target, __init)(ctx, priv);
	return 1;
}

static DUKKY_FUNC(event_target, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(event_target, 0);
	DUKKY_FUNC_T(event_target, __fini)(ctx, priv);
	free(priv);
	return 0;
}

DUKKY_FUNC(event_target, __proto)
{
	/* Populate event_target's prototypical functionality */

	/* Set this prototype's prototype (left-parent)*/
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, event_target);
	DUKKY_SET_CONSTRUCTOR(0, event_target, 0);
	return 1; /* The proto object */
}
