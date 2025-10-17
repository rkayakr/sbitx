#include "sdr_ui.h"
#include <stdlib.h>
#include <string.h>
#include <cairo.h>

/* Patch file to provide APF GTK handler referencing existing apf1 instance */

/* extern for the existing APF instance (type 'struct apf') */
extern struct apf apf1;

/* Handler implementation: updates apf1 based on UI fields APF_GAIN and APF_WIDTH */
int do_apf_edit(struct field *f, cairo_t *gfx, int event, int a, int b, int c)
{
    if (!strcmp(field_str("APF"), "ON"))
    {
        struct field *gain_field = get_field("#apf_gain");
        if (gain_field && gain_field->value)
            apf1.gain = atoi(gain_field->value);

        struct field *width_field = get_field("#apf_width");
        if (width_field && width_field->value)
            apf1.width = atoi(width_field->value);

        apf1.ison = 1;
    }
    else
    {
        apf1.ison = 0;
    }

    return 0;
}