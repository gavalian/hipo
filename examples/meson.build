external_test_file = get_option('test_file')
have_external_test_file = external_test_file != '' # true if a user specified a test file

example_sources = [
  # {
  #   'sources':    list of source code files; the example name will be based off the first one
  #   'test_this':  boolean, whether or not to run this example as a test
  #   'test_args':  list of arguments for this example, used for testing
  #   'first':      boolean, whether this test should be run before any others (default = false)
  # },
  { 'sources': [ 'bankRowList.cc' ],        'test_this': have_external_test_file, 'test_args': [ external_test_file ] },
  { 'sources': [ 'builder.cc' ],            'test_this': false,                   'test_args': [] },
  { 'sources': [ 'dataframe.cc' ],          'test_this': false,                   'test_args': [] },
  { 'sources': [ 'histograms.cc' ],         'test_this': have_external_test_file, 'test_args': [ external_test_file ] },
  { 'sources': [ 'multithread.cc' ],        'test_this': false,                   'test_args': [] },
  { 'sources': [ 'readEvents.cc' ],         'test_this': false,                   'test_args': [] },
  { 'sources': [ 'readFile.cc' ],           'test_this': true,                    'test_args': [ 'example_output.hipo' ] },
  { 'sources': [ 'readFileDebug.cc' ],      'test_this': false,                   'test_args': [] },
  { 'sources': [ 'readFileTags.cc' ],       'test_this': true,                    'test_args': [] },
  { 'sources': [ 'readFusion.cc' ],         'test_this': false,                   'test_args': [] },
  { 'sources': [ 'readHist.cc' ],           'test_this': false,                   'test_args': [] },
  { 'sources': [ 'readJson.cc' ],           'test_this': have_external_test_file, 'test_args': [ external_test_file ] },
  { 'sources': [ 'schema.cc' ],             'test_this': false,                   'test_args': [] },
  { 'sources': [ 'showFile.cc' ],           'test_this': true,                    'test_args': [ 'example_output.hipo', 'event::particle' ] },
  { 'sources': [ 'tupleFile.cc' ],          'test_this': true,                    'test_args': [] },
  { 'sources': [ 'writeEvents.cc' ],        'test_this': false,                   'test_args': [] },
  { 'sources': ['writeFile.cc' ],           'test_this': true,                    'test_args': [], 'first': true },
  # { 'sources': [ 'writeFileComposite.cc' ], 'test_this': false,                   'test_args': [] }, # FIXME: does not compile
  { 'sources': [ 'writeUserHeader.cc' ],    'test_this': true,                    'test_args': [] },
]

foreach ex : example_sources
  example_name = fs.stem(ex['sources'][0])
  example_exe = executable(
    f'@example_name@.exe',
    sources:             ex['sources'],
    include_directories: [ hipo_inc ],
    link_with:           project_libs,
    install:             get_option('install_examples'),
    cpp_args:            [ '-Wno-sign-compare' ], # FIXME: fix the warnings; they are suppressed by these args
  )
  if(ex.get('test_this', true))
    test(
      example_name,
      example_exe,
      suite:       [ 'example' ],
      args:        ex.get('test_args', []),
      priority:    ex.get('first', false) ? -1 : 0,       # make sure 'first' tests start before any other
      is_parallel: ex.get('first', false) ? false : true, # make sure 'first' tests finish before any other
    )
  endif
endforeach
