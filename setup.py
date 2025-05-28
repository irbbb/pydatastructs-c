from setuptools import setup, Extension, find_packages

dllist_ext = Extension(
    name='pydatastructs.dllist',
    sources=[
        'bindings/dllist_py.c',
        'src/dllist.c',
    ],
)


setup(
    name="pydatastructs",
    version="0.1.0",
    description="Data Structures in C for Python",
    author="irbbb",
    ext_modules=[dllist_ext],
    classifiers=[
        'Programming Language :: Python :: 3',
        'Programming Language :: C',
    ],
    python_requires='>=3.7',
)