project(
  'hipo4',
  'cpp',
  meson_version: '>=1.2',
  default_options: {
    'cpp_std':         'c++17',
    'buildtype':       'release',
    'libdir':          'lib',
    'default_library': 'both',
    'pkgconfig.relocatable': 'true',
  },
  version: '4.2.0',
)
add_languages('fortran', native: false, required: get_option('test_fortran'))

# modules
fs  = import('fs')
pkg = import('pkgconfig')

# dependencies
lz4_dep = dependency('liblz4', method: 'pkg-config', version: '>=1.9', fallback: ['lz4', 'liblz4'])

# preprocessor
lz4_preproc_def = '-D__LZ4__'
add_project_arguments(lz4_preproc_def, language: ['cpp'])

# build main project
project_libs = []
subdir('hipo4')
subdir('examples')
if(get_option('test_fortran'))
  subdir('examples/fortran')
endif

# build extensions
if(get_option('dataframes'))
  subdir('extensions' / 'dataframes')
endif

# packaging
pkg.generate(
  name:        meson.project_name(),
  description: 'High Performance Output Data format for experimental Physics',
  libraries:   project_libs,
  requires:    [ lz4_dep ],
)
