FunctionType = print$type
BuiltinFunctionType = FunctionType
BuiltinMethodType = FunctionType
CodeType = print.__code__$type

FrameType = sys._getframe()$type
GeneratorType = generator
#GetSetDescriptorType = None # To do....
LambdaType = FunctionType
#MemberDescriptorType = None # To do....
MethodType = FunctionType
ModuleType = __builtins__$type
TracebackType = Exception().__traceback__$type

