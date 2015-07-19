/* DO NOT USE, DODGY EXAMPLE FOR VINCE */

#include "utils/log.h"

#include <dom/dom.h>

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(html_collection, struct dom_html_collection *coll)
{
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
	priv->coll = coll;
	dom_html_collection_ref(coll);
}

DUKKY_FUNC_FINI(html_collection)
{
	/* do any html_collection finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	dom_html_collection_unref(priv->coll);
}

static DUKKY_FUNC(html_collection, __constructor)
{
	DUKKY_CREATE_PRIVATE(html_collection);
	DUKKY_FUNC_T(html_collection, __init)(ctx, priv,
					      duk_get_pointer(ctx, 1));
	return 1;
}

static DUKKY_FUNC(html_collection, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(html_collection, 0);
	DUKKY_FUNC_T(html_collection, __fini)(ctx, priv);
	free(priv);
	return 0;
}

DUKKY_FUNC(html_collection, __proto)
{
	/* Populate html_collection's prototypical functionality */

	/* Set this prototype's prototype (left-parent)*/
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, html_collection);
	DUKKY_SET_CONSTRUCTOR(0, html_collection, 0);
	return 1; /* The proto object */
}
