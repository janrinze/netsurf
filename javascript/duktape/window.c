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
	dom_document *doc = priv->htmlc->document;
	dukky_push_node(ctx, (struct dom_node *)doc);
	return 1;
}

static DUKKY_SETTER(window,document)
{
	LOG("BWUAhAHAHAHAHA FUCK OFF");
	return 0;
}

#define STEAL_THING(X)				\
	duk_get_global_string(ctx, #X);		\
	duk_put_prop_string(ctx, 0, #X)

DUKKY_FUNC(window, __proto)
{
	STEAL_THING(undefined);
	/* Populate window's prototypical functionality */
	DUKKY_POPULATE_FULL_PROPERTY(window, document);
	/* Exposed prototypes */
	DUKKY_GET_PROTOTYPE(NODE);
	duk_put_prop_string(ctx, 0, "Node");
	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(EVENTTARGET);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, window);
	DUKKY_SET_CONSTRUCTOR(0, window, 2);
	return 1; /* The proto object */
}
