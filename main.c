#include <yaul.h>

// Void function pointers for callbacks/interrupts
static void _vblank_out_handler(void *);

static smpc_peripheral_digital_t g_k_digitalPad;


void user_init(void)
{
        vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE,
                VDP2_TVMD_HORZ_NORMAL_A,
                VDP2_TVMD_VERT_224); // Set a resolution for the VDP2

        vdp2_scrn_back_screen_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE),
                COLOR_RGB1555(1, 2, 2, 5)); // Set the background colour

        dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC); // Init debug output using the VDP2 driver
        dbgio_dev_font_load(); // Load the font for dbgio

        vdp_sync_vblank_out_set(_vblank_out_handler, NULL); // Assign a callback function for vblank, mostly for collecting peripheral information

        vdp2_tvmd_display_set(); // Enable display output from VDP2

        vdp2_sync(); // Synchronises the buffered commands waiting for VDP2
        vdp2_sync_wait(); // Wait for all synchronise commands to finish as they're non-blocking
        // returns back to start of VBLANK as well

        return;
}

int main(void)
{
        int iDx = 0;
        while (true)
        {                       // V V Reset cursor to position 1x1, 1 indexed coords
                dbgio_printf("\x1b[1;1H\x1b[2JHello, World!\nCounter: %i\n", iDx); // Write text to debug text buffer
                dbgio_flush(); // Flushes the dbgio buffer to draw to the selected driver 

                // Test to see if controller buttons have been pushed
                smpc_peripheral_process(); // Process all inputs
                smpc_peripheral_digital_port(1, &g_k_digitalPad); // Assign processed inputs to peripheral device? No documentation to confirm

                if (g_k_digitalPad.held.button.start != 0) // If start button pressed
                {
                        smpc_smc_sysres_call(); // Reset the whole console

                        return 0;
                }

                if ((g_k_digitalPad.pressed.raw & PERIPHERAL_DIGITAL_UP) != 0)    // If D-Pad up has been pressed
                {
                        iDx++;                                                  // Increent the counter
                } 
                else if ((g_k_digitalPad.pressed.raw & PERIPHERAL_DIGITAL_DOWN) != 0) // Otherwise if D-Pad down has been pressed
                {
                        iDx--;                                                  // Decrement the counter
                }

                vdp2_sync(); // Synchronises the buffered commands waiting for VDP2
                vdp2_sync_wait(); // Wait for all synchronise commands to finish as they're non-blocking
                // returns back to start of VBLANK as well
        }
        return 0;
}

static void _vblank_out_handler(void *work __unused)
{
        smpc_peripheral_intback_issue();
}