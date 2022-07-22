In order to work with the current CPython API, hotpy objects need to be able to 
be safely manipulated in native code and support reference counting. 

Unfortunately, due to C extensions casting PyObject* pointers to other object types
it is not possible to do this through proxies.

Therefore HotPy objects must, at least when being manipulated by C extension code
support ref-counting, be unmovable
