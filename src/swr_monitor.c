#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fftw3.h>
#include "swr_monitor.h"
#include "sdr_ui.h"
#include "sdr.h"

// VSWR threshold (default 3.0)
float max_vswr = 3.0f;

// VSWR trip status flag
int vswr_tripped = 0;

// Saved drive value (for reference, not restored per requirements)
static int saved_drive_value = 0;

/**
 * check_and_handle_vswr - Monitor VSWR and take protective actions
 * @vswr: VSWR value in project convention (vswr = SWR * 10, e.g., 30 == 3.0)
 *
 * Behavior:
 * - If SWR > max_vswr and not already tripped:
 *   - Set vswr_tripped flag
 *   - Save current DRIVE value
 *   - Read TNPWR from TNPWR field
 *   - Set tx_power to TNPWR
 *   - Update UI: set #vswr_alert, #spectrum_left_msg, #spectrum_left_color
 *   - Write console warning
 *
 * - If SWR <= max_vswr and currently tripped:
 *   - Clear vswr_tripped flag
 *   - Clear UI alerts
 *   - Write console info
 *   - DO NOT restore saved DRIVE value (per requirements)
 */
void check_and_handle_vswr(int vswr)
{
	// Convert from project convention (vswr = SWR * 10) to float
	float swr = vswr / 10.0f;
	
	// Debug logging to help diagnose issues
	char debug_msg[128];
	snprintf(debug_msg, sizeof(debug_msg), 
	         "DEBUG: check_and_handle_vswr called with vswr=%d (SWR=%.1f), max_vswr=%.1f, tripped=%d\n",
	         vswr, swr, max_vswr, vswr_tripped);
	write_console(STYLE_LOG, debug_msg);
	
	// Check if VSWR exceeds threshold and not already tripped
	if (swr > max_vswr && !vswr_tripped) {
		char drive_value[32];
		char tnpwr_value[32];
		char tx_power_cmd[64];
		char console_msg[128];
		
		// Set tripped flag
		vswr_tripped = 1;
		
		// Save current DRIVE value (for reference only)
		if (get_field_value_by_label("DRIVE", drive_value) == 0) {
			saved_drive_value = atoi(drive_value);
		}
		
		// Read TNPWR from TNPWR field
		if (get_field_value_by_label("TNPWR", tnpwr_value) == 0) {
			int tunepower = atoi(tnpwr_value);
			
			// Set tx_power to TNPWR via sdr_request
			char response[64];
			snprintf(tx_power_cmd, sizeof(tx_power_cmd), "tx_power=%d", tunepower);
			sdr_request(tx_power_cmd, response);
			
			// Update DRIVE field in GUI to reflect reduced power
			char drive_buff[32];
			snprintf(drive_buff, sizeof(drive_buff), "%d", tunepower);
			field_set("DRIVE", drive_buff);
			
			// Update UI fields
			set_field("#vswr_alert", "1");
			set_field("#spectrum_left_msg", "HIGH SWR");
			set_field("#spectrum_left_color", "red");
			
			// Write console warning
			snprintf(console_msg, sizeof(console_msg), 
			         "WARNING: HIGH SWR detected (%.1f). TX power reduced to TNPWR (%d)\n", 
			         swr, tunepower);
			write_console(STYLE_LOG, console_msg);
		}
	}
	// Check if VSWR has returned to acceptable level
	else if (swr <= max_vswr && vswr_tripped) {
		char console_msg[128];
		
		// Clear tripped flag
		vswr_tripped = 0;
		
		// Clear UI fields
		set_field("#vswr_alert", "0");
		set_field("#spectrum_left_msg", "");
		set_field("#spectrum_left_color", "");
		
		// Write console info
		snprintf(console_msg, sizeof(console_msg), 
		         "INFO: SWR returned to acceptable level (%.1f). TX power NOT restored.\n", 
		         swr);
		write_console(STYLE_LOG, console_msg);
		
		// Clear saved drive value to avoid accidental restore
		saved_drive_value = 0;
	}
}

/**
 * init_vswr_monitor - Initialize VSWR monitor at startup
 *
 * Ensures all UI fields are cleared and monitor is in a known state.
 * Should be called during application initialization.
 */
void init_vswr_monitor(void)
{
	// Ensure tripped flag is off
	vswr_tripped = 0;
	
	// Clear saved drive value
	saved_drive_value = 0;
	
	// Clear UI fields to ensure clean startup
	set_field("#vswr_alert", "0");
	set_field("#spectrum_left_msg", "");
	set_field("#spectrum_left_color", "");
}

/**
 * reset_vswr_tripped - Reset VSWR trip status and clear UI
 *
 * Clears the vswr_tripped flag and UI alerts without restoring drive.
 * Can be called manually if needed to clear the alert state.
 */
void reset_vswr_tripped(void)
{
	// Clear tripped flag
	vswr_tripped = 0;
	
	// Clear UI fields
	set_field("#vswr_alert", "0");
	set_field("#spectrum_left_msg", "");
	set_field("#spectrum_left_color", "");
	
	// Clear saved drive value
	saved_drive_value = 0;
}
