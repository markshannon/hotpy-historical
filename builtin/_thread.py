class error(Exception):
    'Raised on thread-specific errors.'
    pass

class LockType:
    'This is the type of lock objects.'
    pass

LockType.__name__ = 'lock'

#@c_function
#def _start_new_thread(function, args:tuple, kwargs:dict):
#    pass
#
#def start_new_thread(function, args, kwargs={}):
#    '''Start a new thread and return its identifier. The thread executes 
#    the function function with the argument list args (which must be a tuple). 
#    The optional kwargs argument specifies a dictionary of keyword arguments. 
#    When the function returns, the thread silently exits. 
#    When the function terminates with an unhandled exception, a stack trace 
#    is printed and then the thread exits (but other threads continue to run).'''
#    if not isinstance(args, tuple):
#        args = tuple(args)
#    if not isinstance(kwargs, dict):
#        kwargs = dict(kwargs)
#    _start_new_thread(function, args, kwargs)

#@c_function
#def interrupt_main():
#    '''Raise a KeyboardInterrupt exception in the main thread. 
#    A subthread can use this function to interrupt the main thread.'''
#    pass

#def exit():
#    raise SystemExit()

@_pure
@c_function
def allocate_lock():
    '''Return a new lock object. Methods of locks are described below. 
    The lock is initially unlocked.'''
    pass

#@c_function
#def get_ident():
#    '''Return the thread identifier of the current thread. 
#    This is a nonzero integer. Its value has no direct meaning; 
#    it is intended as a magic cookie to be used e.g. to index a dictionary 
#    of thread-specific data. Thread identifiers may be recycled when a thread
#    exits and another thread is created.'''
#    pass

#@c_function 
#def stack_size(size=0):
#    '''Return the thread stack size used when creating new threads.
#    The optional size argument specifies the stack size to be used for 
#    subsequently created threads, and must be 0 (use platform or 
#    configured default) or a positive integer value of at least 32,768 (32kB). 
#    If changing the thread stack size is unsupported, 
#    the error exception is raised. If the specified stack size is invalid, 
#    a ValueError is raised and the stack size is unmodified. 32kB is currently 
#    the minimum supported stack size value to guarantee sufficient stack space 
#    for the interpreter itself. Note that some platforms may have particular 
#    restrictions on values for the stack size, such as requiring a minimum 
#    stack size > 32kB or requiring allocation in multiples of the system 
#    memory page size - platform documentation should be referred to for 
#    more information (4kB pages are common; using multiples of 4096 for the 
#    stack size is the suggested approach in the absence of more specific 
#    information). Availability: Windows, systems with POSIX threads.'''
#    pass

#Lock objects have the following methods:

@_pure
@c_function
def lock_acquire(self, waitflag=0):
    '''Without the optional argument, this method acquires the lock 
    unconditionally, if necessary waiting until it is released by another
    thread (only one thread at a time can acquire a lock thats their reason 
    for existence). If the integer waitflag argument is present, the action 
    depends on its value: if it is zero, the lock is only acquired if it 
    can be acquired immediately without waiting, while if it is nonzero, 
    the lock is acquired unconditionally as before. The return value is 
    True if the lock is acquired successfully, False if not.'''
    pass

@_pure
@c_function
def lock_release(self):
    '''Releases the lock. The lock must have been acquired earlier,
    but not necessarily by the same thread.'''
    pass

@_pure
@c_function
def lock_locked(self):
    '''Return the status of the lock: True if it has been acquired 
    by some thread, False if not.'''
    pass

LockType.acquire = lock_acquire
lock_acquire.__name__ = 'acquire'
del lock_acquire

LockType.release = lock_release
lock_release.__name__ = 'release'
del lock_release

LockType.locked = lock_locked
lock_locked.__name__ = 'locked'
del lock_locked

