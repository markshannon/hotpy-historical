import sys, os

ops = open(sys.argv[1]).readlines()
ops = [ op.strip('\n') for op in ops if op[0] != '#' and op[0] != '\n' ]

C = open(sys.argv[2] + '.c', 'w')
H = open(sys.argv[2] + '.h', 'w')
J = open(sys.argv[3] + '.java', 'w')

print >> C, '#include "hotpy.h"'
print >> C, '#include "%s.h"' % sys.argv[2].split(os.path.sep)[-1]

print >> C, """
R_tuple operator_names = &operator_names_table;
#ifdef BOOTSTRAP
"""
print >> H, '#ifndef OPERATOR_H'
print >> H, '#define OPERATOR_H'
 
index = 0;
for op in ops:
    print >> H, '#define OPERATOR_%s_INDEX %d' % (op.upper(), index)
    print >> H, '#define operator_%s operators[%d]' % (op, index)
    print >> H, 'extern GVMT_OBJECT(operator) operator_object_%s;' % op
    index += 1

print >> H, '#define OPERATOR_COUNT %d' % index
print >> H, '#endif'

print >> C, 'R_operator operators[] = {'
for i in range(index):
    print >> C, "0,"
print >> C, "};"
print >> C, """
#else
"""

print >> C, 'R_operator operators[] = {'
for op in ops:
    print >> C, "&operator_object_%s," % op
print >> C, "};"


print >> C, '#endif'

C.close()
H.close()

print >> J, 'package parser;\nclass %s {' % sys.argv[3].split('/')[-1]

print >> J, '    static final String[] names = {'
for op in ops:
    print >> J, '        "%s",' % op
print >> J, '    };'
print >> J, '}'
J.close()


