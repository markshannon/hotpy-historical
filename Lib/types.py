FunctionType = print$type
BuiltinFunctionType = FunctionType
BuiltinMethodType = FunctionType
CodeType = print.__code__$type

import sys
FrameType = sys._getframe()$type
del sys
GeneratorType = generator
#GetSetDescriptorType = None # To do....
LambdaType = FunctionType
#MemberDescriptorType = None # To do....
MethodType = FunctionType
ModuleType = __builtins__$type
e = Exception()
try:
    raise e
except:
    pass
TracebackType = e.__traceback__$type

