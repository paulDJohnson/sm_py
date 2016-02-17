from distutils.core import setup, Extension


setup(
    ext_modules = [Extension('sm_py' ,
                            include_dirs = ['/usr/include'],
                            libraries = ['gcrypt'],
                            runtime_library_dirs = ['/lib/x86_64-linux-gnu/'],
                            sources =['sm_py.c', 'sm.c', 'hexString.c']),
                            ]
)
