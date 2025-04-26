configure_file(
  input:       'this_hipo.sh.in',
  output:      'this_hipo.sh',
  install:     true,
  install_dir: get_option('libexecdir'),
  configuration: {
    'ld_path': host_machine.system() != 'darwin' ? 'LD_LIBRARY_PATH' : 'DYLD_LIBRARY_PATH',
    'libdir':  get_option('libdir'),
    'root':    ROOT_dep.found() ? 'true' : 'false',
  },
)

foreach shell : [ 'csh', 'tcsh' ]
  configure_file(
    input:         'this_hipo.csh.in',
    output:        f'this_hipo.@shell@',
    install:       true,
    install_dir:   get_option('libexecdir'),
    configuration: {'shell': shell},
  )
endforeach
