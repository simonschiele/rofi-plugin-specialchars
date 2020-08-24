ACLOCAL_AMFLAGS=-I m4
plugindir=${rofi_PLUGIN_INSTALL_DIR}/

plugin_LTLIBRARIES = emojis_plugin.la \
					 kaomojis_plugin.la

dist_pkgdata_DATA = resources/emojis.txt \
					resources/emojis-simple.txt \
					resources/copy_insert.sh \
					resources/kaomojis.txt \
					README.md

emojis_plugin_la_SOURCES = src/emojis_plugin.c
emojis_plugin_la_CFLAGS = @glib_CFLAGS@ @rofi_CFLAGS@
emojis_plugin_la_LIBADD = @glib_LIBS@ @rofi_LIBS@
emojis_plugin_la_LDFLAGS = -module -avoid-version

kaomojis_plugin_la_SOURCES = src/kaomojis_plugin.c
kaomojis_plugin_la_CFLAGS = @glib_CFLAGS@ @rofi_CFLAGS@
kaomojis_plugin_la_LIBADD = @glib_LIBS@ @rofi_LIBS@
kaomojis_plugin_la_LDFLAGS = -module -avoid-version
