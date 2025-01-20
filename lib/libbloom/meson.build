
libbloom_sources = files(
    'bloom.h',
    'murmurhash2.h',
    'bloom.c',
    'MurmurHash2.c'
)

libbloom = static_library(
    'bloom',
    libbloom_sources,
    include_directories : include_directories('.'),
    c_args : ['-Wall', '-fPIC', '-DBLOOM_VERSION=2.0', '-DBLOOM_VERSION_MAJOR=2', '-DBLOOM_VERSION_MINOR=0'],
    install : false
)

libbloom_dep = declare_dependency(
    link_with : libbloom,
    include_directories : include_directories('.')
)