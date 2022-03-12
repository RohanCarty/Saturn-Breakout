#include "drawing.h"

// Void function pointers for callbacks/interrupts
static void _vblank_out_handler(void *);

int initDrawing()
{
    vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE,
        VDP2_TVMD_HORZ_NORMAL_A,
        VDP2_TVMD_VERT_224); // Set a resolution for the VDP2

    vdp2_scrn_back_screen_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE),
        COLOR_RGB1555(1, 1, 1, 1)); // Set the background colour

    dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC); // Init debug output using the VDP2 driver
    dbgio_dev_font_load(); // Load the font for dbgio

    vdp_sync_vblank_out_set(_vblank_out_handler, NULL); // Assign a callback function for vblank, mostly for collecting peripheral information

    vdp2_tvmd_display_set(); // Enable display output from VDP2

    vdp2_sync(); // Synchronises the buffered commands waiting for VDP2
    vdp2_sync_wait(); // Wait for all synchronise commands to finish as they're non-blocking
    // returns back to start of VBLANK as well
    
    return 0;
}

int updateDrawing()
{
    vdp2_sync(); // Synchronises the buffered commands waiting for VDP2
    vdp2_sync_wait(); // Wait for all synchronise commands to finish as they're non-blocking
    // returns back to start of VBLANK as well

    return 0;
}

static void _vblank_out_handler(void *work __unused)
{
        smpc_peripheral_intback_issue();
}