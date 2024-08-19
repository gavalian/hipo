fortran_example_exe = executable(
  'freader.exe',
  'freader.F',
  link_with:  project_libs,
  install:    get_option('install_examples'),
  # cpp_args: [ '-Wno-sign-compare' ], # FIXME: fix the warnings; they are suppressed by these args
)

if(have_external_test_file)
  test(
    'freader',
    fortran_example_exe,
    suite: [ 'example' ],
    args:  [ external_test_file ],
  )
endif
