project('montecarlo','cpp')
inc = include_directories('include')
src = ['main.cpp','montecarlo.cpp','loadExpr.cpp','util.cpp']

muparserx = subproject('muparserx')
muparserx_dep = muparserx.get_variable('muparserx_dep')
dep = [muparserx_dep]

executable('mocha',include_directories : inc, sources : src, dependencies : dep)
