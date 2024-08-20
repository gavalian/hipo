dataframe_headers = files('RHipoDS.hxx')
dataframe_sources = files('RHipoDS.cxx')
dataframe_linkdef = files('RHipoDS_LinkDef.h')

# generate ROOT dictionary
dataframe_basename = 'HipoDataFrame'
dataframe_lib_ext  = host_machine.system() == 'darwin' ? 'dylib' : 'so' # FIXME: platform dependence is meson's job, not ours
dataframe_lib_file = f'lib@dataframe_basename@.@dataframe_lib_ext@'
dataframe_dict_tgt = custom_target(
  '@dataframe_basename@Dict',
  input: dataframe_headers + hipo_headers + hipo_sources + dataframe_linkdef,
  output: [
    f'@dataframe_basename@_Dict.cxx',
    f'lib@dataframe_basename@_rdict.pcm',
    f'lib@dataframe_basename@.rootmap',
  ],
  install_dir: [
    false,                # generated .cxx file, don't install
    get_option('libdir'), # generated .pcm file
    get_option('libdir'), # generated .rootmap file
  ],
  install: true,
  command: [
    ROOT_cling,
    '-v2',
    '-f',   '@OUTPUT0@',
    '-s',   meson.current_build_dir() / dataframe_lib_file,
    '-rml', dataframe_lib_file,
    '-rmf', '@OUTPUT2@',
    lz4_preproc_def,
    '-I' + ROOT_incdir,
    '-I' + hipo_incdir,
  ] + dataframe_headers + dataframe_linkdef,
)

# library
dataframe_lib = library(
  dataframe_basename,
  dataframe_sources + [ dataframe_dict_tgt[0] ],
  include_directories: [ hipo_inc, include_directories(ROOT_incdir) ],
  link_with:           [ hipo_lib ],
  link_args:           [ '-L' + ROOT_libdir, '-lCore', '-lROOTDataFrame', '-lROOTVecOps' ],
  build_rpath:         ROOT_libdir,
  cpp_args:            [ '-Wno-sign-compare' ], # FIXME: fix these warnings
  install: true
)
install_headers(dataframe_headers, subdir: meson.project_name())
project_libs += dataframe_lib

# make sure the library name used in rootcling matches that of `dataframe_lib` (see above 'FIXME' regarding platform dependence)
if(dataframe_lib_file != fs.name(dataframe_lib.full_path()))
  error(f'dataframe library name mismatch; perhaps you are not building on Linux of macOS; please contact the maintainers')
endif
