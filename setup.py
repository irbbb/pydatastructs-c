from setuptools import setup, Extension, find_packages

dllist_ext = Extension(
    name='pydatastructs.dllist',
    sources=[
        'bindings/dllist_py.c',
        'src/dllist.c',
    ],
)

min_heap_ext = Extension(
    name='pydatastructs.min_heap',
    sources=[
        'bindings/min_heap_py.c',
        'src/min_heap.c',
    ],
)

linked_list_ext = Extension(
    name='pydatastructs.linked_list',
    sources=[
        'bindings/linked_list_py.c',
        'src/linked_list.c'
    ]
)

setup(
    name="pydatastructs",
    version="0.1.0",
    description="Data Structures in C for Python",
    author="irbbb",
    ext_modules=[dllist_ext, min_heap_ext, linked_list_ext],
    classifiers=[
        'Programming Language :: Python :: 3',
        'Programming Language :: C',
    ],
    python_requires='>=3.7',
)