import objects, sys, common, gtypes

try:
    slist = objects.read_file(common.In('vm/types.types'))
except common.GVMTException, ex:
    print ex
    sys.exit(1)
    
C = common.Out('vm/types.c')
H = common.Out('include/types.h')

slist = [s for s in slist if not s.qualifiers or 'notype' not in s.qualifiers]

for s in slist:
    H << "extern R_type type_%s;\n" % s.name

C << '#include "typedefs.h"\n'
for s in slist:
    C << "extern GVMT_OBJECT(type) _%s;\n" % s.name
C << '\n'
for s in slist:
    C << "R_type type_%s = &_%s;\n" % (s.name, s.name)
C << '\n'

C.close()
H.close()

