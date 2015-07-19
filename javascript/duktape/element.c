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

static DUKKY_GETTER(element, firstElementChild)
{
	DUKKY_GET_METHOD_PRIVATE(element);
	dom_node *element;
	dom_exception exc;
	dom_node_type node_type;
	dom_node *next_node;

	exc = dom_node_get_first_child(((node_private_t*)priv)->node, &element);
	if (exc != DOM_NO_ERR) {
		return 0;
	}

	while (element != NULL) {
		exc = dom_node_get_node_type(element, &node_type);
		if ((exc == DOM_NO_ERR) && (node_type == DOM_ELEMENT_NODE)) {
			/* found it */
			dukky_push_node(ctx, (dom_node *)element);
			dom_node_unref(element);
			return 1;
		}

		exc = dom_node_get_next_sibling(element, &next_node);
		dom_node_unref(element);
		if (exc == DOM_NO_ERR) {
			element = next_node;
		} else {
			element = NULL;
		}
	}
	return 0;
}

static DUKKY_GETTER(element, lastElementChild)
{
	DUKKY_GET_METHOD_PRIVATE(element);
	dom_node *element;
	dom_exception exc;
	dom_node_type node_type;
	dom_node *next_node;

	exc = dom_node_get_last_child(((node_private_t*)priv)->node, &element);
	if (exc != DOM_NO_ERR) {
		return 0;
	}

	while (element != NULL) {
		exc = dom_node_get_node_type(element, &node_type);
		if ((exc == DOM_NO_ERR) && (node_type == DOM_ELEMENT_NODE)) {
			/* found it */
			dukky_push_node(ctx, (dom_node *)element);
			dom_node_unref(element);
			return 1;
		}

		exc = dom_node_get_previous_sibling(element, &next_node);
		dom_node_unref(element);
		if (exc == DOM_NO_ERR) {
			element = next_node;
		} else {
			element = NULL;
		}
	}
	return 0;
}

static DUKKY_GETTER(element, previousElementSibling)
{
	DUKKY_GET_METHOD_PRIVATE(element);
	dom_node *element;
	dom_exception exc;
	dom_node_type node_type;
	dom_node *sib_node;

	exc = dom_node_get_previous_sibling(((node_private_t *)priv)->node, &element);
	if (exc != DOM_NO_ERR) {
		return 0;
	}

	while (element != NULL) {
		exc = dom_node_get_node_type(element, &node_type);
		if ((exc == DOM_NO_ERR) && (node_type == DOM_ELEMENT_NODE)) {
			/* found it */
			dukky_push_node(ctx, (dom_node *)element);
			dom_node_unref(element);
			return 1;
		}

		exc = dom_node_get_previous_sibling(element, &sib_node);
		dom_node_unref(element);
		if (exc == DOM_NO_ERR) {
			element = sib_node;
		} else {
			element = NULL;
		}
	}
	return 0;
}

static DUKKY_GETTER(element, nextElementSibling)
{
	DUKKY_GET_METHOD_PRIVATE(element);
	dom_node *element;
	dom_exception exc;
	dom_node_type node_type;
	dom_node *sib_node;

	exc = dom_node_get_next_sibling(((node_private_t *)priv)->node, &element);
	if (exc != DOM_NO_ERR) {
		return 0;
	}

	while (element != NULL) {
		exc = dom_node_get_node_type(element, &node_type);
		if ((exc == DOM_NO_ERR) && (node_type == DOM_ELEMENT_NODE)) {
			/* found it */
			dukky_push_node(ctx, (dom_node *)element);
			dom_node_unref(element);
			return 1;
		}

		exc = dom_node_get_next_sibling(element, &sib_node);
		dom_node_unref(element);
		if (exc == DOM_NO_ERR) {
			element = sib_node;
		} else {
			element = NULL;
		}
	}
	return 0;
}

static DUKKY_GETTER(element, childElementCount)
{
	LOG("MOO");
	DUKKY_GET_METHOD_PRIVATE(element);
	dom_node *element;
	dom_exception exc;
	dom_node_type node_type;
	dom_node *next_node;
	duk_uint_t jsret = 0;

	exc = dom_node_get_first_child(((node_private_t *)priv)->node, &element);
	if (exc != DOM_NO_ERR) {
		return 0;
	}

	while (element != NULL) {
		exc = dom_node_get_node_type(element, &node_type);
		if ((exc == DOM_NO_ERR) && (node_type == DOM_ELEMENT_NODE)) {
			jsret += 1;
		}

		exc = dom_node_get_next_sibling(element, &next_node);
		dom_node_unref(element);
		if (exc == DOM_NO_ERR) {
			element = next_node;
		} else {
			element = NULL;
		}
	}
	LOG("I found %u of them", jsret);
	duk_push_uint(ctx, jsret);
	return 1;
}

DUKKY_FUNC(element, __proto)
{
	/* Populate element's prototypical functionality */
	DUKKY_POPULATE_READONLY_PROPERTY(element, firstElementChild);
	DUKKY_POPULATE_READONLY_PROPERTY(element, lastElementChild);
	DUKKY_POPULATE_READONLY_PROPERTY(element, nextElementSibling);
	DUKKY_POPULATE_READONLY_PROPERTY(element, previousElementSibling);
	DUKKY_POPULATE_READONLY_PROPERTY(element, childElementCount);
	/* Set this prototype's prototype (left-parent)*/
	DUKKY_GET_PROTOTYPE(node);
	duk_set_prototype(ctx, 0);
	/* And the initialiser/finalizer */
	DUKKY_SET_DESTRUCTOR(0, element);
	DUKKY_SET_CONSTRUCTOR(0, element, 1);
	return 1; /* The proto object */
}
