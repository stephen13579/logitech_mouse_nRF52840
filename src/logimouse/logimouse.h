#ifndef LOGIMOUSE_H
#define LOGIMOUSE_H

// 

/*
 * Initialize logimouse.
 * 
 * And need to restore any settings from flash. 
 * 
 */
class Logimouse {
    private:
        // restore mouse settings from flash

    public:
        Logimouse(); // create unifying mouse
        // ~Logimouse(); // reset to default and disable

        // int move_mouse(); // x, y mouse move command

        // initialize esb radio

        // set radio to default settings for mouse
        // this includes RF, address for pipes, etc. 
};

#endif // LOGIMOUSE_H