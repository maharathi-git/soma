project('soma', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

gtk_dep = dependency('gtk+-3.0')

executable('soma',
  'src/soma.c', 'src/dhruva.c', 'src/calendar_list.c',
  dependencies : gtk_dep,
  link_args : ['-lm'],  # Link with the math library
  install : true)
