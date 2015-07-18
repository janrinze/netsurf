/* DO NOT USE, DODGY BIT FOR VINCE */

#include <dom/dom.h>

#include "utils/log.h"

#include "javascript/dukky.h"

DUKKY_FUNC_INIT(html_unknown_element, struct dom_html_element *html_unknown_element)
{
	DUKKY_FUNC_T(html_element, __init)(ctx, &priv->parent, html_unknown_element);
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
}

DUKKY_FUNC_FINI(html_unknown_element)
{
	/* do any html_unknown_element finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	DUKKY_FUNC_T(html_element, __fini)(ctx, &priv->parent);
}

static DUKKY_FUNC(html_unknown_element, __constructor)
{
	DUKKY_CREATE_PRIVATE(html_unknown_element);
	DUKKY_FUNC_T(html_unknown_element, __init)(ctx, priv,
						   duk_get_pointer(ctx, 1));
	duk_set_top(ctx, 1);
	return 1;
}

static DUKKY_FUNC(html_unknown_element, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(html_unknown_element, 0);
	DUKKY_FUNC_T(html_unknown_element, __fini)(ctx, priv);
	free(priv);
	return 0;
}

DUKKY_FUNC(html_unknown_element, __proto)
{
	/* Populate html_unknown_element's prototypical functionality */

	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(html_element);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, html_unknown_element);
	DUKKY_SET_CONSTRUCTOR(0, html_unknown_element, 1);
	return 1; /* The proto object */
}
