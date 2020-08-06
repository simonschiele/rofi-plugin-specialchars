ACLOCAL_AMFLAGS=-I m4
plugindir=${libdir}/rofi/

plugin_LTLIBRARIES = specialchars_plugin.la

specialchars_plugin_la_SOURCES=\
		 src/specialchars_plugin.c

specialchars_plugin_la_CFLAGS= @glib_CFLAGS@ @rofi_CFLAGS@
specialchars_plugin_la_LIBADD= @glib_LIBS@ @rofi_LIBS@
specialchars_plugin_la_LDFLAGS= -module -avoid-version
