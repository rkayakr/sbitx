#include "swr_monitor.h"
#include <stdlib.h>
#include <string.h>
#include "sdr_ui.h"

float max_vswr = 3.0f;
int vswr_tripped = 0;

/* prototypes for external helpers used from the codebase */
struct field;
extern struct field *get_field(const char *id);
extern int set_field(const char *id, const char *value);
extern int field_set(const char *label, const char *value);
extern int get_field_value_by_label(char *label, char *value);
extern void sdr_request(char *cmd, char *response);
extern void write_console(sbitx_style style, const char *msg);

static void set_tx_power_and_ui(int power)
{
    char cmd[64];
    char resp[128];
    snprintf(cmd, sizeof(cmd), "tx_power=%d", power);
    sdr_request(cmd, resp);

    char pstr[16];
    snprintf(pstr, sizeof(pstr), "%d", power);
    field_set("DRIVE", pstr);
}

void check_and_handle_vswr(int vswr)
{
    float swr = vswr / 10.0f;

    if (swr > max_vswr) {
        if (!vswr_tripped) {
            vswr_tripped = 1;

            int tnpwr = 10;
            struct field *f_tnp = get_field("#tune_power");
            if (f_tnp && f_tnp->value && strlen(f_tnp->value)) {
                tnpwr = atoi(f_tnp->value);
                if (tnpwr < 1) tnpwr = 1;  // Minimum 1 watt to avoid zero power
            }

            set_tx_power_and_ui(tnpwr);

            set_field("#vswr_alert", "1");
            set_field("#spectrum_left_msg", "HIGH SWR");
            set_field("#spectrum_left_color", "red");

            write_console(STYLE_LOG, "WARNING: HIGH SWR - drive reduced to TNPWR\n");
        }
    } else {
        if (vswr_tripped) {
            vswr_tripped = 0;

            set_field("#vswr_alert", "0");
            set_field("#spectrum_left_msg", "");
            set_field("#spectrum_left_color", "");

            write_console(STYLE_LOG, "INFO: VSWR normalized - HIGH SWR message cleared (drive NOT restored)\n");
        }
    }
}

void reset_vswr_tripped(void)
{
    vswr_tripped = 0;
    set_field("#vswr_alert", "0");
    set_field("#spectrum_left_msg", "");
    set_field("#spectrum_left_color", "");
}
