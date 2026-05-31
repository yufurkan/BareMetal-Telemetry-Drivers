/*
 * delay.h
 *
 *  Created on: May 31, 2026
 *      Author: yufur
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_


class Delay {
public:

    static void init(void);
    static void ms(uint32_t delay_ms);

private:
    // C++11 block create object
    Delay() = delete;
};


#endif /* INC_DELAY_H_ */
