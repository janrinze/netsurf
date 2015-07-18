/* DO NOT USE, DODGY BIT FOR VINCE */

#include <dom/dom.h>

#include "utils/log.h"

#include "javascript/dukky.h"

#include "desktop/browser.h"
#include "render/html.h"
#include "render/html_internal.h"
#include "utils/nsurl.h"

DUKKY_FUNC_INIT(window, struct browser_window *win,
		struct html_content *htmlc)
{
	DUKKY_FUNC_T(event_target, __init)(ctx, &priv->parent);
	LOG("Initialise %p (priv=%p)", duk_get_heapptr(ctx, 0), priv);
	/* element window */
	priv->win = win;
	priv->htmlc = htmlc;
	LOG("win=%p htmlc=%p", priv->win, priv->htmlc);
	
	LOG("URL is %s", nsurl_access(browser_window_get_url(priv->win)));
	
	/* populate window.window */
	duk_dup(ctx, 0);
	duk_put_prop_string(ctx, 0, "window");
}

DUKKY_FUNC_FINI(window)
{
	/* do any window finalisation here, priv ptr exists */
	LOG("Finalise %p", duk_get_heapptr(ctx, 0));
	DUKKY_FUNC_T(event_target, __fini)(ctx, &priv->parent);
}

static DUKKY_FUNC(window, __constructor)
{
	DUKKY_CREATE_PRIVATE(window);
	DUKKY_FUNC_T(window, __init)(ctx, priv,
				     duk_get_pointer(ctx, 1),
				     duk_get_pointer(ctx, 2));
	duk_set_top(ctx, 1);
	return 1;
}

static DUKKY_FUNC(window, __destructor)
{
	DUKKY_SAFE_GET_PRIVATE(window, 0);
	DUKKY_FUNC_T(window, __fini)(ctx, priv);
	free(priv);
	return 0;
}

static DUKKY_GETTER(window,document)
{
	DUKKY_GET_METHOD_PRIVATE(window);
	LOG("priv=%p", priv);
	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, PROP_NAME(window, document));
	if (!duk_is_undefined(ctx, -1)) {
		return 1;
	} else {
		duk_pop(ctx);
	}
	dom_document *doc = priv->htmlc->document;
	duk_push_pointer(ctx, doc);
	if (dukky_create_object(ctx, PROTO_NAME(document), 1) != DUK_EXEC_SUCCESS) {
		LOG("ERROR");
	}
	duk_push_this(ctx);
	duk_dup(ctx, -2);
	duk_put_prop_string(ctx, -2, PROP_NAME(window, document));
	duk_pop(ctx);
	return 1;
}

static DUKKY_SETTER(window,document)
{
	LOG("BWUAhAHAHAHAHA FUCK OFF");
	return 0;
}

DUKKY_FUNC(window, __proto)
{
	/* Populate window's prototypical functionality */
	DUKKY_POPULATE_FULL_PROPERTY(window, document);
	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(event_target);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, window);
	DUKKY_SET_CONSTRUCTOR(0, window, 2);
	return 1; /* The proto object */
}
