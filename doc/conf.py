# -*- coding: utf-8 -*-
"""RTI OPC UA/DDS Gateway documentation build configuration file."""
# This file is execfile()d with the current directory set to its
# containing dir.
#
# Note that not all possible configuration values are present in this
# autogenerated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
sys.path.insert(0, os.path.abspath('.'))
from extlinks import *

# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['sphinx.ext.extlinks',
              'sphinx.ext.mathjax' ]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
source_suffix = '.rst'

# The encoding of source files.
#
# source_encoding = 'utf-8-sig'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = 'RTI OPC UA/DDS Gateway'
copyright = '2021, Real-Time Innovations, Inc'
author = 'Real-Time Innovations, Inc.'

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = RTI_VERSION
# The full version, including alpha/beta/rc tags.
release = RTI_VERSION

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = "en"

# There are two options for replacing |today|: either, you set today to some
# non-false value, then it is used:
#
# today = ''
#
# Else, today_fmt is used as the format for a strftime call.
#
# today_fmt = '%B %d, %Y'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
exclude_patterns = ['build', 'Thumbs.db', '.DS_Store']

# The reST default role (used for this markup: `text`) to use for all
# documents.
#
# default_role = None

# If true, '()' will be appended to :func: etc. cross-reference text.
#
# add_function_parentheses = True

# If true, the current module name will be prepended to all description
# unit titles (such as .. function::).
#
# add_module_names = True

# If true, sectionauthor and moduleauthor directives will be shown in the
# output. They are ignored by default.
#
# show_authors = False

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# A list of ignored prefixes for module index sorting.
# modindex_common_prefix = []

# If true, keep warnings as "system message" paragraphs in the built documents.
# keep_warnings = False

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False

numfig = True
numfig_format = {'figure': 'Figure %s',
                 'table': 'Table %s',
                 'code-block': 'Listing %s',
                 'section': 'Section %s'}

# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'
# html_theme_path = [sphinx_rtd_theme.get_html_theme_path()]
# Override default css to get a larger width for local build


def setup(app):
    app.add_css_file('theme_overrides.css')

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
# html_theme_options = {}

# Add any paths that contain custom themes here, relative to this directory.
# html_theme_path = []

# The name for this set of Sphinx documents.
# "<project> v<release> documentation" by default.
#
# html_title = u'RTI Web Integration Service v5.2.3'

# A shorter title for the navigation bar.  Default is the same as html_title.
#
# html_short_title = None

# The name of an image file (relative to this directory) to place at the top
# of the sidebar.
#
html_logo = "static/rti-logo-FINALv2-White-OrangeDot.png"

# The name of an image file (relative to this directory) to use as a favicon of
# the docs.  This file should be a Windows icon file (.ico) being 16x16 or
# 32x32 pixels large.
#
html_favicon = "static/favicon.ico"

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['static']

# Add any extra paths that contain custom files (such as robots.txt or
# .htaccess) here, relative to this directory. These files are copied
# directly to the root of the documentation.
#
# html_extra_path = []

# If not None, a 'Last updated on:' timestamp is inserted at every page
# bottom, using the given strftime format.
# The empty string is equivalent to '%b %d, %Y'.
#
# html_last_updated_fmt = None

# If true, SmartyPants will be used to convert quotes and dashes to
# typographically correct entities.
#
# html_use_smartypants = True

# Custom sidebar templates, maps document names to template names.
#
html_sidebars = {
    '**': [
        'relations.html',  # needs 'show_related': True theme option to display
        'searchbox.html',
    ]
}

# Additional templates that should be rendered to pages, maps page names to
# template names.
#
# html_additional_pages = {}

# If false, no module index is generated.
#
# html_domain_indices = True

# If false, no index is generated.
#
# html_use_index = True

# If true, the index is split into individual pages for each letter.
#
# html_split_index = False

# If true, links to the reST sources are added to the pages.
#
html_show_sourcelink = False

# If true, "Created using Sphinx" is shown in the HTML footer. Default is True.
#
html_show_sphinx = True

# If true, "(C) Copyright ..." is shown in the HTML footer. Default is True.
#
# html_show_copyright = True

# If true, an OpenSearch description file will be output, and all pages will
# contain a <link> tag referring to it.  The value of this option must be the
# base URL from which the finished HTML is served.
#
# html_use_opensearch = ''

# This is the file name suffix for HTML files (e.g. ".xhtml").
# html_file_suffix = None

# Language to be used for generating the HTML full-text search index.
# Sphinx supports the following languages:
#   'da', 'de', 'en', 'es', 'fi', 'fr', 'hu', 'it', 'ja'
#   'nl', 'no', 'pt', 'ro', 'ru', 'sv', 'tr', 'zh'
#
# html_search_language = 'en'

# A dictionary with options for the search language support, empty by default.
# 'ja' uses this config value.
# 'zh' user can custom change `jieba` dictionary path.
#
# html_search_options = {'type': 'default'}

# The name of a javascript file (relative to the configuration directory) that
# implements a search results scorer. If empty, the default will be used.
#
# html_search_scorer = 'scorer.js'

# Output file base name for HTML help builder.
htmlhelp_basename = 'RTIDDSOPCUADoc'

# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    'papersize': 'letterpaper',
    'releasename': 'Version',
    'fncychap': '',
    'printindex': '\\footnotesize\\raggedright\\printindex',
    'inputenc': '',
    'pointsize': '11pt',
    'figure_align': 'H',
    'preamble': '''\
        \\sphinxsetup{TitleColor={named}{black},InnerLinkColor={named}{black},OuterLinkColor={named}{blue}}
        \\usepackage[utf8]{inputenc}
        \\usepackage[titles]{tocloft}
        \\usepackage{pmboxdraw}
        \\usepackage{charter}
        \\usepackage[defaultsans]{lato}
        \\usepackage{inconsolata}
        \\usepackage{emptypage}
        \\usepackage{afterpage}
        \\usepackage{newunicodechar}
        \\usepackage{multirow}
        \\usepackage{hyperref}
        \\usepackage{graphicx}
        \\usepackage{caption}
        \\setkeys{Gin}{width=.85\\textwidth}
        \\hypersetup{bookmarksnumbered}
        \\setcounter{tocdepth}{3}
        \\setcounter{secnumdepth}{4}
        \\usepackage[draft]{minted}
        \\fvset{breaklines=true, breakanywhere=true}
    ''',
    'printindex': '\\footnotesize\\raggedright\\printindex',
    'inputenc': '',
    'utf8extra': '',
    # main page
    'maketitle': '''\
        \\pagenumbering{Roman} %%% to avoid page 1 conflict with actual page 1

        \\begin{titlepage}
            \\centering

            \\vspace*{50mm} %%% * is used to give space from top
            \\textbf{\\Huge{''' + project + '''}}

            \\vspace{30mm}
            \\textbf{\\Large{User\'s Manual}}

            \\vspace{7mm}
            \\Large{Version ''' + version + '''}

            \\begin{figure}[b]
                \centering
                \\includegraphics[width=60mm]{rti-logo-tag-StackedRight.png}
            \\end{figure}

            \\vspace{0mm}

        \\end{titlepage}
    '''
}

# latex_engine = "xelatex"
latex_toplevel_sectioning = "chapter"
latex_use_latex_multicolumn = True

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [(master_doc,
                   'RTI_DDS_OPCUA_Gateway_UsersManual.tex',
                    u'RTI OPC UA/DDS Gateway',
                    u'Real-Time Innovations, Inc.', 'manual'),
                   ]

latex_use_modindex = True

latex_logo = "static/rti-logo-tag-StackedRight.png"


# For "manual" documents, if this is true, then toplevel headings are parts,
# not chapters.
#
# latex_use_parts = False

# If true, show page references after internal links.
#
# latex_show_pagerefs = False

# If true, show URL addresses after external links.
#
# latex_show_urls = False

# Documents to append as an appendix to all manuals.
#
# latex_appendices = []

# It false, will not define \strong, \code, 	itleref, \crossref ... but only
# \sphinxstrong, ..., \sphinxtitleref, ... To help avoid clash with user added
# packages.
#
# latex_keep_old_macro_names = True

# If false, no module index is generated.
#
# latex_domain_indices = True


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [(master_doc,
              'rtiddsopcuagateway',
              u'RTI OPC UA/DDS Gateway',
              [author], 1)
             ]

# If true, show URL addresses after external links.
#
# man_show_urls = False


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [(master_doc,
                     'RTIDDSOPCUAGateway',
                      u'RTI OPC UA/DDS Gateway Service',
                      author,
                      'RTIDDSOPCUAGateway',
                      'RTI OPC UA/DDS Gateway is an out-of-the-box solution for integrating OPC UA applications with RTI Connext DDS.',
                      'Software'),
                     ]

# Documents to append as an appendix to all manuals.
#
# texinfo_appendices = []

# If false, no module index is generated.
#
# texinfo_domain_indices = True

# How to display URL addresses: 'footnote', 'no', or 'inline'.
#
# texinfo_show_urls = 'footnote'

# If true, do not generate a @detailmenu in the "Top" node's menu.
#
# texinfo_no_detailmenu = False

# -- Custom roles
from docutils import nodes

def role_litrep(name, rawtext, text, lineno, inliner,
            options={}, content=[]):

    node = nodes.Text(text)
    node.source, node.line = inliner.reporter.get_source_and_line(lineno)
    return [node],[]

from docutils.parsers.rst import roles
roles.register_local_role('litrep', role_litrep)

extlinks = {'link_xml_spec':
                (LINK_XML_SPEC + '%s',
                'link_xml_spec_text'),
            'link_xml_qos_config':
                (LINK_CONNEXT_USER_XML_CONFIG + '%s',
                'link_xml_qos_config_text'),
            'link_xml_cft_config':
                (LINK_CONNEXT_USER_CFT_CONFIG + '%s',
                 'link_xml_cft_config_text'),
            'link_userman_distlog_in_services':
                (LINK_CONNEXT_USER_MAN_DISTLOG + '%s',
                 'link_userman_distlog_in_services'),
            'link_userman_types_in_xml':
                (LINK_CONNEXT_USER_MAN_TYPES_XML + '%s',
                'link_userman_types_in_xml'),
            'link_userman_largedata':
                    (LINK_CONNEXT_USER_MAN_LARGEDATA  + '%s',
                    'link_userman_largedata'),
            'link_router_manual':
                (LINK_ROUTER_MAN + '%s',
                'link_router_manual'),
            'link_router_manual_config':
                (LINK_ROUTER_MAN_CONFIG + '%s',
                'link_router_manual_config'),
            'link_router_manual_config_transformation':
                (LINK_ROUTER_MAN_CONFIG_TRANSFORMATION + '%s',
                'link_router_manual_config_transformation'),
            'link_router_manual_tutorials':
                (LINK_ROUTER_MAN_TUTORIALS + '%s',
                'link_router_manual_tutorials'),
            'link_sqlite_download':
                (LINK_SQLITE_DOWNLOAD + '%s',
                'link_sqlite_download'),
            'link_c_api':
                (LINK_C_API_DOCS + '%s',
                'link_c_api'),
            'link_cpp_api':
                (LINK_CPP_API_DOCS + '%s',
                'link_cpp_api'),
            'link_cpp_api_service':
                (LINK_CPP_API_SERVICE_DOCS + '%s',
                'link_cpp_api_service'),
            'link_community_examples_recording_c_storage':
                (LINK_COMMUNITY_EXAMPLES_RECORDING_C_STORAGE  + '%s',
                'link_community_examples_recording_c_storage'),
            'link_community_examples_recording_cpp_storage':
                (LINK_COMMUNITY_EXAMPLES_RECORDING_CPP_STORAGE  + '%s',
                'link_community_examples_recording_cpp_storage'),
            'link_community_examples_recording_service_admin':
                (LINK_COMMUNITY_EXAMPLES_RECORDING_SERVICE_ADMIN  + '%s',
                'link_community_examples_recording_service_admin'),
            'link_community_examples_recording_service_as_lib':
                (LINK_COMMUNITY_EXAMPLES_RECORDING_SERVICE_AS_LIB  + '%s',
                'link_community_examples_recording_service_as_lib')
}
