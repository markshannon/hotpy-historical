
R_object PREFIX(_table_get)(R_dict_table dict, KEY_TYPE key);

R_object PREFIX(_table_get_raise)(R_dict_table dict, KEY_TYPE key);

R_dict_table PREFIX(_table_resize) (R_dict_table old);

R_dict_table PREFIX(_table_set) (R_dict_table table, KEY_TYPE key, R_object item);

void PREFIX(_table_delete) (R_dict_table dict, KEY_TYPE key);

void PREFIX(_table_discard) (R_dict_table dict, KEY_TYPE key);

R_dict_table PREFIX(_table_update) (R_dict_table table, R_dict_table from);
