/*
 * Copyright 2012 Vincent Sanders <vince@netsurf-browser.org>
 * Copyright 2015 All of us.
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file
 * Duktapeish implementation of javascript engine functions.
 */

#include "content/content.h"
#include "utils/nsoption.h"

#include "javascript/js.h"
#include "javascript/content.h"
#include "utils/log.h"

#include "duktape.h"
#include "dukky.h"

static duk_ret_t dukky_populate_object(duk_context *ctx)
{
	/* ... obj args protoname nargs */
	int nargs = duk_get_int(ctx, -1);
	duk_pop(ctx);
	/* ... obj args protoname */
	duk_get_global_string(ctx, PROTO_MAGIC);
	/* .. obj args protoname prototab */
	duk_insert(ctx, -2);
	/* ... obj args prototab protoname */
	duk_get_prop(ctx, -2);
	/* ... obj args prototab {proto/undefined} */
	if (duk_is_undefined(ctx, -1)) {
		LOG("RuhRoh, couldn't find a prototype, getting htmlelement");
		duk_pop(ctx);
		duk_push_string(ctx, PROTO_NAME(html_unknown_element));
		duk_get_prop(ctx, -2);
	}
	/* ... obj args prototab proto */
	duk_dup(ctx, -1);
	/* ... obj args prototab proto proto */
	duk_set_prototype(ctx, -(nargs+4));
	/* ... obj[proto] args prototab proto */
	duk_get_prop_string(ctx, -1, INIT_MAGIC);
	/* ... obj[proto] args prototab proto initfn */
	duk_insert(ctx, -(nargs+4));
	/* ... initfn obj[proto] args prototab proto */
	duk_pop_2(ctx);
	/* ... initfn obj[proto] args */
	LOG("Call the init function");
	duk_call(ctx, nargs + 1);
	return 1; /* The object */
}

duk_ret_t dukky_create_object(duk_context *ctx, const char *name, int args)
{
	duk_ret_t ret;
	LOG("name=%s nargs=%d", name+2, args);
	/* ... args */
	duk_push_object(ctx);
	/* ... args obj */
	duk_insert(ctx, -(args+1));
	/* ... obj args */
	duk_push_string(ctx, name);
	/* ... obj args name */
	duk_push_int(ctx, args);
	if ((ret = duk_safe_call(ctx, dukky_populate_object, args + 3, 1))
	    != DUK_EXEC_SUCCESS)
		return ret;
	LOG("created");
	return DUK_EXEC_SUCCESS;
}

static duk_ret_t dukky_create_prototype(duk_context *ctx,
					duk_safe_call_function genproto,
					const char *proto_name)
{
	duk_int_t ret;
	duk_push_object(ctx);
	if ((ret = duk_safe_call(ctx, genproto, 1, 1)) != DUK_EXEC_SUCCESS) {
		duk_pop(ctx);
		LOG("Failed to register prototype for %s", proto_name + 2);
		return ret;
	}
	/* top of stack is the ready prototype, inject it */
	duk_put_global_string(ctx, proto_name);
	return 0;
}

/**************************************** js.h ******************************/
struct jscontext {
	duk_context *ctx;
	duk_context *thread;
};

#define CTX (ctx->thread)

void js_initialise(void)
{
	/* NADA for now */
	nsoption_set_bool(enable_javascript, true);
	javascript_init();
}

void js_finalise(void)
{
	/* NADA for now */
}

#define DUKKY_NEW_PROTOTYPE(klass)		\
	dukky_create_prototype(ctx, dukky_##klass##___proto, PROTO_NAME(klass))

jscontext *js_newcontext(int timeout, jscallback *cb, void *cbctx)
{
	duk_context *ctx;
	jscontext *ret = calloc(1, sizeof(*ret));
	LOG("Creating new JS context");
	if (ret == NULL) return NULL;
	ctx = ret->ctx = duk_create_heap_default();
	if (ret->ctx == NULL) { free(ret); return NULL; }
	/* Create the prototype stuffs */
	duk_push_global_object(ctx);
	duk_push_boolean(ctx, true);
	duk_put_prop_string(ctx, -2, "protos");
	duk_put_global_string(ctx, PROTO_MAGIC);
	/* Create prototypes here? */
	DUKKY_NEW_PROTOTYPE(event_target);
	DUKKY_NEW_PROTOTYPE(window);
	DUKKY_NEW_PROTOTYPE(node);
	DUKKY_NEW_PROTOTYPE(document);
	DUKKY_NEW_PROTOTYPE(element);
	DUKKY_NEW_PROTOTYPE(html_element);
	DUKKY_NEW_PROTOTYPE(html_unknown_element);
	return ret;
}

void js_destroycontext(jscontext *ctx)
{
	LOG("Destroying context");
	duk_destroy_heap(ctx->ctx);
	free(ctx);
}

jsobject *js_newcompartment(jscontext *ctx, void *win_priv, void *doc_priv)
{
	/* Pop any active thread off */
	LOG("Yay, new compartment, win_priv=%p, doc_priv=%p", win_priv, doc_priv);
	duk_set_top(ctx->ctx, 0);
	duk_push_thread(ctx->ctx);
	ctx->thread = duk_require_context(ctx->ctx, -1);
	duk_push_int(CTX, 0);
	duk_push_int(CTX, 1);
	duk_push_int(CTX, 2);
	/* Manufacture a Window object */
	/* win_priv is a browser_window, doc_priv is an html content struct */
	duk_push_pointer(CTX, win_priv);
	duk_push_pointer(CTX, doc_priv);
	dukky_create_object(CTX, PROTO_NAME(window), 2);
	duk_push_global_object(CTX);
	duk_put_prop_string(CTX, -2, PROTO_MAGIC);
	duk_set_global_object(CTX);
	
	return (jsobject *)ctx;
}

static duk_ret_t eval_top_string(duk_context *ctx)
{
	duk_eval(ctx);
	return 0;
}

bool js_exec(jscontext *ctx, const char *txt, size_t txtlen)
{
	assert(ctx);
	if (txt == NULL || txtlen == 0) return false;
	duk_set_top(CTX, 0);
	duk_push_lstring(CTX, txt, txtlen);
	LOG("Dumpy");
	DUKKY_DUMP_STACK(CTX);
	if (duk_safe_call(CTX, eval_top_string, 1, 1) == DUK_EXEC_ERROR) {
		LOG("JAVASCRIPT SPLOOF: %s", duk_safe_to_string(CTX, 0));
		return false;
	}
	if (duk_get_top(CTX) == 0) duk_push_boolean(CTX, false);
	LOG("Returning %s", duk_get_boolean(CTX, 0) ? "true" : "false");
	return duk_get_boolean(CTX, 0);
}

bool js_fire_event(jscontext *ctx, const char *type, struct dom_document *doc, struct dom_node *target)
{
	/* La La La */
	LOG("Oh arse, an event: %s", type);
	return true;
}
