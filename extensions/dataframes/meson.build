dataframe_headers = files('RHipoDS.hxx')
dataframe_sources = files('RHipoDS.cxx')
dataframe_linkdef = files('RHipoDS_LinkDef.h')

# ROOT stuff
ROOT_config = find_program('root-config')
ROOT_cling  = find_program('rootcling')
ROOT_libdir = run_command(ROOT_config, '--libdir', check: true).stdout().strip()
ROOT_incdir = run_command(ROOT_config, '--incdir', check: true).stdout().strip()
ROOT_inc    = include_directories(ROOT_incdir)

# generate ROOT dictionary
dataframe_dict    = 'HipoDataFrame_Dict.cxx'
dataframe_lib_file = meson.current_build_dir() / 'libHipoDataFrame.so' # FIXME: file extension is platform dependent
dataframe_dict_tgt = custom_target(
  'HipoDataFrameDict',
  input: dataframe_headers + hipo_headers + hipo_sources + [ dataframe_linkdef ],
  output: [ dataframe_dict ],
  command: [
    ROOT_cling,
    '-v2',
    '-f', meson.current_build_dir() / dataframe_dict,
    '-s', dataframe_lib_file,
    '-rml', fs.name(dataframe_lib_file),
    '-rmf', fs.parent(dataframe_lib_file) / fs.stem(dataframe_lib_file) + '.rootmap',
    lz4_preproc_def,
    '-I' + ROOT_incdir,
    '-I' + hipo_incdir,
    ] + dataframe_sources + dataframe_linkdef,
)

# library
dataframe_lib = library(
  'HipoDataFrame',
  dataframe_sources + [ dataframe_dict_tgt[0] ],
  include_directories: [ hipo_inc, ROOT_inc ],
  link_with: [ hipo_lib ],
  link_args: [ '-L' + ROOT_libdir, '-lCore', '-lROOTDataFrame', '-lROOTVecOps'],
  build_rpath: ROOT_libdir,
  cpp_args: [ '-Wno-sign-compare' ], # FIXME: fix these warnings
  install: true
)
install_headers(dataframe_headers, subdir: meson.project_name())
project_libs += dataframe_lib

