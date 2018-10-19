#!/usr/bin/env python3
#
# Copyright (c) 2017, Linaro Limited
# Copyright (c) 2018, Bobby Noelte
#
# SPDX-License-Identifier: Apache-2.0
#

import os, fnmatch
import re
import yaml
from collections.abc import Mapping

from pathlib import Path

class Binder(yaml.Loader):

    ##
    # List of all yaml files available for yaml loaders
    # of this class. Must be preset before the first
    # load operation.
    _files = []

    ##
    # Files that are already included.
    # Must be reset on the load of every new binding
    _included = []

    @classmethod
    def _merge_binding_dicts(cls, parent, fname, dct, merge_dct):
        # from https://gist.github.com/angstwad/bf22d1822c38a92ec0a9

        """ Recursive dict merge. Inspired by :meth:``dict.update()``, instead of
        updating only top-level keys, _merge_binding_dicts recurses down into dicts nested
        to an arbitrary depth, updating keys. The ``merge_dct`` is merged into
        ``dct``.
        :param parent: parent tuple key
        :param fname: yaml file being processed
        :param dct: dict onto which the merge is executed
        :param merge_dct: dct merged into dct
        :return: None
        """
        for k, v in merge_dct.items():
            if (k in dct and isinstance(dct[k], dict)
                    and isinstance(merge_dct[k], Mapping)):
                Binder._merge_binding_dicts(k, fname, dct[k], merge_dct[k])
            else:
                if k in dct and dct[k] != merge_dct[k]:
                    # type, title, description, version of inherited node
                    # are overwritten by intention.
                    if k == 'type':
                        # collect types
                        if not isinstance(dct['type'], list):
                            dct['type'] = [dct['type'],]
                        if isinstance(merge_dct['type'], list):
                            dct['type'].extend(merge_dct['type'])
                        else:
                            dct['type'].append(merge_dct['type'])
                        continue
                    elif k in ("title", "version", "description"):
                        # do not warn
                        pass
                    elif (k == "category") and (merge_dct[k] == "required") \
                          and (dct[k] == "optional"):
                        # do not warn
                        pass
                    else:
                        print("binder.py: {}('{}') merge of property '{}': "
                              "'{}' overwrites '{}'."
                              .format(fname, parent, k, merge_dct[k], dct[k]))
                dct[k] = merge_dct[k]

    @classmethod
    def _traverse_inherited(cls, fname, node):
        """ Recursive overload procedure inside ``node``
        ``inherits`` section is searched for and used as node base when found.
        Base values are then overloaded by node values
        and some consistency checks are done.
        :param fname: initial yaml file being processed
        :param node:
        :return: node
        """

        # do some consistency checks. Especially id is needed for further
        # processing. title must be first to check.
        if 'title' not in node:
            # If 'title' is missing, make fault finding more easy.
            # Give a hint what node we are looking at.
            print("binder.py: {} node without 'title' -", fname, node)
        for prop in ('title', 'version', 'description'):
            if prop not in node:
                node[prop] = "<unknown {}>".format(prop)
                print("binder.py: {} WARNING:".format(fname),
                      "'{}' property missing in".format(prop),
                      "'{}' binding. Using '{}'.".format(node['title'],
                                                         node[prop]))

        # warn if we have an 'id' field
        if 'id' in node:
            print("binder.py: {} WARNING: id field set".format(fname),
                  "in '{}', should be removed.".format(node['title']))

        if 'inherits' in node:
            if isinstance(node['inherits'], list):
                inherits_list  = node['inherits']
            else:
                inherits_list  = [node['inherits'],]
            node.pop('inherits')
            for inherits in inherits_list:
                if 'inherits' in inherits:
                    inherits = cls._traverse_inherited(fname, inherits)
                cls._merge_binding_dicts(None, fname, inherits, node)
                node = inherits
        return node

    ##
    # @brief Get bindings for given compatibles.
    #
    # @param compatibles
    # @param bindings_paths directories to search for binding files
    # @return dictionary of bindings found
    @classmethod
    def bindings(cls, compatibles, bindings_paths):
        # find unique set of compatibles across all active nodes
        s = set()
        for k, v in compatibles.items():
            if isinstance(v, list):
                for item in v:
                    s.add(item)
            else:
                s.add(v)

        # scan YAML files and find the ones we are interested in
        # We add our own bindings directory first (highest priority)
        # We only allow one binding file with the same name
        bindings_paths.insert(0, Path(Path(__file__).resolve().parent,
                                      'bindings'))
        cls._files = []
        binding_files = []
        for path in bindings_paths:
            for root, dirnames, filenames in os.walk(str(path)):
                for filename in fnmatch.filter(filenames, '*.yaml'):
                    if not filename in binding_files:
                        binding_files.append(filename)
                        cls._files.append(os.path.join(root, filename))

        bindings_list = {}
        file_load_list = set()
        for file in cls._files:
            for line in open(file, 'r', encoding='utf-8'):
                if re.search('^\s+constraint:*', line):
                    c = line.split(':')[1].strip()
                    c = c.strip('"')
                    if c in s:
                        if file not in file_load_list:
                            file_load_list.add(file)
                            with open(file, 'r', encoding='utf-8') as yf:
                                cls._included = []
                                # collapse the bindings inherited information before return
                                bindings_list[c] = cls._traverse_inherited(file, yaml.load(yf, cls))

        return bindings_list

    def __init__(self, stream):
        filepath = os.path.realpath(stream.name)
        if filepath in self._included:
            print("Error:: circular inclusion for file name '{}'".
                  format(stream.name))
            raise yaml.constructor.ConstructorError
        self._included.append(filepath)
        super(Binder, self).__init__(stream)
        Binder.add_constructor('!include', Binder._include)
        Binder.add_constructor('!import',  Binder._include)

    def _include(self, node):
        if isinstance(node, yaml.ScalarNode):
            return self._extract_file(self.construct_scalar(node))

        elif isinstance(node, yaml.SequenceNode):
            result = []
            for filename in self.construct_sequence(node):
                result.append(self._extract_file(filename))
            return result

        elif isinstance(node, yaml.MappingNode):
            result = {}
            for k, v in self.construct_mapping(node).iteritems():
                result[k] = self._extract_file(v)
            return result

        else:
            print("Error: unrecognised node type in !include statement")
            raise yaml.constructor.ConstructorError

    def _extract_file(self, filename):
        filepaths = [filepath for filepath in self._files
		     if os.path.basename(filepath) == filename]
        if len(filepaths) == 0:
            print("Error: unknown file name '{}' in !include statement".
                  format(filename))
            raise yaml.constructor.ConstructorError
        elif len(filepaths) > 1:
            # multiple candidates for filename
            print("Warning: multiple candidates for file name "
                   "'{}' in !include statement - using first of".
                   format(filename), filepaths)
        with open(filepaths[0], 'r', encoding='utf-8') as f:
            return yaml.load(f, Binder)
