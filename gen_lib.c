#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#define UNIMPLEMENTED \
	do { \
		fprintf(stderr, "%s:%d: %s is not implemented yet\n", \
			__FILE__, __LINE__, __func__); \
		abort(); \
	} while(0)

typedef struct {
	char data[128];
} Short_String;

typedef struct {
	const char *name;
	const char *suffix;
} Type_Def;

typedef enum {
	TYPE_DEF_FLOAT = 0,
	TYPE_DEF_DOUBLE,
	TYPE_DEF_INT,
	TYPE_DEF_SIZE,
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

typedef struct {
	const char *suffix;
	// NULL -> function not supported for type
	const char *name_for_type[COUNT_TYPE_DEFS];
	size_t arity;
} Fun_Def;

typedef enum {
	FUN_SQRT = 0,
	FUN_POW,
	FUN_SIN,
	FUN_COS,
	COUNT_FUN_DEFS,
} Fun_Def_Type;

static Type_Def type_defs[COUNT_TYPE_DEFS] = {
	[TYPE_DEF_FLOAT] 	= {.name = "float", .suffix = "f"},
	[TYPE_DEF_DOUBLE] 	= {.name = "double", .suffix = "d"},
	[TYPE_DEF_INT] 		= {.name = "int", .suffix = "i"},
	[TYPE_DEF_SIZE] 	= {.name = "size_t", .suffix = "s"},
};

static Op_Def op_defs[COUNT_OP_DEFS] = {
	[OP_DEF_SUM] = {.suffix = "sum", .op = "+="},
	[OP_DEF_SUB] = {.suffix = "sub", .op = "-="},
	[OP_DEF_MUL] = {.suffix = "mul", .op = "*="},
	[OP_DEF_DIV] = {.suffix = "div", .op = "/="},
};

static_assert(COUNT_FUN_DEFS == 4, "More functions are been introduced please update the array below");
static_assert(COUNT_TYPE_DEFS == 4, "More types are been introduced please update the array below");
static Fun_Def fun_defs[COUNT_FUN_DEFS] = {
	[FUN_SQRT] = {
		.suffix = "sqrt",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "sqrtf",
			[TYPE_DEF_DOUBLE] = "sqrt",
		},
		.arity = 1,
	},
	[FUN_POW] = {
		.suffix = "pow",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "powf",
			[TYPE_DEF_DOUBLE] = "pow",
		},
		.arity = 2,
	},
	[FUN_SIN] = {
		.suffix = "sin",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "sinf",
			[TYPE_DEF_DOUBLE] = "sin",
		},
		.arity = 1,
	},
	[FUN_COS] = {
		.suffix = "cos",
		.name_for_type = {
			[TYPE_DEF_FLOAT] = "cosf",
			[TYPE_DEF_DOUBLE] = "cos",
		},
		.arity = 1,
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
	int n = vsnprintf(result.data, sizeof(result.data), fmt, args);
	assert(n >= 0);
		assert((size_t) n + 1 <= sizeof(result.data));
	va_end(args);

	return result;
}

void gen_vector_def(FILE* stream, size_t n, Type_Def type_def){
	fprintf(stream, "typedef struct { %s c[%zu]; } V%zu%s;\n", type_def.name, n, n, type_def.suffix);
}

void gen_vector_op_sig(FILE* stream, size_t n, Type_Def type_def, Op_Def op_def){
	Short_String type = shortf("V%zu%s", n ,type_def.suffix);
	Short_String prefix = shortf("v%zu%s", n, type_def.suffix);

	fprintf(stream, "%s %s_%s(%s a, %s b)", type.data, prefix.data, op_def.suffix, type.data, type.data);
}

void gen_vector_ctor_sig(FILE* stream, size_t n, Type_Def type_def){
	Short_String type = shortf("V%zu%s", n ,type_def.suffix);
	Short_String prefix = shortf("v%zu%s", n, type_def.suffix);
	fprintf(stream, "%s %s(", type.data, prefix.data);
	for(size_t i = 0; i < n; ++i){
		if(i > 0) fprintf(stream, ", ");
		fprintf(stream, "%s x%zu", type_def.name, i);
	}
	fprintf(stream, ")");
}

void gen_vector_scalar_ctor_sig(FILE* stream, size_t n, Type_Def type_def){
	Short_String type = shortf("V%zu%s", n ,type_def.suffix);
	Short_String prefix = shortf("v%zu%s", n, type_def.suffix);
	fprintf(stream, "%s %ss(%s x)", type.data, prefix.data, type_def.name);

}

void gen_vector_fun_sig(FILE *stream, const char *ret_type, const char *prefix, const char *suffix, const char *arg_type, const char *arg_prefix, size_t arity){
	fprintf(stream, "%s %s_%s(", ret_type, prefix, suffix);
	for(size_t arg_num = 0; arg_num < arity; ++arg_num){
		if(arg_num > 0) fprintf(stream, ", ");
		fprintf(stream, "%s %s%zu", arg_type, arg_prefix, arg_num);
	}
	fprintf(stream, ")");
}

void gen_vector_op_decl(FILE* stream, size_t n, Type_Def type_def, Op_Def op_def){
	gen_vector_op_sig(stream, n, type_def, op_def);
	fprintf(stream, ";\n");
}

void gen_vector_op_impl(FILE* stream, size_t n, Type_Def type_def, Op_Def op_def){
	gen_vector_op_sig(stream, n, type_def, op_def);
	fprintf(stream, "\n{\n");
	fprintf(stream, "\tfor(int i = 0; i < %zu; ++i) a.c[i] %s b.c[i];\n", n, op_def.op);
	fprintf(stream, "\treturn a;\n");
	fprintf(stream, "}\n");
}

void gen_vector_ctor_decl(FILE* stream, size_t n, Type_Def type_def){
	gen_vector_ctor_sig(stream, n, type_def);
	fprintf(stream, ";\n");
}

void gen_vector_ctor_impl(FILE* stream, size_t n, Type_Def type_def){
	Short_String type = shortf("V%zu%s", n ,type_def.suffix);

	gen_vector_ctor_sig(stream, n, type_def);
	fprintf(stream, "\n{\n");
	fprintf(stream, "\t%s result;\n", type.data);
	for(size_t i = 0; i < n; ++i){
		fprintf(stream, "\tresult.c[%zu] = x%zu;\n", i, i);
	}
	fprintf(stream, "\treturn result;\n");
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
	fprintf(stream, "\treturn %s(", prefix.data);
	for(size_t i = 0; i < n; ++i){
		if(i > 0) fprintf(stream, ", ");
		fprintf(stream, "x");
	}
	fprintf(stream, ");\n");
	fprintf(stream, "}\n");
}

void gen_vector_fun_decl(FILE *stream, size_t n, Type_Def_Type type, Fun_Def_Type fun){
	Type_Def type_def = type_defs[type];
	Fun_Def fun_def = fun_defs[fun];

	if(fun_def.name_for_type[type]){ //Print only functions who are defined
		Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
		Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);

		gen_vector_fun_sig(stream, vector_type.data, vector_prefix.data, fun_def.suffix, vector_type.data, "v", fun_def.arity);
		fprintf(stream, ";\n");
	}
}

void gen_vector_fun_impl(FILE *stream, size_t n, Type_Def_Type type, Fun_Def_Type fun){
	Type_Def type_def = type_defs[type];
	Fun_Def fun_def = fun_defs[fun];

	const char *arg_prefix = "v";

	if(fun_def.name_for_type[type]){ //Print only functions who are defined
		Short_String vector_type = shortf("V%zu%s", n, type_def.suffix);
		Short_String vector_prefix = shortf("v%zu%s", n, type_def.suffix);

		gen_vector_fun_sig(stream, vector_type.data, vector_prefix.data, fun_def.suffix, vector_type.data, arg_prefix, fun_def.arity);
		fprintf(stream, "\n{\n");
		assert(fun_def.arity >= 1);
		fprintf(stream, "\tfor(int i = 0; i < %zu; ++i) %s0.c[i] = %s(", n, arg_prefix, fun_def.name_for_type[type]);
		for(size_t arg_num = 0; arg_num < fun_def.arity; ++arg_num){
			if(arg_num > 0) fprintf(stream, ", ");
			fprintf(stream, "%s%zu.c[i]", arg_prefix, arg_num);
		}
		fprintf(stream, ");\n");
		fprintf(stream, "\treturn %s0;\n", arg_prefix);
		fprintf(stream, "}\n");
	}
}

// TODO: lerp operation for vectors
// TODO: len operation for vectors
// TODO: sqrlen operation for vectors
// TODO: min operation for vectors
// TODO: max operation for vectors
// TODO: matrices
// TODO: macro blocks to disable size, types, etc

int main(int argc, char **argv){

	(void) argc;
	(void) argv;

	FILE *stream = stdout;

	// HEADER
	fprintf(stream, "#ifndef LA_H_\n");
	fprintf(stream, "#define LA_H_\n");
	fprintf(stream, "\n");
	fprintf(stream, "#include <stdlib.h>\n");
	fprintf(stream, "#include <math.h>\n");
	fprintf(stream, "\n");

	for(size_t n = 2; n <= 4; ++n){
		for(Type_Def_Type type = 0; type < COUNT_TYPE_DEFS; ++type){
			gen_vector_def(stream, n, type_defs[type]);
			for(Op_Def_Type op = 0; op < COUNT_OP_DEFS; ++op){
				gen_vector_op_decl(stream, n, type_defs[type], op_defs[op]);
			}
			gen_vector_ctor_decl(stream, n, type_defs[type]);
			gen_vector_scalar_ctor_decl(stream, n, type_defs[type]);
			for(Fun_Def_Type fun = 0; fun < COUNT_FUN_DEFS; ++fun){
				gen_vector_fun_decl(stream, n, type, fun);
			}
			fprintf(stream, "\n");
		}
	}

	fprintf(stream, "#endif //LA_H_\n");
	fprintf(stream, "\n");

	// C IMPLEMENTATION
	fprintf(stream, "#ifdef LA_IMPLEMENTATION\n");
	fprintf(stream, "\n");

	for(size_t n = 2; n <= 4; ++n){
		for(Type_Def_Type type = 0; type < COUNT_TYPE_DEFS; ++type){
			for(Op_Def_Type op = 0; op < COUNT_OP_DEFS; ++op){
				gen_vector_op_impl(stream, n, type_defs[type], op_defs[op]);
			}
			gen_vector_ctor_impl(stream, n, type_defs[type]);
			gen_vector_scalar_ctor_impl(stream, n, type_defs[type]);
			for(Fun_Def_Type fun = 0; fun < COUNT_FUN_DEFS; ++fun){
				gen_vector_fun_impl(stream, n, type, fun);
			}
		}
	}

	fprintf(stream, "#endif //LA_IMPLEMENTATION\n");

	return 0;
}
