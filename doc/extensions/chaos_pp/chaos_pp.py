# Copyright (c) 2018 Bobby Noelte.
#
# SPDX-License-Identifier: Apache-2.0

from pathlib import Path

from docutils import nodes
from docutils.statemachine import ViewList
from docutils.parsers.rst import Directive
from docutils.parsers.rst import directives

from sphinx import addnodes
from sphinx.util.nodes import nested_parse_with_titles
from sphinx.ext.autodoc import AutodocReporter

import xml.etree.ElementTree as ET


class ChaosPPDocDirective(Directive):
    '''Directive to generate Chaos documentation.

    Example to generate documentation for all macros in
    a header file:

    .. chaos-pp-doc::
       :header: chaos/preprocessor/cat.h

    Example to generate documentation for a macro:

    .. chaos-pp-doc::
       :header: chaos/preprocessor/cat.h
       :macro: CAT

    Example to generate documentation:

    .. chaos-pp-doc::
       :document: arrays.xml
       :contents:

    Directive options:

    - :header: chaos header or header directory to be documented
    - :macro: macro to be documented
    '''
    has_content = False
    required_arguments = 0
    optional_arguments = 0
    final_argument_whitespace = False
    option_spec = {
        'header': directives.unchanged,
        'document': directives.unchanged,
        'section_level': directives.unchanged,
        'contents': directives.unchanged,
        'debug': directives.unchanged,
    }
    ZEPHYR_PATH = Path(__file__).parents[3]
    CHAOS_PATH = ZEPHYR_PATH.joinpath('ext', 'lib', 'preprocessor', 'chaos')
    CHAOS_DOC_PATH = CHAOS_PATH.joinpath('documentation')
    SECTION_LEVEL_MARKUP= '#*=-^"'

    def path_walk(self, top, topdown = False, followlinks = False):
        """
            See Python docs for os.walk, exact same behavior but it yields Path() instances instead

            Form: http://ominian.com/2016/03/29/os-walk-for-pathlib-path/
        """
        names = list(top.iterdir())

        dirs = (node for node in names if node.is_dir() is True)
        nondirs = (node for node in names if node.is_dir() is False)

        if topdown:
            yield top, dirs, nondirs

        for name in dirs:
            if followlinks or name.is_symlink() is False:
                for x in self.path_walk(name, topdown, followlinks):
                    yield x

        if topdown is not True:
            yield top, dirs, nondirs

    ##
    # @param target string describing the target
    #
    def _target_id(self, target, target_type):
        if not isinstance(target, str):
            raise self.error(
                '_target_id expects string - got  "{}".'.format(
                str(target)))
        target_id = target.replace("/", "-")  \
                          .replace("\\", "-") \
                          .replace(" ", "-")  \
                          .replace("\t", "-") \
                          .replace("_", "-")  \
                          .replace(",", "-")  \
                          .replace(";", "-")  \
                          .replace(".", "-")  \
                          .replace(":", "-")  \
                          .replace("@", "-")
        if target_type == 'macro':
            target_id = 'chaos-pp-{}'.format(target_id)
        elif target_type == 'document':
            pass
        elif target_type == 'header':
            pass
        else:
            raise self.error(
                '_target_id target type "{}" unknown - target: {}.'.format(
                target_type, target))
        target_id = "chaos-pp-{}-{}".format(target_type, target_id.lower())
        return target_id

    ##
    # @param target string describing the target
    #
    def _target_ref(self, target, target_type, context = None):
        if target_type == 'document':
            reference = "\ :ref:`{}\ <{}>`\ ".format(target,
                                self._target_id(target, 'document'))
        elif target_type == 'header':
            # assume this is an internal link to a header file
            target_name = target.replace('-', '/')
            if context is not None and context == 'listing':
                reference = '<{}>'.format(target_name)
            else:
                reference = "\ :ref:`{}`\ ".format(
                                self._target_id(target, 'header'))
        elif target_type == 'macro':
            # assume this is an internal link to a CHAOS_PP macro
            if context is not None and context == 'listing':
                reference = "CHAOS_PP_{}".format(target)
            else:
                if target.endswith('_'):
                    target_name = "CHAOS_PP_{}\_".format(target[:-1])
                else:
                    target_name = "CHAOS_PP_{}".format(target)
                reference = "\ :ref:`{}\ <{}>`\ ".format(
                                target_name, self._target_id(target, 'macro'))
        return reference

    ##
    # combine lines to single line
    def _clean_line(self, lines):
        if not isinstance(lines, list):
            raise self.error(
                '_clean_line expects list - got  "{}".'.format(
                str(lines)))
        for line in lines:
            if not isinstance(line, str):
                raise self.error(
                    '_clean_line expects string - got  "{}".'.format(
                    str(line)))
        words = []
        for line in lines:
            words.extend(line.split())
        for idx, word in enumerate(words):
            if word.endswith('\\'):
                words[idx] = word + ' '
        return " ".join(words).strip()

    def _para_indent(self, para):
        indent = -1;
        for line in para:
            if len(line) == 0:
                continue
            line = line.expandtabs(8)
            if line.isspace():
                if indent == -1 or len(line) < indent:
                    indent = len(line)
                continue
            for idx, c in enumerate(line[:indent]):
                if not c == ' ':
                    indent = idx
                    break
        if indent < 0:
            indent = 0

        return indent

    def _clean_para(self, lines):
        if not isinstance(lines, list):
            raise self.error(
                '_clean_para expects list - got  "{}".'.format(
                str(lines)))
        for line in lines:
            if not isinstance(line, str):
                raise self.error(
                    '_clean_para expects string - got  "{}".'.format(
                    str(line)))

        para = []
        for line in lines:
            para.extend(line.splitlines())
        # strip empty lines
        # expand all tabs
        lines = para
        para = []
        for line in lines:
            if len(line) > 0:
                para.append(line.expandtabs(8))
        # remove leading and trailing white space line
        if para[0].isspace():
            para.pop(0)
        if para[-1].isspace():
            para.pop(-1)

        indent = self._para_indent(para)

        # strip white space lines with exact indent
        # remove minimum leading spaces
        # Assure there is a empty line before/after .. or ::
        lines = para
        para = []
        for idx, line in enumerate(lines):
            if line.isspace() and len(line) <= indent:
                continue
            if line[indent:] == '::':
                para.extend(["", '::', ""])
                continue
            if line[indent:].startswith('.. '):
                if (idx < len(lines) - 1) and \
                    lines[idx + 1].startswith(' ') and \
                    (' :' in lines[idx + 1]):
                    para.append("")
                else:
                    para.extend(["", line[indent:], ""])
                    continue
            para.append(line[indent:])

        # if we still have indented lines - we have to separate them
        lines = para
        para = []
        indent = 0
        for line in lines:
            for idx, c in enumerate(line):
                if not c == ' ':
                    if (idx > 0 and indent == 0) or (idx == 0 and indent > 0):
                        para.append("")
                    indent = idx
                    break
            para.append(line)
        return para

    def _clean_listing(self, lines):
        if not isinstance(lines, list):
            raise self.error(
                '_clean_listing expects list - got  "{}".'.format(
                str(lines)))
        for line in lines:
            if not isinstance(line, str):
                raise self.error(
                    '_clean_listing expects string - got  "{}".'.format(
                    str(line)))

        para = []
        for line in lines:
            para.extend(line.splitlines())
        # strip empty lines
        # expand all tabs
        # Replace some wired character coding '??='
        lines = para
        para = []
        for line in lines:
            if len(line) > 0:
                para.append(line.expandtabs(8))
        # remove leading and trailing white space line
        if para[0].isspace():
            para.pop(0)
        if para[-1].isspace():
            para.pop(-1)

        indent = self._para_indent(para)

        # remove minimum leading spaces
        if indent > 0:
            for idx, line in enumerate(para):
                if len(line) < indent:
                    para[idx] = '{}'.format(' ' * indent)
                else:
                    para[idx] = line[indent:]
        return para

    ##
    # @param lines List of line string
    def _add_lines(self, content, lines, inline=False):
        if not isinstance(lines, list):
            raise self.error(
                '_add_lines expects list - got  "{}".'.format(
                str(lines)))
        for line in lines:
            if not isinstance(line, str):
                raise self.error(
                    '_add_lines expects string - got  "{}".'.format(
                    str(line)))

        if len(lines) == 0:
            return
        if inline is False:
            content.extend(lines)
            return
        if len(content) == 0:
            content.append(lines[0])
        else:
            content[-1] = '{}{}'.format(content[-1], lines[0])
        if len(lines) > 1:
            content.extend(lines[1:])

    def _section_title(self, title, level):
        if not isinstance(title, str):
            raise self.error(
                '_section_title expects string - got  "{}".'.format(
                str(title)))
        title = self._clean_line([title])
        heading = []
        heading.append(title)
        heading.append(self.SECTION_LEVEL_MARKUP[level] * len(title))
        return heading

    def _macro_definition(self, header, macro_id, macro_args, paramlist,
                          description, paramparalist):
        macro = []
        if macro_id.endswith('_'):
            macro_name = "CHAOS_PP_{}\_".format(macro_id[:-1])
        else:
            macro_name = "CHAOS_PP_{}".format(macro_id)
        if macro_args is None:
            macro_args = ''
        macro.append("**{}{}**".format(macro_name, macro_args))
        header_ref = self._target_ref(header, 'header')
        macro.append("    #include {}".format(header_ref))
        macro.append("")
        if description is not None and len(description) > 0:
            for descr in description:
                macro.append("    " + descr)
            macro.append("")
        if paramlist is not None and len(paramlist) > 0:
            macro.append("    **Parameters:**")
            macro.append("")
            for param in paramlist:
                macro.append("    * " + param)
            macro.append("")
        if paramparalist is not None:
            for para in paramparalist:
                macro.extend(para)
                macro.append("")
        return macro


    def _derivative_definition(self, header, macro_id, macro_args):
        macro_ref = self._target_ref(macro_id, 'macro')

        if macro_id.endswith('_'):
            parent_ref = self._target_ref(macro_id[:-1], 'macro')
            document_ref = self._target_ref('λ expression', 'document')
            abstract = \
                'The {} macro expands to a {} representing {}.' \
                .format(macro_ref, document_ref, parent_ref)
            macro_args = None
        elif macro_id.endswith('_BYPASS'):
            parent_ref = self._target_ref(macro_id[:-7], 'macro')
            document_ref = self._target_ref('bypass recursion state', 'document')
            abstract = \
                'The {} macro behaves identically to {} except that it is parameterized by the {}.' \
                .format(macro_ref, parent_ref, document_ref)
        elif macro_id.endswith('_ID'):
            parent_ref = self._target_ref(macro_id[:-3], 'macro')
            abstract = \
                "The {} macro indirectly expands to {}." \
                .format(macro_ref, parent_ref)
            macro_args = '()'
        elif macro_id.endswith('_S'):
            parent_ref = self._target_ref(macro_id[:-2], 'macro')
            document_ref = self._target_ref('recursion state', 'document')
            abstract = \
                "The {} macro behaves identically to {} except that it is parameterized by the {}." \
                .format(macro_ref, parent_ref, document_ref)
        else:
            raise self.error(
                '_derivative_definition - got unexpected macro id "{}".'.format(
                str(macro_id)))
        description = []
        description.append(abstract)

        return self._macro_definition(header, macro_id, macro_args,
                                      None, description, None)

    ##
    # xml to rst
    #
    #
    def _xml_to_rst(self, xml, data):
        rst = []
        tag = xml.tag
        if data.get('tag') is None:
            data['tag'] = []
        data['tag'].insert(0, tag)
        if tag is None:
            pass
        elif tag in ('em', 'var', 'x-var', 'code'):
            para = []
            if xml.text is not None:
                para.append('\ **{}**\ '.format(xml.text))
                data['inline_next'] = True
            # no children assumed
            inline = data['inline_next']
            rst.extend(para)
        elif tag == 'sup':
            para = []
            if xml.text is not None:
                para.append('\ :sup:`{}`\ '.format(xml.text))
                data['inline_next'] = True
            # no children assumed
            inline = data['inline_next']
            rst.extend(para)
        elif tag in ('para', 'overview', 'abstract', 'listing', 'snippet'):
            para = []
            data['inline_next'] = False
            if tag in ('listing', 'snippet'):
                context = data['context']
                data['context'] = 'listing'
            if xml.text is not None:
                lines_raw = xml.text.splitlines()
                lines = []
                for line in lines_raw:
                    if len(lines) > 0:
                        # we are past the start
                        lines.append(line)
                    elif len(line) > 0:
                        lines.append(line)
                if len(lines) > 0:
                    self._add_lines(para, lines, data['inline_next'])
                    if xml.text.endswith('\n'):
                        data['inline_next'] = False
                    else:
                        data['inline_next'] = True
            inline = data['inline_next']
            for child in xml:
                if child.tag == 'snippet':
                    # assure we do not inline snippets
                    inline = False
                    data['para_indent'] = self._para_indent([para[-1]])
                lines = self._xml_to_rst(child, data)
                self._add_lines(para, lines, inline)
                if child.tag == 'snippet':
                    para.append(' ' * data['para_indent'])
                    data['inline_next'] = True
                if child.tail is not None:
                    lines_raw = child.tail.splitlines()
                    lines = []
                    if data['inline_next']:
                        lines = lines_raw
                    else:
                        for line in lines_raw:
                            if len(lines) > 0:
                                # we are past the start
                                lines.append(line)
                            elif len(line) > 0:
                                lines.append(line)
                    if len(lines) > 0:
                        self._add_lines(para, lines, data['inline_next'])
                        if child.tail.endswith('\n'):
                            data['inline_next'] = False
                        else:
                            data['inline_next'] = True
                inline = data['inline_next']
            data['inline_next'] = False
            if tag == 'snippet':
                para = self._clean_listing(para)
                indent = data.get('para_indent', 0)
                for idx, line in enumerate(para):
                    para[idx] = "{}{}".format(' ' * (indent + 4), line)
                para.insert(0, "")
                para.insert(0, "{}.. code-block:: text".format(' ' * indent))
                para.insert(0, "")
            elif tag == 'listing':
                para = self._clean_listing(para)
            else:
                para = self._clean_para(para)
            if tag == 'overview':
                data['overview'] = para
            elif tag == 'abstract':
                data['description'] = para
            elif tag == 'listing':
                for idx, line in enumerate(para):
                    para[idx] = "    {}".format(line)
                para.insert(0, "")
                para.insert(0, '.. code-block:: C')
                data['listinglist'].append(para)
            elif tag == 'para' and data['tag'][1] == 'para':
                # indent para within para
                for idx, line in enumerate(para):
                    para[idx] = "    {}".format(line)
                #rst.extend(para) @TODO !!!!!!!!!!!!!!!!!!!!!
            elif tag == 'para' and data['tag'][1] == 'macro':
                data['paralist'].append(para)
            elif tag == 'para' and data['tag'][1] == 'param':
                data['paramparalist'].append(para)
            else:
                # para outside of macro, snippet, para
                rst.extend(para)
            if tag in ('listing', 'snippet'):
                data['context'] = context
        elif tag == 'syntax':
            args = []
            if xml.text is not None:
                args.extend(xml.text.splitlines())
            for child in xml:
                if child.text is not None:
                    args.extend(child.text.splitlines())
                if child.tail is not None:
                    args.extend(child.tail.splitlines())
            args = self._clean_line(args)
            if len(args) == 0 or 'define' in args or 'include' in args:
                if 'define' in args:
                    data['description'].extend(["", "#define CHAOS_PP_{} **{}**".format(data['macro_id'], child.tail.strip()), ""])
                elif '#include' in args:
                    data['description'].extend(["", "#include CHAOS_PP_{}{}".format(data['macro_id'], child.tail.strip()), ""])
                elif '??=include' in args:
                    data['description'].extend(["", "??=include CHAOS_PP_{}{}".format(data['macro_id'], child.tail.strip()), ""])
                data['macro_args'] = None
            else:
                data['macro_type'] = 'function'
                data['macro_args'] = args
        elif tag == 'param':
            name = xml.get('id')
            optional = xml.get('optional')
            desc = []
            data['paramparalist'] = []
            param_has_para = False
            param_has_complex = False
            if xml.text is not None:
                desc.extend(xml.text.splitlines())
            for child in xml:
                if child.tag == 'para':
                    param_has_para = True
                elif child.tag in ('snippet', 'listing'):
                    param_has_complex = True
                desc.extend(self._xml_to_rst(child, data))
                if child.tail is not None:
                    desc.extend(child.tail.splitlines())
            if param_has_para:
                data['paramparalist'].insert(0, ["\ **{}**\ :".format(name)])
                data['parametersparalist'].extend(data['paramparalist'])
                desc = 'see description.'
            elif param_has_complex:
                data['paramparalist'].insert(0, self._clean_para(desc))
                data['paramparalist'].insert(0, ["\ **{}**\ :".format(name)])
                data['parametersparalist'].extend(data['paramparalist'])
                desc = 'see description.'
            else:
                desc = self._clean_line(desc)
            data['parameterlist'].append("**{}** : {}".format(name, desc))
        elif tag == 'usage':
            lang = xml.get('lang')
            data['inline_next'] = False
            if lang is None or lang == 'C99':
                data['lang'] = lang
                data['parameterlist'] = []
                data['parametersparalist'] = []
                data['macro_args'] = None
                # usage text is not of interest
                for child in xml:
                    self._xml_to_rst(child, data)
                    # children's tails are not of interest
                data['macro_def'].extend(
                    self._macro_definition(data['self'],
                                           data['macro_id'],
                                           data['macro_args'],
                                           data['parameterlist'],
                                           data['description'],
                                           data['parametersparalist']))
                data['inline_next'] = False
        elif tag == 'derivative':
            derivative_id = xml.get('id')
            if derivative_id.endswith('_'):
                derivative_macro = "CHAOS_PP_{}\_".format(derivative_id[:-1])
            else:
                derivative_macro = "CHAOS_PP_{}".format(derivative_id)
            derivative = []
            # Derivative section title
            derivative.append(
                ".. index:: single: CHAOS_PP, {}\ (C\ {}), c.{}"
                .format(derivative_macro, data['macro_type'], derivative_macro))
            derivative.append("")
            derivative.append('.. _{}:'.format(self._target_id(derivative_id,
                                                         'macro')))
            derivative.append("")
            derivative.extend(self._section_title(derivative_macro,
                                                  data['section_level']))
            derivative.append("")
            data['section_level'] += 1
            data['macro_args'] = None
            if len(xml) == 0:
                # no more subordinated derivated macros
                derivative.extend(
                    self._derivative_definition(
                        data['self'], derivative_id, data['macro_args']))
            else:
                for child in xml:
                    if child.tag == 'syntax':
                        self._xml_to_rst(child, data)
                        derivative.extend(
                            self._derivative_definition(
                                data['self'], derivative_id, data['macro_args']))
                        derivative.append("")
                    else:
                        self._xml_to_rst(child, data)
            data['derivativelist'].append(derivative)
            data['inline_next'] = False
            data['section_level'] -= 1
        elif tag == 'macro':
            context = data['context']
            data['context'] = 'macro'
            macro = []
            data['macro_id'] = xml.get('id')
            data['macro_group'] = xml.get('group')
            data['macro_type'] = 'macro'
            data['macro_def'] = []
            data['paralist'] = []
            data['listinglist'] = []
            data['derivativelist'] = []
            data['description'] = None
            data['inline_next'] = False
            # Macro section title
            macro.append("")
            macro.append('.. _{}:'.format(self._target_id(data['macro_id'],
                                                         'macro')))
            macro.append("")
            macro.extend(self._section_title(
                            'CHAOS_PP_{}'.format(data['macro_id']),
                            data['section_level']))
            macro.append("")
            data['section_level'] += 1
            # macro text is not of interest
            for child in xml:
                self._xml_to_rst(child, data)
                # child tails are not of interest
            # prepend index
            macro.insert(0,
                ".. index:: single: CHAOS_PP, CHAOS_PP_{}\ (C\ {}), c.CHAOS_PP_{}"
                .format(data['macro_id'], data['macro_type'], data['macro_id']))
            macro.insert(0, "")
            # add the macro definitions
            macro.extend(data['macro_def'])
            for para in data['paralist']:
                macro.extend(para)
                macro.append("")
            for listing in data['listinglist']:
                macro.extend(listing)
                macro.append("")
            for derivative in data['derivativelist']:
                macro.extend(derivative)
                macro.append("")
            data['macrolist'].append(macro)
            data['inline_next'] = False
            data['section_level'] -= 1
            data['context'] = context
        elif tag == 'header':
            context = data['context']
            data['context'] = 'header'
            header = []
            data['self'] = xml.get('name')
            data['overview'] = None
            data['macrolist'] = []
            # header section title
            header.append("")
            header.append('.. _{}:'.format(self._target_id(data['self'], 'header')))
            header.append("")
            header.extend(self._section_title('<{}>'.format(data['self']),
                                               data['section_level']))
            data['section_level'] += 1
            header.append("")
            data['inline_next'] = False
            for child in xml:
                self._xml_to_rst(child, data)
                # child tails are not of interest
            if data['overview'] is not None:
                header.extend(data['overview'])
                header.append("")
            for macro in data['macrolist']:
                header.extend(macro)
                header.append("")
            rst.extend(header)
            data['inline_next'] = False
            data['context'] = context
            data['section_level'] -= 1
        elif tag == 'section':
            section = []
            data['section_title'] = xml.get('id')
            # document section title
            section.extend(self._section_title(data['section_title'],
                                               data['section_level']))
            data['section_level'] += 1
            section.append("")
            if data.get('contents') is not None:
                depth = int(data['contents'])
                section.append(".. contents::")
                section.append("   :depth: {}".format(depth))
                section.append("   :local:")
                section.append("   :backlinks: top")
                section.append("")
                data['contents'] = None
            data['inline_next'] = False
            for child in xml:
                lines = self._xml_to_rst(child, data)
                self._add_lines(section, lines, data['inline_next'])
                # child tails are not of interest
                section.append("")
            rst.extend(section)
            data['inline_next'] = False
            data['section_level'] -= 1
        elif tag == 'document':
            context = data['context']
            data['context'] = 'document'
            document = []
            if data['document_name'][-4:] == '.xml':
                document_name = data['document_name'][:-4]
            else:
                document_name = data['document_name']
            data['inline_next'] = False
            for child in xml:
                # all children are sections
                if child.get('id') is None:
                    # This is the first section
                    # it includes all other sections
                    # The document name as target
                    document.append("")
                    document_id = self._target_id(document_name, 'document')
                    document.append('.. _{}:'.format(document_id))
                    document.append("")
                    # The document header as target
                    document_headline = xml.get('id')
                    header_id = self._target_id(document_headline, 'document')
                    if header_id != document_id:
                        document.append('.. _{}:'.format(header_id))
                        document.append("")
                    child.set('id', document_headline)
                    lines = self._xml_to_rst(child, data)
                    self._add_lines(document, lines, data['inline_next'])
                else:
                    # These are secondary sections
                    data['section_level'] += 1
                    lines = self._xml_to_rst(child, data)
                    self._add_lines(document, lines, data['inline_next'])
                    data['section_level'] -= 1
                # children's tails are not of interest
            rst.extend(document)
            data['inline_next'] = False
            data['context'] = context
        elif tag == 'link' or tag == 'self' or tag.isupper() or 'chaos-preprocessor-' in tag:
            para = []
            # no link text assumed
            # no link children assumed
            if tag == 'link':
                reference = self._target_ref(xml.get('id'), 'document')
                #reference = "\ :ref:`{} <{}>`\ ".format(xml.get('id'),
                #                    self._target_id(xml.get('id'), 'document'))
            elif tag == 'self':
                reference = self._target_ref(data['self'], 'header')
                #reference = ":ref:`{}`".format(self._target_id(data['self'], 'header'))
            elif 'chaos-preprocessor-' in tag:
                # assume this is an internal link to a header file
                reference = self._target_ref(tag, 'header', data['context'])
                #if data['context'] == 'listing':
                #    reference = '<{}>'.format(tag.replace('-', '/'))
                #else:
                #    reference = "\ :ref:`{}`\ ".format(self._target_id(tag, 'header'))
            elif tag.isupper():
                # assume this is an internal link to a CHAOS_PP macro
                reference = self._target_ref(tag, 'macro', data['context'])
                #if data['context'] == 'listing':
                #    reference = 'CHAOS_PP_{}'.format(tag)
                #else:
                #    reference = "\ :ref:`{}`\ ".format(self._target_id(tag, 'macro'))
            self._add_lines(para, [reference, ], data['inline_next'])
            data['inline_next'] = True
            rst.extend(para)

        data['tag'].pop(0)
        return rst

    ##
    #
    # @param document xml node
    # @param document_name Name of document
    # @param context one of 'document', 'header'
    # @param section_level
    # @param contents Add contents directive to first section
    # @param debug_log file path for debug log or None
    # @param debug_append If True append to log otherwise create new.
    # @return list of nodes
    #
    def _generate(self, document, document_name, context, section_level,
                  contents, debug_log, debug_append):
        data = {
            'context': context,
            'document_name': document_name,
            'section_level': section_level,
            'contents': contents,
        }


        document_rst = self._xml_to_rst(document, data)
        document_content = ViewList(document_rst, source=document_name)
        document_text = '\n'.join(document_rst)
        # Prepare a dummy section node
        title = document.get('id', document_name)
        document_id = self._target_id(title, context)
        document_node = nodes.section(rawsource=document_text, ids=[document_id])

        if debug_log is not None:
            if debug_append:
                write_mode = 'a'
            else:
                write_mode = 'w'
            log = open(debug_log, write_mode)
            log.write("\n\n--------- {} --------------\n{}".
                      format(document_name, document_text))
            log.close()

        # parse the rst including titles
        self.state.memo.reporter = AutodocReporter(document_content,
                                                   self.state.memo.reporter)
        nested_parse_with_titles(self.state, document_content, document_node)

        if debug_log is not None:
            log = open(debug_log, 'a')
            log.write("\n\n--------- parsed nodes for {} -----------\n".
                      format(document_name))
            log.write(document_node.pformat(level = 1))
            log.close()

        content_nodes = []
        for idx in range(0, len(document_node)):
            if isinstance(document_node[idx], nodes.target) or \
                isinstance(document_node[idx], nodes.section):
                    # propagate up
                    content_nodes.append(document_node[idx])
        if len(content_nodes) == 0 or \
            (len(content_nodes) != len(document_node)):
            # could not propagate up
            # use dummy section node
            document_title = nodes.title(text=title)
            document_node.insert(0, document_title)
            content_nodes.append(document_node)

        if debug_log is not None:
            log = open(debug_log, 'a')
            log.write("\n\n--------- final nodes for {} -----------\n".
                      format(document_name))
            for idx, node in enumerate(content_nodes):
                log.write("\n-- node: {} --\n\n{}".format(idx, node.pformat(level = 1)))
            log.close()

        return content_nodes

    ##
    # @return list of nodes
    #
    def _generate_document(self, **kwargs):
        document_path = self.CHAOS_DOC_PATH.joinpath(kwargs['document_name'])
        if not document_path.exists():
            raise self.error('document file "{}" does not exist.'.format(
                document_path))

        tree = ET.parse(str(document_path)) # str is workaround for pathlib
                                            # in python 3.5
        document = tree.getroot()

        if document.tag != 'document':
            raise self.error(
                'XML root tag "{}" in "{}", expected "document".'.format(
                document.tag, document_path))

        return self._generate(document, kwargs['document_name'], 'document',
                              kwargs['section_level'],
                              kwargs['document_contents'],
                              kwargs['debug_log'],
                              False)

    ##
    # @return list of nodes
    #
    def _generate_header(self, **kwargs):
        header_path = self.CHAOS_PATH.joinpath(kwargs['header_name'])
        if not header_path.exists():
            raise self.error('header file "{}" does not exist.'.format(
                header_path))

        header_doc_name = kwargs['header_name'][:-1] + 'xml'
        header_doc_path = self.CHAOS_DOC_PATH.joinpath(header_doc_name)
        if not header_doc_path.exists():
            raise self.error(
                'header documentation file "{}" does not exist.'.format(
                header_doc_path))


        tree = ET.parse(str(header_doc_path)) # str is workaround for pathlib
                                              # in python 3.5
        header = tree.getroot()

        if header.tag != 'header':
            raise self.error(
                'XML root tag "{}" in "{}", expected "header".'.format(
                header.tag, header_doc_path))

        return self._generate(header, header_doc_name, 'header',
                              kwargs['section_level'], None,
                              kwargs['debug_log'],
                              kwargs.get('debug_append', False))

    ##
    # @return list of nodes
    #
    def _generate_dir(self, **kwargs):
        content_nodes = []
        headers = []
        header_dir_path = self.CHAOS_PATH.joinpath(kwargs['header_name'])

        if kwargs['debug_log'] is not None:
            log = open(kwargs['debug_log'], 'w')
            log.write("--------- {} --------------\n".
                      format(kwargs['header_name']))
            log.close()

        for path, dnames, fnames in self.path_walk(header_dir_path):
            headers.extend([x for x in fnames if x.suffix == '.h'])
        for header in sorted(headers):
            run_config = {
                'header_name': str(header)[len(str(self.CHAOS_PATH)) + 1:],
                'macro_name': None,
                'section_level': kwargs['section_level'],
                'debug_log': kwargs['debug_log'],
                'debug_append' : True,
            }
            if 'detail' in run_config['header_name']:
                # detail headers are not documented
                continue
            content_nodes.extend(self._generate_header(**run_config))
        return content_nodes

    ##
    # @return list of nodes
    #
    def run(self):
        # Re-run on the current document if this directive's source changes.
        self.state.document.settings.env.note_dependency(__file__)

        header_name = self.options.get('header', None)
        debug_log = self.options.get('debug', None)
        document_name = self.options.get('document', None)
        document_contents = self.options.get('contents', None)
        section_level = int(self.options.get('section_level', 0))

        if header_name is None and document_name is None:
            raise self.error('header or document definition missing.')

        run_config = {
            'header_name': header_name,
            'document_name': document_name,
            'document_contents': document_contents,
            'section_level': section_level,
            'debug_log': debug_log,
            }

        content_nodes = []

        if header_name is not None:
            header_path = self.CHAOS_PATH.joinpath(header_name)
            if not header_path.exists():
                raise self.error('header file "{}" does not exist.'.format(
                    header_path))

            if header_path.is_dir():
                content_nodes.extend(self._generate_dir(**run_config))
            else:
                content_nodes.extend(self._generate_header(**run_config))

        if document_name is not None:
            document_path = self.CHAOS_DOC_PATH.joinpath(document_name)
            if not document_path.exists():
                raise self.error('document file "{}" does not exist.'.format(
                    document_path))
            content_nodes.extend(self._generate_document(**run_config))

        return content_nodes

def setup(app):
    app.add_directive('chaos-pp-doc', ChaosPPDocDirective)

    return {
        'version': '1.0',
        'parallel_read_safe': True,
        'parallel_write_safe': True
    }
