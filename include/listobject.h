#ifndef HOTPY_LISTOBJECT_H
#define HOTPY_LISTOBJECT_H

// Size in kept in array to allow atomic resizing.
GVMT_OBJECT(list) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t lock;                     
    GVMT_OBJECT(array)* array;  
};

GVMT_OBJECT(listiterator) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t next;              
    GVMT_OBJECT(list)* list;   
};

extern GVMT_OBJECT(list) ARGV_LIST;

R_list HotPySequence_List(R_object object);

R_list pack_list_reverse(int count, GVMT_Value* values);

R_list make_list(int capacity);

void list_append(R_list l, R_object o);

void list_insert_nolock(R_list l, intptr_t index, R_object o);

void list_append_nolock(R_list list, R_object item);

R_object list_pop(R_list list);

R_object PY_FUNC1(new_list, R_type cls);

void unpack_list(int count, R_list sequence, GVMT_Value* values);

R_object list_pop_unchecked(R_list list, intptr_t index);

R_list list_concat(R_list left, R_list right);

#ifndef NDEBUG
void __hotpy_list_type_assert(R_list l, R_type cls);
#define LIST_TYPE_ASSERT(l, cls) __hotpy_list_type_assert((R_list)(l), type_##cls)
#else
#define LIST_TYPE_ASSERT(l, cls)
#endif

#define LIST_SIZE(l) ((l)->array->a_size)
#define SET_LIST_SIZE(l,s) (l)->array->a_size = (s)

#endif
