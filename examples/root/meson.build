example_root_sources = [
  { 'sources': [ 'benchmark.cc' ],       'install_this': false },
  { 'sources': [ 'converter.cc' ],       'install_this': get_option('install_root_tools') },
  { 'sources': [ 'hist2root.cc' ],       'install_this': get_option('install_root_tools') },
  { 'sources': [ 'simulation2hipo.cc' ], 'install_this': get_option('install_root_tools') },
]

foreach ex : example_root_sources
  example_name = 'hipo_' + fs.stem(ex['sources'][0])
  example_exe = executable(
    example_name,
    sources:             ex['sources'],
    include_directories: [ hipo_inc, include_directories(ROOT_incdir) ],
    link_with:           [ hipo_lib ],
    link_args:           [ '-L' + ROOT_libdir, '-lCore', '-lRIO', '-lHist', '-lTree', '-lPhysics' ],
    install:             ex['install_this'],
    cpp_args:            get_option('suppress_warnings') ? [ '-Wno-unused-variable', '-Wno-unused-but-set-variable', '-Wno-sign-compare' ] : [], # FIXME: fix the warnings
  )
endforeach
