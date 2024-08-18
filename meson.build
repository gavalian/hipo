project(
  'hipo4',
  'cpp',
  meson_version: '>=1.2',
  default_options: {
    'cpp_std': 'c++17',
    'buildtype': 'release',
    'libdir': 'lib',
    'default_library': 'both',
    'pkgconfig.relocatable': 'true',
  },
  version: '4.2.0',
)

# modules
fs = import('fs')
pkg = import('pkgconfig')

# dependencies
lz4_dep = dependency('liblz4', method: 'pkg-config', version: '>=1.9', fallback: ['lz4', 'liblz4'])
ROOT_dep = dependency('ROOT', method: 'cmake', version: '>=6.28', required: false)

# preprocessor
lz4_preproc_def = '-D__LZ4__'
add_project_arguments(lz4_preproc_def, language: ['cpp'])

# main project builds
project_libs = []
subdir('hipo4')

# extensions
if(get_option('dataframes'))
  if(ROOT_dep.found())
    subdir('extensions' / 'dataframes')
  else
    warning('ROOT dependency not found; dataframes extension will NOT be built')
  endif
endif

# packaging
pkg.generate(
  name: meson.project_name(),
  description: 'High Performance Output Data format for experimental Physics',
  libraries: project_libs,
  requires: [ lz4_dep ],
)
