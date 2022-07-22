
typedef enum _block_type { FunctionBlock, ClassBlock, ModuleBlock }
    _Py_block_ty;

typedef GVMT_OBJECT(symtable)* R_symtable;
typedef GVMT_OBJECT(symtable_scope)* R_symtable_scope;
extern GVMT_OBJECT(type) *type_symtable;
extern GVMT_OBJECT(type) *type_symtable_scope;

GVMT_OBJECT(symtable) { // special 
    GVMT_OBJECT(type)* ob_type; 
    uintptr_t ref_count;
    GVMT_OBJECT(str)* st_filename;
    GVMT_OBJECT(symtable_scope)* st_top;
    GVMT_OBJECT(dict)* st_blocks;
    GVMT_OBJECT(object)* st_future;
};

GVMT_OBJECT(symtable_scope) { // special 
    GVMT_OBJECT(type)* ob_type;
    uintptr_t ref_count;
    int ste_type;   /* module, class, or function */
	int8_t ste_unoptimized;     /* false if namespace is optimized */
	int8_t ste_deletes;      /* true if block is nested */
	int8_t ste_nested;      /* true if block is nested */
	int8_t ste_free;        /* true if block has free variables */
	int8_t ste_child_free;  /* true if a child block has free vars, including free refs to globals */
	int8_t ste_generator;   /* true if namespace is a generator */
	int8_t ste_varargs;     /* true if block has varargs */
	int8_t ste_varkeywords; /* true if block has varkeywords */
	int8_t ste_returns_value;  /* true if namespace uses return with an argument */
	int ste_lineno;          /* first line of block */
	int ste_opt_lineno;      /* lineno of last exec or import * */
	int ste_tmpname;         /* counter for listcomp temp vars */
    GVMT_OBJECT(object)* ste_node; /* AST root node for this scope */
    GVMT_OBJECT(dict)* ste_symbols; /* Mapping of names to flags */
    GVMT_OBJECT(str)* ste_name;      /* string: name of current block */
	GVMT_OBJECT(object)* ste_varnames;  /* list of variable names */
    GVMT_OBJECT(symtable_scope)* ste_enclosing;
    GVMT_OBJECT(symtable)* ste_table;   
};


/* Flags for def-use information */

#define DEF_GLOBAL 1           /* global stmt */
#define DEF_LOCAL 2            /* assignment in code block */
#define DEF_PARAM 2<<1         /* formal parameter */
#define DEF_NONLOCAL 2<<2      /* nonlocal stmt */
#define USE 2<<3               /* name is used */
#define DEF_STAR 2<<4          /* parameter is star arg */
#define DEF_DOUBLESTAR 2<<5    /* parameter is star-star arg */
#define DEF_INTUPLE 2<<6       /* name defined in tuple in parameters */
#define DEF_FREE 2<<7          /* name used but not defined in nested block */
#define DEF_FREE_GLOBAL 2<<8   /* free variable is actually implicit global */
#define DEF_FREE_CLASS 2<<9    /* free variable from class's method */
#define DEF_IMPORT 2<<10        /* assignment occurred via import */

#define DEF_BOUND (DEF_LOCAL | DEF_PARAM | DEF_IMPORT)


#define LOCAL 1
#define GLOBAL_EXPLICIT 2
#define GLOBAL_IMPLICIT 3
#define FREE 4
#define NONLOCAL 5

/* The following two names are used for the ste_unoptimized bit field */
#define OPT_IMPORT_STAR 1
#define OPT_TOPLEVEL 2  /* top-level names, including eval and exec */

#define GENERATOR 1
#define GENERATOR_EXPRESSION 2


/***** Code gen *****/

typedef GVMT_OBJECT(code_builder)* R_code_builder;

R_code_builder new_code_builder(void);
R_code_object make_code_object(R_code_builder builder, R_Constants consts);
void write_pyc_file(R_code_builder builder, R_Constants consts, FILE* out);

extern R_type type_code_builder;

/**** Extends bytelist ******/
GVMT_OBJECT(code_builder) {  // special
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t size;
    int32_t lock;
    GVMT_OBJECT(bytearray)* array;   
//  Extension
    GVMT_OBJECT(list)* local_names;
    uint32_t name;
    uint32_t kind;   
    uint32_t params; 
    uint32_t locals; // Should this be computed?
};

