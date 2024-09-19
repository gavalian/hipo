hipo_sources = files(
  'bank.cpp',
  'datastream.cpp',
  'dictionary.cpp',
  'event.cpp',
  'fusion.cpp',
  'node.cpp',
  'parser.cpp',
  'reaction.cpp',
  'reader.cpp',
  'record.cpp',
  'recordbuilder.cpp',
  'tuple.cpp',
  'utils.cpp',
  'wrapper.cpp',
  'writer.cpp',
)
hipo_headers = files(
  'bank.h',
  'datastream.h',
  'dictionary.h',
  'event.h',
  'fizika.h',
  'fusion.h',
  'hipoexceptions.h',
  'json.h',
  'jsonutil.h',
  'node.h',
  'parser.h',
  'reaction.h',
  'reader.h',
  'record.h',
  'recordbuilder.h',
  'tuple.h',
  'twig.h',
  'utils.h',
  'writer.h',
)

hipo_lib = library(
  meson.project_name(),
  hipo_sources,
  dependencies: [ lz4_dep ],
  cpp_args: [ # FIXME: fix the warnings; they are suppressed by these args
    '-Wno-sign-compare',
    '-Wno-unused-variable',
    '-Wno-unused-but-set-variable',
    '-Wno-misleading-indentation',
    '-Wno-format',
    '-Wno-shift-overflow',
    ],
  install: true
)

install_headers(hipo_headers, subdir: meson.project_name())
project_libs += hipo_lib

hipo_incdir = meson.current_source_dir()
hipo_inc    = include_directories('.')
