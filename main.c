#include <yaul.h>

#include "drawing.h"

static smpc_peripheral_digital_t g_k_digitalPad;


void user_init(void)
{
        initDrawing();

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

                updateDrawing();
        }
        return 0;
}