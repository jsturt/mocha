project('montecarlo','cpp')
inc = include_directories('include')
src = ['main.cpp','montecarlo.cpp','loadExpr.cpp','util.cpp']

muparser = subproject('muparser')
muparser_dep = muparser.get_variable('muparser_dep')
dep = [muparser_dep]

executable('mocha',include_directories : inc, sources : src, dependencies : dep)
