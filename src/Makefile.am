bin_PROGRAMS = manic
manic_SOURCES = manic.cpp

# set the include path found by configure
INCLUDES= $(all_includes) @ECORE_CFLAGS@ @EDJE_CFLAGS@ @ELEMENTARY_CFLAGS@ @EVAS_CFLAGS@ @EWEBKIT_CFLAGS@

# the library search path.
manic_LDFLAGS = $(all_libraries) @ECORE_LIBS@ @EVAS_LIBS@ @EDJE_LIBS@ @ELEMENTARY_LIBS@ @EWEBKIT_LIBS@
