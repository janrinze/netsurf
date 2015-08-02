/* DO NOT USE, DODGY BIT FOR VINCE */

#ifndef DUKKY_H
#define DUKKY_H


#define DUKKY_FUNC_T(t,e) dukky_##t##_##e
#define DUKKY_FUNC(t,e) duk_ret_t DUKKY_FUNC_T(t,e) (duk_context *ctx)

static inline void *dukky_get_private(duk_context *ctx, int idx)
{
	void *ret;
	duk_get_prop_string(ctx, idx, PRIVATE_MAGIC);
	ret = duk_get_pointer(ctx, -1);
	duk_pop(ctx);
	return ret;
}

#define DUKKY_CREATE_PRIVATE(klass)		\
	klass##_private_t *priv = calloc(1, sizeof(*priv));	\
	if (priv == NULL) return 0;				\
	duk_push_pointer(ctx, priv);				\
	duk_put_prop_string(ctx, 0, PRIVATE_MAGIC)

#define DUKKY_SET_DESTRUCTOR(idx,n)		\
	duk_dup(ctx, idx);						\
	duk_push_c_function(ctx, DUKKY_FUNC_T(n,__destructor), 1);	\
	duk_set_finalizer(ctx, -2);		\
	duk_pop(ctx);

#define DUKKY_SET_CONSTRUCTOR(idx,n,a)			\
	duk_dup(ctx, idx);			\
	duk_push_c_function(ctx, DUKKY_FUNC_T(n,__constructor), 1 + a);	\
	duk_put_prop_string(ctx, -2, INIT_MAGIC);	\
	duk_pop(ctx);

#define DUKKY_SAFE_GET_PRIVATE(t,idx)		\
	t##_private_t *priv = dukky_get_private(ctx, idx);	\
	if (priv == NULL) return 0; /* No can do */

#define DUKKY_SAFE_GET_ANOTHER(n,t,idx)				\
	t##_private_t *n = dukky_get_private(ctx, idx);	\
	if (priv == NULL) return 0; /* No can do */

#define DUKKY_GET_METHOD_PRIVATE(t)		\
	t##_private_t *priv = NULL;			\
	duk_push_this(ctx);				\
	duk_get_prop_string(ctx, -1, PRIVATE_MAGIC);	\
	priv = duk_get_pointer(ctx, -1);		\
	duk_pop_2(ctx);					\
	if (priv == NULL) return 0; /* No can do */

#define DUKKY_GET_PROTOTYPE(klass)			\
	duk_get_global_string(ctx, PROTO_MAGIC);	\
	duk_get_prop_string(ctx, -1, PROTO_NAME(klass));	\
	duk_replace(ctx, -2)

#define DUKKY_DECLARE_PROTOTYPE(klass)		\
	DUKKY_FUNC(klass,__proto)

#define DUKKY_GETTER(klass,prop)		\
	duk_ret_t DUKKY_FUNC_T(klass,prop##_getter)(duk_context *ctx)

#define DUKKY_SETTER(klass,prop)		\
	duk_ret_t DUKKY_FUNC_T(klass,prop##_setter)(duk_context *ctx)

#define DUKKY_POPULATE_FULL_PROPERTY(klass,prop)	\
	duk_dup(ctx, 0);			\
	duk_push_string(ctx, #prop);		\
	duk_push_c_function(ctx, DUKKY_FUNC_T(klass,prop##_getter), 0); \
	duk_push_c_function(ctx, DUKKY_FUNC_T(klass,prop##_setter), 1); \
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER |			\
		     DUK_DEFPROP_HAVE_SETTER |				\
		     DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE | \
		     DUK_DEFPROP_HAVE_CONFIGURABLE);			\
	duk_pop(ctx)

#define DUKKY_POPULATE_READONLY_PROPERTY(klass,prop)	\
	duk_dup(ctx, 0);			\
	duk_push_string(ctx, #prop);		\
	duk_push_c_function(ctx, DUKKY_FUNC_T(klass,prop##_getter), 0); \
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER |			\
		     DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE | \
		     DUK_DEFPROP_HAVE_CONFIGURABLE);			\
	duk_pop(ctx)

#define DUKKY_DUMP_STACK(ctx)				\
	do {						\
		duk_push_context_dump(ctx);		\
		LOG("Stack: %s", duk_to_string(ctx, -1));	\
		duk_pop(ctx);					\
	} while(0)

#define DUKKY_ADD_METHOD(klass,meth,nargs)	\
	duk_dup(ctx, 0);			\
	duk_push_string(ctx, #meth);		\
	duk_push_c_function(ctx, DUKKY_FUNC_T(klass,meth), nargs);	\
	DUKKY_DUMP_STACK(ctx);\
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE |			\
		     DUK_DEFPROP_HAVE_WRITABLE |			\
		     DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE | \
		     DUK_DEFPROP_HAVE_CONFIGURABLE);			\
	duk_pop(ctx)

#define DUKKY_FUNC_INIT(klass,args...)	\
	void DUKKY_FUNC_T(klass, __init)(duk_context *ctx, klass##_private_t *priv, ##args)

#define DUKKY_FUNC_FINI(klass)			\
	void DUKKY_FUNC_T(klass, __fini)(duk_context *ctx, klass##_private_t *priv)

#define DUKKY_DECLARE_INTERFACE(klass,init...)		\
	DUKKY_FUNC(klass, __proto);			\
	DUKKY_FUNC_INIT(klass, ##init);	\
	DUKKY_FUNC_FINI(klass)


duk_ret_t dukky_create_object(duk_context *ctx, const char *name, int args);
duk_bool_t dukky_push_node_stacked(duk_context *ctx);
duk_bool_t dukky_push_node(duk_context *ctx, struct dom_node *node);


#endif
