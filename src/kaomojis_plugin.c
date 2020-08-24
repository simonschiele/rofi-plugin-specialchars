/**
 * rofi-plugin-kaomojis
 *
 * MIT/X11 License
 * Copyright (c) 2020 Simon Schiele <simon.codingmonkey@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <gmodule.h>

#include <rofi/mode.h>
#include <rofi/helper.h>
#include <rofi/mode-private.h>

#include <stdint.h>

G_MODULE_EXPORT Mode mode;

typedef struct
{
	GPtrArray *array;
	unsigned int array_length;
} KAOMOJISModePrivateData;


static void get_kaomojis(Mode *sw)
{
	KAOMOJISModePrivateData *pd = (KAOMOJISModePrivateData*) mode_get_private_data(sw);
	char *dpath = "~/.config/i3/resources/kaomojis.txt";
	char *path = rofi_expand_path(dpath);
	char *content = NULL;
	char *next = NULL;
	char *next_tmp = NULL;

	if (!g_file_get_contents(path, &content, NULL, NULL)) {
		fprintf(stderr, "Could not read entries file: %s\n", path);
		return;
	}

	next = strtok_r(content, "\n", &next_tmp);
	do {
		pd->array_length++;
		g_ptr_array_add(pd->array, (gpointer) next);
	} while ((next = strtok_r(NULL, "\n", &next_tmp)) != NULL);

	g_free(path);
	g_free(next);
}


static int kaomojis_mode_init ( Mode *sw )
{
	/**
	 * Called on startup when enabled (in modi list)
	 */
	if (mode_get_private_data(sw) == NULL) {
		KAOMOJISModePrivateData *pd = g_malloc0(sizeof(*pd));
		mode_set_private_data(sw, (void *) pd);

		// Load content.
		pd->array = g_ptr_array_new();
		get_kaomojis(sw);
	}
	return TRUE;
}
static unsigned int kaomojis_mode_get_num_entries(const Mode *sw)
{
	const KAOMOJISModePrivateData *pd = (const KAOMOJISModePrivateData *) mode_get_private_data(sw);
	return pd->array_length;
}

static void action(char *line) {
	char *kaomoji;
	char *desc;
	char *tmp;

	kaomoji = strtok_r(line, "\t\t", &tmp);
	desc = strtok_r(NULL, "\t\t", &tmp);

	fprintf(stderr, "kaomoji: %s\n", kaomoji);
	fprintf(stderr, "desc: %s\n", desc);
}

static ModeMode kaomojis_mode_result(Mode *sw, int mretv, char **input, unsigned int selected_line)
{
	ModeMode retv = MODE_EXIT;
	KAOMOJISModePrivateData *pd = (KAOMOJISModePrivateData *) mode_get_private_data(sw);

	if ( mretv & MENU_NEXT ) {
		retv = NEXT_DIALOG;
	} else if ( mretv & MENU_PREVIOUS ) {
		retv = PREVIOUS_DIALOG;
	} else if ( mretv & MENU_QUICK_SWITCH ) {
		retv = ( mretv & MENU_LOWER_MASK );
	} else if ( ( mretv & MENU_OK ) ) {
		action(g_ptr_array_index(pd->array, selected_line));
		retv = MODE_EXIT;
	} else if ( ( mretv & MENU_ENTRY_DELETE ) == MENU_ENTRY_DELETE ) {
		retv = RELOAD_DIALOG;
	}
	return retv;
}

static void kaomojis_mode_destroy(Mode *sw)
{
	KAOMOJISModePrivateData *pd = (KAOMOJISModePrivateData *) mode_get_private_data(sw);
	if (pd != NULL) {
		if (pd->array != NULL) {
			g_ptr_array_free(pd->array, TRUE);
		}
		g_free(pd);
		mode_set_private_data(sw, NULL);
	}
}

static char *get_display_string(const KAOMOJISModePrivateData *pd, char *data) {
	return data;
}

static char *_get_display_value (const Mode *sw, unsigned int selected_line, G_GNUC_UNUSED int *state, G_GNUC_UNUSED GList **attr_list, int get_entry)
{
	KAOMOJISModePrivateData *pd = (KAOMOJISModePrivateData *) mode_get_private_data(sw);
	char *value = NULL;

	value = g_strdup(g_ptr_array_index(pd->array, selected_line));
	return get_entry ? get_display_string(pd, value): NULL;
}

/**
 * @param sw The mode object.
 * @param tokens The tokens to match against.
 * @param index  The index in this plugin to match against.
 *
 * Match the entry.
 *
 * @param returns try when a match.
 */
static int kaomojis_token_match ( const Mode *sw, rofi_int_matcher **tokens, unsigned int index )
{
	KAOMOJISModePrivateData *pd = (KAOMOJISModePrivateData *) mode_get_private_data ( sw );
	return helper_token_match(tokens, g_strdup(g_ptr_array_index(pd->array, index)));
}


Mode mode =
{
	.abi_version        = ABI_VERSION,
	.name               = "kaomojis",
	.cfg_name_key       = "display-kaomojis",
	._init              = kaomojis_mode_init,
	._get_num_entries   = kaomojis_mode_get_num_entries,
	._result            = kaomojis_mode_result,
	._destroy           = kaomojis_mode_destroy,
	._token_match       = kaomojis_token_match,
	._get_display_value = _get_display_value,
	._get_message       = NULL,
	._get_completion    = NULL,
	._preprocess_input  = NULL,
	.private_data       = NULL,
	.free               = NULL,
};
