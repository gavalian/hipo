project(
  'hipo4',
  'cpp',
  meson_version: '>=1.2',
  default_options: {
    'cpp_std':         'c++17',
    'buildtype':       'release',
    'libdir':          'lib',
    'libexecdir':      'libexec',
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
lz4_dep = dependency(
  'liblz4',
  method: 'pkg-config',
  version: '>=1.9',
  fallback: ['lz4', 'liblz4'],
)
ROOT_dep = dependency(
  'ROOT',
  method: 'cmake',
  version: '>=6.28',
  required: false,
  not_found_message: 'ROOT not found; tools which depend on ROOT will not be built.',
)
thread_dep = dependency(
  'threads',
  required: false,
)

# handle ROOT
# NOTE: hipo library does NOT depend on ROOT, but some extensions and examples do
ROOT_config = find_program('root-config', required: ROOT_dep.found())
ROOT_cling  = find_program('rootcling', required: ROOT_dep.found())
ROOT_libdir = ROOT_dep.found() ? run_command(ROOT_config, '--libdir', check: true).stdout().strip() : ''
ROOT_incdir = ROOT_dep.found() ? run_command(ROOT_config, '--incdir', check: true).stdout().strip() : ''

# preprocessor
lz4_preproc_def = '-D__LZ4__'
add_project_arguments(lz4_preproc_def, language: ['cpp'])

# build main project
project_libs = []
subdir('hipo4')

# build examples
if(get_option('build_examples'))
  subdir('examples')
endif
if(ROOT_dep.found() and get_option('install_root_tools'))
  subdir('examples/root')
endif

# build extensions
build_dataframes = get_option('dataframes') and ROOT_dep.found()
if(build_dataframes)
  subdir('extensions' / 'dataframes')
endif

# packaging
pkg.generate(
  name:        meson.project_name(),
  description: 'High Performance Output Data format for experimental physics',
  libraries:   project_libs,
  requires:    [ lz4_dep ],
  variables: [
    'bindir=${prefix}' / get_option('bindir'),
    f'with_dataframes=@build_dataframes@',
  ],
)
subdir('meson')
