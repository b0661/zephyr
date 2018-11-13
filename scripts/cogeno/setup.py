#
# Copyright (c) 2018 Bobby Noelte.
#
# SPDX-License-Identifier: Apache-2.0
#

import sys
from setuptools import setup, find_packages
from setuptools.command.test import test as TestCommand

import cogeno

long_desc = '''
Cogeno, the inline code generation tool, processes Python or Jinja2 'snippets'
inlined in your source files. It can also access CMake build
parameters and device tree information to generate source code automatically
tailored and tuned to a specific project configuration.

Cogeno can be used, for example, to generate source code that creates
and fills data structures, adapts programming logic, creates
configuration-specific code fragments, and more.
'''

requires = ['Jinja2',]

setup(
    name='cogeno',
    packages=find_packages(),
    keywords='cogeno inline code generator embedded',
    version='0.1.3',
    url='https://gitlab.com/b0661/cogeno',
    download_url='https://gitlab.com/b0661/cogeno',
    license='Apache-2.0',
    author='Bobby Noelte',
    author_email='b0661n0e17e@gmail.com',
    description='Inline code generation using Python or Jinja2 script snippets in any type of source file.',
    long_description=long_desc,
    zip_safe=False,
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'Intended Audience :: Education',
        'License :: OSI Approved :: Apache Software License',
        'Programming Language :: Python :: 3 :: Only',
        'Topic :: Software Development :: Code Generators',
        'Topic :: Software Development :: Embedded Systems',
        'Topic :: Utilities',
        'Operating System :: OS Independent',
    ],
    platforms='any',
    include_package_data=True,
    entry_points={
        'console_scripts': [
            'cogeno = cogeno.cogeno:main',
            'edtsdatabase = cogeno.modules.edtsdatabase:main'
        ],
    },
    install_requires=requires,
    setup_requires=["pytest-runner"],
    tests_require=["pytest", "scancode-toolkit"],
)
