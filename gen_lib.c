#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#define VECTOR_MIN_SIZE 2
#define VECTOR_MAX_SIZE 4
static_assert(VECTOR_MIN_SIZE <= VECTOR_MAX_SIZE, "Max vector size may not be less than the min vector size, c'mon");

char *op_arg_names[] = {"a", "b"};
#define OP_ARITY (sizeof(op_arg_names) / sizeof(op_arg_names[0]))

typedef struct {
	char cstr[128];
} Short_String;

typedef struct {
	const char *name;
	const char *suffix;
	const char *fmt;
	const char *zero_lit;
} Type_Def;

typedef enum {
	TYPE_DEF_FLOAT = 0,
	TYPE_DEF_DOUBLE,
	TYPE_DEF_INT,
	COUNT_TYPE_DEFS,
} Type_Def_Type;

typedef struct {
	const char *suffix;
	const char *op;
} Op_Def;

typedef enum {
	OP_DEF_SUM = 0,
	OP_DEF_SUB,
	OP_DEF_MUL,
	OP_DEF_DIV,
	COUNT_OP_DEFS,
} Op_Def_Type;

#define FUN_DEF_MAX_ARITY 5

typedef struct {
	const char *suffix;
	// NULL -> function not supported for type
	const char *name_for_type[COUNT_TYPE_DEFS];
	size_t arity;
	char *args[FUN_DEF_MAX_ARITY];
} Fun_Def;

typedef enum {
	FUN_SQRT = 0,
	FUN_POW,
	FUN_SIN,
	FUN_COS,
	FUN_MIN,
	FUN_MAX,
	FUN_LERP,
	FUN_FLOOR,
	FUN_CEIL,
	FUN_CLAMP,
	COUNT_FUN_DEFS,
} Fun_Def_Type;

static Type_Def type_defs[COUNT_TYPE_DEFS] = {
	[TYPE_DEF_FLOAT] 	= {.name = "float", .suffix = "f", .fmt = "f", .zero_lit = "0.0f"},
	[TYPE_DEF_DOUBLE] 	= {.name = "double", .suffix = "d", .fmt = "lf", .zero_lit = "0.0"},
	[TYPE_DEF_INT] 		= {.name = "int", .suffix = "i", .fmt = "d", .zero_lit = "0"},
};

static Op_Def op_defs[COUNT_OP_DEFS] = {
	[OP_DEF_SUM] = {.suffix = "sum", .op = "+="},
	[OP_DEF_SUB] = {.suffix = "sub", .op = "-="},
	[OP_DEF_MUL] = {.suffix = "mul", .op = "*="},
	[OP_DEF_DIV] = {.suffix = "div", .op = "/="},
};

static_assert(COUNT_FUN_DEFS == 10, "More functions are been introduced please update the array below");
static_assert(COUNT_TYPE_DEFS == 3, "More types are been introduced please update the array below");
static Fun_Def fun_defs[COUNT_FUN_DEFS] = {
	[FUN_SQRT] = {
		.suffix = "sqrt",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "sqrtf",
			[TYPE_DEF_DOUBLE] = "sqrt",
		},
		.arity = 1,
		.args = {"a"},
	},
	[FUN_POW] = {
		.suffix = "pow",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "powf",
			[TYPE_DEF_DOUBLE] = "pow",
		},
		.arity = 2,
		.args = {"base", "exp"},
	},
	[FUN_SIN] = {
		.suffix = "sin",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "sinf",
			[TYPE_DEF_DOUBLE] = "sin",
		},
		.arity = 1,
		.args = {"a"},
	},
	[FUN_COS] = {
		.suffix = "cos",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "cosf",
			[TYPE_DEF_DOUBLE] = "cos",
		},
		.arity = 1,
		.args = {"a"},
	},
	[FUN_MIN] = {
		.suffix = "min",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "fminf",
			[TYPE_DEF_DOUBLE] = "fmin",
			[TYPE_DEF_INT] = "mini",
		},
		.arity = 2,
		.args = {"a", "b"},
	},
	[FUN_MAX] = {
		.suffix = "max",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "fmaxf",
			[TYPE_DEF_DOUBLE] = "fmax",
			[TYPE_DEF_INT] = "maxi",
		},
		.arity = 2,
		.args = {"a", "b"},
	},
	[FUN_LERP] = {
		.suffix = "lerp",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "lerpf",
			[TYPE_DEF_DOUBLE] = "lerp",
		},
		.arity = 3,
		.args = {"a", "b", "t"},
	},
	[FUN_FLOOR] = {
		.suffix = "floor",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "floorf",
			[TYPE_DEF_DOUBLE] = "floor",
		},
		.arity = 1,
		.args = {"a"},
	},
	[FUN_CEIL] = {
		.suffix = "ceil",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "ceilf",
			[TYPE_DEF_DOUBLE] = "ceil",
		},
		.arity = 1,
		.args = {"a"},
	},
	[FUN_CLAMP] = {
		.suffix = "clamp",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "clampf",
			[TYPE_DEF_DOUBLE] = "clampd",
			[TYPE_DEF_INT] = "clampi",
		},
		.arity = 3,
		.args = {"x", "a", "b"}
	},
};

#if defined(__GNUC__) || defined(__clang__)
#define CHECK_PRINTF_FMT(a, b) __attribute__ ((format (printf, a, b)))
#else
#define CHECK_PRINTF_FMT(...)
#endif

CHECK_PRINTF_FMT(1, 2) Short_String shortf(const char* fmt, ...){
	Short_String result = {0};

	va_list args;
	va_start(args, fmt);
	int n = vsnprintf(result.cstr, sizeof(result.cstr), fmt, args);
	assert(n >= 0);
		assert((size_t) n + 1 <= sizeof(result.cstr));
	va_end(args);

	return result;
}

static_assert(VECTOR_MAX_SIZE == 4, "We defined only 4 vector component names. Please update this list accordingly");
static char *vector_comps[VECTOR_MAX_SIZE] = {"x", "y", "z", "w"};

void gen_vector_def(FILE* stream, size_t n, Type_Def type_def){
	Short_String vector_type = shortf("V%zu%s", n ,type_def.suffix);
	fprintf(stream, "typedef struct { %s ", type_def.name);
	assert(n <= VECTOR_MAX_SIZE);
	for(size_t i = 0; i < n; ++i) {
		if(i > 0) fprintf(stream, ", ");
		fprintf(stream, "%s", vector_comps[i]);
	}
	fprintf(stream, "; } %s;\n", vector_type.cstr);
}

// Generates function signatures of the following form:
// ret_type name(arg_type arg_names[0], arg_type arg_names[1], ..., arg_type arg_names[arity - 1])
// All arguments have the same type.
void gen_func_sig(FILE *stream, const char *ret_type, const char *name, const char *arg_type, char **arg_names, size_t arity){
	fprintf(stream, "LADEF %s %s(", ret_type, name);
	if(arity > 0) fprintf(stream, "%s %s", arg_type, arg_names[0]);
	for(size_t arg_index = 1; arg_index < arity; ++arg_index) {
		fprintf(stream, ", ");
		fprintf(stream, "%s %s", arg_type, arg_names[arg_index]);
	}
	fprintf(stream, ")");
}

void gen_vector_op_sig(FILE *stream, size_t n, Type_Def type_def, Op_Def op_def){
	Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
	Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);
	Short_String name = shortf("%s_%s", vector_prefix.cstr, op_def.suffix);
	gen_func_sig(stream, vector_type.cstr, name.cstr, vector_type.cstr, op_arg_names, OP_ARITY);
}

void gen_vector_ctor_sig(FILE* stream, size_t n, Type_Def type_def){
	Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
	Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);
	assert(n <= VECTOR_MAX_SIZE);
	gen_func_sig(stream, vector_type.cstr, vector_prefix.cstr, type_def.name, vector_comps, n);
}

void gen_vector_ctor_decl(FILE *stream, size_t n, Type_Def type_def){
	gen_vector_ctor_sig(stream, n, type_def);
	fprintf(stream, ";\n");
}

void gen_vector_ctor_impl(FILE *stream, size_t n, Type_Def type_def){
	Short_String type = shortf("V%zu%s", n, type_def.suffix);

	gen_vector_ctor_sig(stream, n, type_def);
	fprintf(stream, "\n");
	fprintf(stream, "{\n");
	fprintf(stream, "\t%s v;\n", type.cstr);
	assert(n <= VECTOR_MAX_SIZE);
	for(size_t i = 0; i < n; ++i) {
		fprintf(stream, "\tv.%s = %s;\n", vector_comps[i], vector_comps[i]);
	}
	fprintf(stream, "\treturn v;\n");
	fprintf(stream, "}\n");
}

void gen_vector_scalar_ctor_sig(FILE* stream, size_t n, Type_Def type_def){
	Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
	Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);
	Short_String name = shortf("%s%s", vector_prefix.cstr, type_def.suffix);
	static_assert(VECTOR_MAX_SIZE >= 1, "The vector size is too short for this code");
	gen_func_sig(stream, vector_type.cstr, name.cstr, type_def.name, vector_comps, 1);
}

void gen_vector_op_decl(FILE* stream, size_t n, Type_Def type_def, Op_Def op_def){
	gen_vector_op_sig(stream, n, type_def, op_def);
	fprintf(stream, ";\n");
}

void gen_vector_op_impl(FILE* stream, size_t n, Type_Def type_def, Op_Def op_def){
	gen_vector_op_sig(stream, n, type_def, op_def);
	fprintf(stream, "\n{\n");
	assert(n <= VECTOR_MAX_SIZE);
	static_assert(OP_ARITY >= 2, "This code assumes that operation's arity is at least 2");
	for(size_t i = 0; i < n; ++i) {
		fprintf(stream, "\t%s.%s %s %s.%s;\n", op_arg_names[0], vector_comps[i], op_def.op, op_arg_names[1], vector_comps[i]);
	}
	fprintf(stream, "\treturn %s;\n", op_arg_names[0]);
	fprintf(stream, "}\n");
}

void gen_vector_scalar_ctor_decl(FILE* stream, size_t n, Type_Def type_def){
	gen_vector_scalar_ctor_sig(stream, n, type_def);
	fprintf(stream, ";\n");
}

void gen_vector_scalar_ctor_impl(FILE* stream, size_t n, Type_Def type_def){
	Short_String prefix = shortf("v%zu%s", n, type_def.suffix);

	gen_vector_scalar_ctor_sig(stream, n, type_def);
	fprintf(stream, "\n{\n");
	fprintf(stream, "\treturn %s(", prefix.cstr);
	for(size_t i = 0; i < n; ++i){
		if(i > 0) fprintf(stream, ", ");
		static_assert(VECTOR_MAX_SIZE >= 1, "The vector size is too short for this code");
		fprintf(stream, "%s", vector_comps[0]);
	}
	fprintf(stream, ");\n");
	fprintf(stream, "}\n");
}

void gen_vector_fun_sig(FILE *stream, size_t n, Type_Def_Type type, Fun_Def_Type fun){
	Fun_Def fun_def = fun_defs[fun];
	Type_Def type_def = type_defs[type];
	Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
	Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);
	Short_String name = shortf("%s_%s", vector_prefix.cstr, fun_def.suffix);
	gen_func_sig(stream, vector_type.cstr, name.cstr, vector_type.cstr, fun_def.args, fun_def.arity);
}

void gen_vector_fun_decl(FILE *stream, size_t n, Type_Def_Type type, Fun_Def_Type fun){
	gen_vector_fun_sig(stream, n, type, fun);
	fprintf(stream, ";\n");
}

void gen_vector_fun_impl(FILE *stream, size_t n, Type_Def_Type type, Fun_Def_Type fun){
	gen_vector_fun_sig(stream, n, type, fun);
	fprintf(stream, "\n{\n");
	Fun_Def fun_def = fun_defs[fun];
	assert(fun_def.name_for_type[type]);
	assert(fun_def.arity >= 1);
	assert(n <= VECTOR_MAX_SIZE);
	for(size_t i = 0; i < n; ++i) {
		fprintf(stream, "\t%s.%s = %s(", fun_def.args[0], vector_comps[i], fun_def.name_for_type[type]);
		for(size_t arg_num = 0; arg_num < fun_def.arity; ++arg_num) {
			if(arg_num > 0) fprintf(stream, ", ");
			fprintf(stream, "%s.%s", fun_def.args[arg_num], vector_comps[i]);
		}
		fprintf(stream, ");\n");
	}
	fprintf(stream, "\treturn %s;\n", fun_def.args[0]);
	fprintf(stream, "}\n");
}

#define LERP_ARITY 3
static char *lerp_args[LERP_ARITY] = {"a", "b", "t"};

void gen_lerp_sig(FILE *stream, const char* name, const char *type){
	 gen_func_sig(stream, type, name, type, lerp_args, LERP_ARITY);
}

void gen_lerp_decl(FILE *stream, const char* name, const char *type){
	gen_lerp_sig(stream, name, type);
	fprintf(stream, ";\n");
}

void gen_lerp_impl(FILE *stream, const char* name, const char *type){
	gen_lerp_sig(stream, name, type);
	fprintf(stream, "\n{\n");
	char *a = lerp_args[0];
	char *b = lerp_args[1];
	char *t = lerp_args[2];
	fprintf(stream, "\treturn %s + (%s - %s) * %s;\n", a, b, a, t);
	fprintf(stream, "}\n");

}

char *minmax_args[] = {"a", "b"};
#define MINMAX_ARITY (sizeof(minmax_args) / sizeof(minmax_args[0]))

void gen_minmax_sig(FILE *stream, const char *name, const char *type)
{
	gen_func_sig(stream, type, name, type, minmax_args, MINMAX_ARITY);
}

void gen_minmax_decl(FILE *stream, const char *name, const char *type)
{
	gen_minmax_sig(stream, name, type);
	fprintf(stream, ";\n");
}

void gen_min_impl(FILE *stream, const char *name, const char *type)
{
	gen_minmax_sig(stream, name, type);
	fprintf(stream, "\n");
	fprintf(stream, "{\n");
	static_assert(MINMAX_ARITY == 2, "Unexpected arity of min/max functions");
	fprintf(stream, "\treturn %s < %s ? %s : %s;\n", minmax_args[0], minmax_args[1], minmax_args[0], minmax_args[1]);
	fprintf(stream, "}\n");
}

void gen_max_impl(FILE *stream, const char *name, const char *type)
{
	gen_minmax_sig(stream, name, type);
	fprintf(stream, "\n");
	fprintf(stream, "{\n");
	static_assert(MINMAX_ARITY == 2, "Unexpected arity of min/max functions");
	fprintf(stream, "\treturn %s < %s ? %s : %s;\n", minmax_args[0], minmax_args[1], minmax_args[1], minmax_args[0]);
	fprintf(stream, "}\n");
}

char *clamp_args[] = {"x", "a", "b"};
#define CLAMP_ARITY (sizeof(clamp_args) / sizeof(clamp_args[0]))

void gen_clamp_sig(FILE *stream, Type_Def type_def){
	Short_String name = shortf("clamp%s", type_def.suffix);
	gen_func_sig(stream, type_def.name, name.cstr, type_def.name, clamp_args, CLAMP_ARITY);
}

void gen_clamp_decl(FILE *stream, Type_Def type_def){
	gen_clamp_sig(stream, type_def);
	fprintf(stream, ";\n");
}

void gen_clamp_impl(FILE *stream, Type_Def_Type type, Fun_Def min_def, Fun_Def max_def){
	Type_Def type_def = type_defs[type];
	const char *min_name = min_def.name_for_type[type];
	assert(min_name != NULL);
	const char *max_name = max_def.name_for_type[type];
	assert(max_name != NULL);

	gen_clamp_sig(stream, type_def);
	fprintf(stream, "\n");
	fprintf(stream, "{\n");
	static_assert(CLAMP_ARITY == 3, "Unexpected clamp arity");
	fprintf(stream, "\treturn %s(%s(%s, %s), %s);\n", min_name, max_name, clamp_args[1], clamp_args[0], clamp_args[2]);
	fprintf(stream, "}\n");
}

static char *sqrlen_arg_name = "a";

void gen_vector_sqrlen_sig(FILE *stream, size_t n, Type_Def type_def){
	Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
	Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);
	Short_String name = shortf("%s_sqrlen", vector_prefix.cstr);
	gen_func_sig(stream, type_def.name, name.cstr, vector_type.cstr, &sqrlen_arg_name, 1);
}

void gen_vector_sqrlen_decl(FILE *stream, size_t n, Type_Def type_def){
	gen_vector_sqrlen_sig(stream, n, type_def);
	fprintf(stream, ";\n");
}

void gen_vector_sqrlen_impl(FILE *stream, size_t n, Type_Def type_def){
	gen_vector_sqrlen_sig(stream, n, type_def);
	fprintf(stream, "\n");
	fprintf(stream, "{\n");
	fprintf(stream, "\treturn ");
	assert(n <= VECTOR_MAX_SIZE);
	for(size_t i = 0; i < n; ++i) {
		if(i > 0) fprintf(stream, " + ");
		fprintf(stream, "%s.%s*%s.%s", sqrlen_arg_name, vector_comps[i], sqrlen_arg_name, vector_comps[i]);
	}
	fprintf(stream, ";\n");
	fprintf(stream, "}\n");
}

void gen_vector_printf_macros(FILE *stream, size_t n, Type_Def type_def){
	Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
	Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);

	fprintf(stream, "#define %s_Fmt \"%s(", vector_type.cstr, vector_prefix.cstr);
	for(size_t i = 0; i < n; ++i) {
		if(i > 0) fprintf(stream, ", ");
		fprintf(stream, "%%%s", type_def.fmt);
	}
	fprintf(stream, ")\"\n");

	fprintf(stream, "#define %s_Arg(v) ", vector_type.cstr);
	assert(n <= VECTOR_MAX_SIZE);
	for(size_t i = 0; i < n; ++i) {
		if(i > 0) fprintf(stream, ", ");
		fprintf(stream, "(v).%s", vector_comps[i]);
	}
	fprintf(stream, "\n");
}

static_assert(COUNT_TYPE_DEFS == 3, "Amount of types have changed");
const char *funcs_sqrt_defined_for[COUNT_TYPE_DEFS] = {
	[TYPE_DEF_FLOAT] = "sqrtf",
	[TYPE_DEF_DOUBLE] = "sqrt",
};

void gen_vector_len_sig(FILE *stream, size_t n, Type_Def type_def){
	Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
	Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);
	Short_String func_name = shortf("%s_len", vector_prefix.cstr);
	gen_func_sig(stream, type_def.name, func_name.cstr, vector_type.cstr, &sqrlen_arg_name, 1);
}

void gen_vector_len_decl(FILE *stream, size_t n, Type_Def type_def){
	gen_vector_len_sig(stream, n, type_def);
	fprintf(stream, ";\n");
}

void gen_vector_len_impl(FILE *stream, size_t n, Type_Def type_def, const char *sqrt_name){
	Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);
	Short_String sqrlen_name = shortf("%s_sqrlen", vector_prefix.cstr);

	gen_vector_len_sig(stream, n, type_def);
	fprintf(stream, "\n");
	fprintf(stream, "{\n");
	fprintf(stream, "\treturn %s(%s(%s));\n", sqrt_name, sqrlen_name.cstr, sqrlen_arg_name);
	fprintf(stream, "}\n");
}

char *vector_convert_arg = "a";

void gen_vector_convert_sig(FILE *stream, size_t dst_n, Type_Def dst_type_def, size_t src_n, Type_Def src_type_def){
	Short_String dst_type = shortf("V%zu%s", dst_n, dst_type_def.suffix);
	Short_String src_type = shortf("V%zu%s", src_n, src_type_def.suffix);
	Short_String name = shortf("v%zu%s%zu%s", dst_n, dst_type_def.suffix, src_n, src_type_def.suffix);
	gen_func_sig(stream, dst_type.cstr, name.cstr, src_type.cstr, &vector_convert_arg, 1);
}

void gen_vector_convert_decl(FILE *stream, size_t dst_n, Type_Def dst_type_def, size_t src_n, Type_Def src_type_def){
	gen_vector_convert_sig(stream, dst_n, dst_type_def, src_n, src_type_def);
	fprintf(stream, ";\n");
}

void gen_vector_convert_impl(FILE *stream, size_t dst_n, Type_Def dst_type_def, size_t src_n, Type_Def src_type_def){
	Short_String dst_type = shortf("V%zu%s", dst_n, dst_type_def.suffix);

	gen_vector_convert_sig(stream, dst_n, dst_type_def, src_n, src_type_def);
	fprintf(stream, "\n");
	fprintf(stream, "{\n");
	fprintf(stream, "\t%s result;\n", dst_type.cstr);
	assert(dst_n <= VECTOR_MAX_SIZE);
	for(size_t i = 0; i < dst_n; ++i) {
		if(i < src_n) {
			fprintf(stream, "\tresult.%s = (%s) %s.%s;\n", vector_comps[i], dst_type_def.name, vector_convert_arg, vector_comps[i]);
		} else {
			fprintf(stream, "\tresult.%s = %s;\n", vector_comps[i], dst_type_def.zero_lit);
		}
	}
	fprintf(stream, "\treturn result;\n");
	fprintf(stream, "}\n");
}

// TODO: matrices
// TODO: documentation

int main(int argc, char **argv){

	(void) argc;
	(void) argv;

	FILE *stream = stdout;

	// HEADER
	{
		fprintf(stream, "#ifndef LA_H_\n");
		fprintf(stream, "#define LA_H_\n");
		fprintf(stream, "\n");
		fprintf(stream, "#include <stdlib.h>\n");
		fprintf(stream, "#include <math.h>\n");
		fprintf(stream, "\n");
		fprintf(stream, "#ifndef LADEF\n");
		fprintf(stream, "#define LADEF static inline\n");
		fprintf(stream, "#endif // LADEF\n");
		fprintf(stream, "\n");

		gen_lerp_decl(stream, "lerpf", "float");
		gen_lerp_decl(stream, "lerp", "double");
		gen_minmax_decl(stream, "mini", "int");
		gen_minmax_decl(stream, "maxi", "int");
		for(Type_Def_Type type = 0; type < COUNT_TYPE_DEFS; ++type) {
			gen_clamp_decl(stream, type_defs[type]);
		}
		fprintf(stream, "\n");
		for(size_t n = VECTOR_MIN_SIZE; n <= VECTOR_MAX_SIZE; ++n){
			for(Type_Def_Type type = 0; type < COUNT_TYPE_DEFS; ++type){
				gen_vector_def(stream, n, type_defs[type]);
			}
		}
		fprintf(stream, "\n");
		for(size_t n = VECTOR_MIN_SIZE; n <= VECTOR_MAX_SIZE; ++n) {
			for(Type_Def_Type type = 0; type < COUNT_TYPE_DEFS; ++type) {
				gen_vector_printf_macros(stream, n, type_defs[type]);
				gen_vector_ctor_decl(stream, n, type_defs[type]);
				gen_vector_scalar_ctor_decl(stream, n, type_defs[type]);
				for(size_t src_n = VECTOR_MIN_SIZE; src_n <= VECTOR_MAX_SIZE; ++src_n) {
					for(Type_Def_Type src_type = 0; src_type < COUNT_TYPE_DEFS; ++src_type) {
						if(src_n != n || src_type != type) {
							gen_vector_convert_decl(stream, n, type_defs[type], src_n, type_defs[src_type]);
						}
					}
				}
				for(Op_Def_Type op = 0; op < COUNT_OP_DEFS; ++op){
					gen_vector_op_decl(stream, n, type_defs[type], op_defs[op]);
				}
				for(Fun_Def_Type fun = 0; fun < COUNT_FUN_DEFS; ++fun){
					if(fun_defs[fun].name_for_type[type]) {
						gen_vector_fun_decl(stream, n, type, fun);
					}
				}
				gen_vector_sqrlen_decl(stream, n, type_defs[type]);
				if(funcs_sqrt_defined_for[type]) {
					gen_vector_len_decl(stream, n, type_defs[type]);
				}
				fprintf(stream, "\n");
			}
		}

		fprintf(stream, "#endif //LA_H_\n");
		fprintf(stream, "\n");
	}

	// C IMPLEMENTATION
	{
		fprintf(stream, "#ifdef LA_IMPLEMENTATION\n");
		fprintf(stream, "\n");

		gen_lerp_impl(stream, "lerpf", "float");
		fputc('\n', stream);
		gen_lerp_impl(stream, "lerp", "double");
		fputc('\n', stream);
		gen_min_impl(stream, "mini", "int");
		fputc('\n', stream);
		gen_max_impl(stream, "maxi", "int");
		fputc('\n', stream);
		for(Type_Def_Type type = 0; type < COUNT_TYPE_DEFS; ++type) {
			gen_clamp_impl(stream, type, fun_defs[FUN_MIN], fun_defs[FUN_MAX]);
			fputc('\n', stream);
		}
		for(size_t n = VECTOR_MIN_SIZE; n <= VECTOR_MAX_SIZE; ++n){
			for(Type_Def_Type type = 0; type < COUNT_TYPE_DEFS; ++type){
				gen_vector_ctor_impl(stream, n, type_defs[type]);
				fputc('\n', stream);
				gen_vector_scalar_ctor_impl(stream, n, type_defs[type]);
				fputc('\n', stream);
				for(size_t src_n = VECTOR_MIN_SIZE; src_n <= VECTOR_MAX_SIZE; ++src_n) {
					for(Type_Def_Type src_type = 0; src_type < COUNT_TYPE_DEFS; ++src_type) {
						if(src_n != n || src_type != type) {
							gen_vector_convert_impl(stream, n, type_defs[type], src_n, type_defs[src_type]);
							fputc('\n', stream);
						}
					}
				}
				for(Op_Def_Type op = 0; op < COUNT_OP_DEFS; ++op){
					gen_vector_op_impl(stream, n, type_defs[type], op_defs[op]);
					fputc('\n', stream);
				}
				for(Fun_Def_Type fun = 0; fun < COUNT_FUN_DEFS; ++fun){
					if(fun_defs[fun].name_for_type[type]) {
						gen_vector_fun_impl(stream, n, type, fun);
						fputc('\n', stream);
					}
				}
				gen_vector_sqrlen_impl(stream, n, type_defs[type]);
				fputc('\n', stream);
				if(funcs_sqrt_defined_for[type]) {
					gen_vector_len_impl(stream, n, type_defs[type], funcs_sqrt_defined_for[type]);
					fputc('\n', stream);
				}
			}
		}

		fprintf(stream, "#endif //LA_IMPLEMENTATION\n");
	}
	// TODO: print stats on how many things were generated
	// TODO: v2*_add alias to v2*_sum
	// 'Cause I keep confusing them

	return 0;
}
