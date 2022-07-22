
R_object PREFIX(_table_get)(R_dict_table dict, KEY_TYPE key) {
    return VALUE(dict, LOOKUP (dict, key));
}

R_object PREFIX(_table_get_raise)(R_dict_table dict, KEY_TYPE key) {
    R_object result = VALUE(dict, LOOKUP (dict, key));
    if (result == 0) {
        KEY_ERROR(key);
        return NULL; /* Keep compiler quiet */
    } else {
        return result;
    }
}

R_dict_table PREFIX(_table_resize) (R_dict_table old) {
    R_dict_table table = new_dict_table(old->length);
    uint32_t i, len = old->length >> 1;
    for (i = 0; i < len; i++) {
        assert(KEY(table, i) == (R_object)NULL_R);
        assert(VALUE(table, i) == (R_object)NULL_R);
    }
    for (i = 0; i < len; i++) {
        if (VALUE(old, i) != (R_object)NULL_R) {
            R_object key, value;
            uint32_t index = LOOKUP (table, (KEY_TYPE)KEY(old, i));
            assert(KEY(table, index) == (R_object)NULL_R);
            key = KEY(old, i);
            KEY(table, index) = key;
            value = VALUE(old, i);
            VALUE(table, index) = value;
        }
    } 
    assert(old->size >= 0);
    table->size = table->used = old->size;
    return table;
}

R_dict_table PREFIX(_table_set) (R_dict_table table, KEY_TYPE key, R_object item) {
    uint32_t index;
    assert(table->load >= 0);
    index = LOOKUP(table, key);
    if (KEY(table, index) == (R_object)NULL_R) {
        if (table->used >= table->load) {
            R_dict_table new_table = PREFIX(_table_resize) (table);
            // This renders the old table invalid - set its load negative.
            // and ensures that the empty dict-table always remains valid.
            table->load = -table->load;
            table = new_table;
            index = LOOKUP(table, key);
        }
        table->used += 1;
    }
    if (VALUE(table, index) == (R_object)NULL_R) {
        table->size++;      
        KEY(table, index) = (R_object)key;
    }
    VALUE(table, index) = item;
    return table;
}

R_dict_table PREFIX(_table_update) (R_dict_table table, R_dict_table from) {
    uint32_t i, len = from->length >> 1;
    KEY_TYPE key;
    R_object value;
    for (i = 0; i < len; i++) {
        value = VALUE(from, i);
        if (value != (R_object)NULL_R) {
            key = (KEY_TYPE)KEY(from, i);
            table = PREFIX(_table_set)(table, key, value);
        }
    }
    return table;
}

void PREFIX(_table_delete) (R_dict_table dict, KEY_TYPE key) {
    uint32_t index = LOOKUP (dict, key);
    if (VALUE(dict, index) != (R_object)NULL_R) {
        VALUE(dict, index) = 0; 
        dict->size--;
        assert(dict->size >= 0);
    } else {
        KEY_ERROR(key);
    }
}

void PREFIX(_table_discard) (R_dict_table dict, KEY_TYPE key) {
    uint32_t index = LOOKUP (dict, key);
    if (VALUE(dict, index) != (R_object)NULL_R) {
        VALUE(dict, index) = 0; 
        dict->size--;
        assert(dict->size >= 0);
    }
}




