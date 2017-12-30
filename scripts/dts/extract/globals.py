#
# Copyright (c) 2017 Linaro
# Copyright (c) 2017 Bobby Noelte
#
# SPDX-License-Identifier: Apache-2.0
#

# globals
compatibles = {}
phandles = {}
aliases = {}
chosen = {}
reduced = {}


def convert_string_to_label(s):
    # Transmute ,- to _
    s = s.replace("-", "_")
    s = s.replace(",", "_")
    s = s.replace("@", "_")
    return s


def get_all_compatibles(d, name, comp_dict):
    if 'props' in d:
        compat = d['props'].get('compatible')
        enabled = d['props'].get('status')

    if enabled == "disabled":
        return comp_dict

    if compat is not None:
        comp_dict[name] = compat

    if name != '/':
        name += '/'

    if isinstance(d, dict):
        if d['children']:
            for k, v in d['children'].items():
                get_all_compatibles(v, name + k, comp_dict)

    return comp_dict


def get_aliases(root):
    if 'children' in root:
        if 'aliases' in root['children']:
            for k, v in root['children']['aliases']['props'].items():
                aliases[v] = k

    return


def get_compat(node):

    compat = None

    if 'props' in node:
        compat = node['props'].get('compatible')

    if isinstance(compat, list):
        compat = compat[0]

    return compat


def get_chosen(root):

    if 'children' in root:
        if 'chosen' in root['children']:
            for k, v in root['children']['chosen']['props'].items():
                chosen[k] = v

    return


def get_phandles(root, name, handles):

    if 'props' in root:
        handle = root['props'].get('phandle')
        enabled = root['props'].get('status')

    if enabled == "disabled":
        return

    if handle is not None:
        phandles[handle] = name

    if name != '/':
        name += '/'

    if isinstance(root, dict):
        if root['children']:
            for k, v in root['children'].items():
                get_phandles(v, name + k, handles)

    return


def insert_defs(node_address, defs, new_defs, new_aliases):
    if node_address in defs:
        if 'aliases' in defs[node_address]:
            defs[node_address]['aliases'].update(new_aliases)
        else:
            defs[node_address]['aliases'] = new_aliases

        defs[node_address].update(new_defs)
    else:
        new_defs['aliases'] = new_aliases
        defs[node_address] = new_defs

    return


def find_node_by_path(nodes, path):
    d = nodes
    for k in path[1:].split('/'):
        d = d['children'][k]

    return d


def compress_nodes(nodes, path):
    if 'props' in nodes:
        status = nodes['props'].get('status')

        if status == "disabled":
            return

    if isinstance(nodes, dict):
        reduced[path] = dict(nodes)
        reduced[path].pop('children', None)
        if path != '/':
            path += '/'
        if nodes['children']:
            for k, v in nodes['children'].items():
                compress_nodes(v, path + k)

    return


def get_node_label(node_compat, node_address):

    def_label = convert_string_to_label(node_compat.upper())
    if '@' in node_address:
        def_label += '_' + node_address.split('@')[-1].upper()
    else:
        def_label += convert_string_to_label(node_address.upper())

    return def_label


