
_thread = sys.modules['_thread']
_allocate_lock = _thread.allocate_lock
#_start_new_thread = _thread.start_new_thread
#_get_ident = _thread.get_ident
ThreadError = _thread.error
del _thread

class _Condition:
    
    def __enter__(self):
        return self._lock.__enter__()

    def __exit__(self, *args):
        return self._lock.__exit__(*args)

@c_function
def condition_notify(self:_Condition, n=1):
    pass

@c_function
def condition_notify_all(self:_Condition):
    pass

@c_function
def condition_wait(self:_Condition, timeout=None):
    pass

@c_function
def condition_acquire(self:_Condition):
    pass

@c_function
def condition_release(self:_Condition):
    pass
    
_Condition.notify = condition_notify
condition_notify.__name__ = 'notify'
del condition_notify
    
_Condition.notify_all = condition_notify_all
condition_notify_all.__name__ = 'notify_all'
del condition_notify_all
     
_Condition.wait = condition_wait
condition_wait.__name__ = 'wait'
del condition_wait
     
_Condition.acquire = condition_acquire
condition_acquire.__name__ = 'acquire'
del condition_acquire
     
_Condition.release = condition_release
condition_release.__name__ = 'release'
del condition_release

def method(cls, name):
    def _method(func):
        setattr(cls, name, func)
        func.__name__ = name
        return None
    return _method

@method(Thread, '__init__')
def thread_init(self, group=None, target=None, name=None,
             args=(), kwargs=None, verbose=None):
    assert group is None, "group argument must be None for now"
    _Verbose.__init__(self, verbose)
    if kwargs is None:
        kwargs = {}
    self._target = target
    self._name = str(name or _newname())
    self._args = args
    self._kwargs = kwargs
#        self._daemonic = self._set_daemon()
    self._ident = None
    self._done = False
    self._join = Condition()
    
#        self._started = Event()
#        self._stopped = False
#        self._block = Condition(Lock())
#        self._initialized = True
    # sys.stderr is not stored in the class like
    # sys.exc_info since it can be changed between instances
    self._stderr = sys.stderr
    
@method(Thread, 'start')
def thread_start(self):
    if self._running is None:
        raise RuntimeError("thread.__init__() not called")
    if self._running.acquire(0):
        assert "Fix when _thread is sorted"
        #_start_new_thread(self._bootstrap, ())
    else:
        raise RuntimeError("thread already started")
    
@method(Thread, 'run')
def thread_run(self):
    if self._target:
        self._target(*self._args, **self._kwargs)

@method(Thread, '_bootstrap')
def thread_bootstrap(self):
    self._set_ident()
    if _trace_hook:
        sys.settrace(_trace_hook)
    if _profile_hook:
        sys.setprofile(_profile_hook)
    try:
        self.run()
    except SystemExit:
        pass
    except BaseException as ex:
        sys.stderr.write("Exception in thread %s:\n%s\n" %
                          (self.name, _format_exc()))
    self._join.acquire()
    self._done = True
    self._join.broadcast()
    self._join.release()
    
@method(Thread, '_set_ident')
def thread_set_ident(self):
    assert "Fix when _thread is sorted"
    #self._ident = _get_ident()

@method(Thread, 'join')
def thread_join(self, timeout=None):
    self._join.acquire()
    if not self._done:
        self._join.wait(timeout)
    self._join.release()

del thread_init
del thread_start
del thread_run
del thread_bootstrap
del thread_set_ident
del thread_join

