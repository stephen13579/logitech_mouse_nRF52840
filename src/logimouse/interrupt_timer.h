#ifndef INTERRUPT_TIMER_H
#define INTERRUPT_TIMER_H

/*
 * This file contains the interrupt timer implementation for the logitech mouse.
 * The interrupt timer is used to periodically execute a function.
 * For a logitech mouse this could be reporting the mouse position at steady
 * intervals, ex. 500 Hz or 1000 Hz polling rate.
 */

class Interrupt
{
public:
    /*
     * Initialize hardware timer interrupt.
     * This development environment uses a RTOS and uses the correct
     * implementation to allow the kernel to service the interrupt as quickly as
     * possible.
     */
    void timer_init(void);

    Interrupt();

private:
    // store the current timer counter value
    

    // reset timer
    static void reset_timestamp_timer(void);
};

// handler for interrupt callback to be executed
static int m_timer_counter;
static void interrupt_handler();

#endif // INTERRUPT_TIMER_H