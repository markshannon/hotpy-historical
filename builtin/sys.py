
argv = []

modules = { }

path = ""

platform = "linux2"

stdin = 0
stdout = 0
stderr = 0

# FIX THIS!
_base_path = '/home/mark/hotpy/trunk/'

path = [ '', _base_path + 'Lib/' ]

del _base_path

@c_function
def _abort(code:int):
    pass

def exit(code):
    if $subtype(code$type, int):
        raise SystemExit(code)
    else:
        print(code)
        raise SystemExit(0)

@c_function
def setrecursionlimit(x):
    pass

@c_function
def getrecursionlimit():
    pass

@c_function
def settrace(tracefunc):
    pass

@c_function
def gettrace():
    pass

@c_function
def _getframe(depth:int=0):
    pass

def getrefcount(obj):
    'For CPython compatibility'
    #Docs say 'may return one more than you expect'
    return 1+1

copyright = '''Copyright Mark Shannon 2009-2010.
This is free(libre) software.
See http://www.gnu.org/licenses/gpl-3.0-standalone.html
'''

ps1 = 'H>>'
ps2 = 'H..'

dont_write_bytecode = False

def setcheckinterval(interval):
    'HotPy is free-threading and signals are immediate'
    pass

version_info = (3, 1, 0, 'alpha', 0)


