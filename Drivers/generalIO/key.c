/*
 * =====================================================================================
 *
 *       Filename:  key.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03.04.24 20:56:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "key.h"
#include "cmsis_gcc.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>


/* Return with status macro */
#define RETURN_WITH_STATUS(p, s) (p)->presed = s; return s
 
void BUT_Init(BUT_Button_t* but, GPIO_TypeDef* GPIO_B_Port,   uint16_t GPIO_B_Pin, uint8_t ButtonState, void (*BUT_Handler)(BUT_Button_t*, BUT_Presed_type_t)) {
    but->GPIO_BUTTON = GPIO_B_Port;
    but->GPIO_PIN_BUTTON = GPIO_B_Pin;
    but->GPIO_state = ButtonState;
    but->state = BT_HIDLE;
    but->presed = CLICK_NOTHING;
    but->pressed_time = 0;
    but->BUT_Handler = BUT_Handler;
}

void BUT_Check(BUT_Button_t* but) {
    uint8_t state;
    uint32_t current_time = HAL_GetTick();
    state = HAL_GPIO_ReadPin(but->GPIO_BUTTON, but->GPIO_PIN_BUTTON);
    if (but->state == BT_HIDLE) {
        if (state == but->GPIO_state) {
            but->state = BT_DEBOUNCE;
            but->pressed_time = current_time;
        }
    }
    if (but->state == BT_DEBOUNCE) {
        if (state == but->GPIO_state) {
            if ((current_time - but->pressed_time) > BUT_DEBOUNCE_TIME) {
                but->state = BT_PRESSED;
            }
        } else if (state != but->GPIO_state) {
            but->state = BT_HIDLE;
        }
    }
    if (but->state == BT_PRESSED) {
        if (state == but->GPIO_state) {
            if ((current_time - but->pressed_time) > BUT_LONG_PRESS_TIME) {
                if (but->BUT_Handler) {
                    but->BUT_Handler(but, PRESS_LONG_CLICK);
                }
                but->state = BT_UNRELEASED;
            } 
        }else if (state != but->GPIO_state) {
            if ((current_time - but->pressed_time) > BUT_NORMAL_PRESS_TIME) {
                if (but->BUT_Handler) {
                    but->BUT_Handler(but, PRESS_CLICK);
                }
                but->state = BT_UNRELEASED;
            } 
            else {
                but->state = BT_HIDLE;
            }

        } else {
            but->state = BT_HIDLE;
        }
    }
    if ((but->state == BT_UNRELEASED) && (state != but->GPIO_state)) {
        but->state = BT_HIDLE;
    }
}

BUT_Presed_type_t BUT_Get(BUT_Button_t* but) {
    if(but->state == BT_CLICKED) {
        but->state = BT_UNRELEASED;
        RETURN_WITH_STATUS(but, PRESS_CLICK);
    }
    else if(but->state == BT_LONG_CLICK) {
        but->state = BT_UNRELEASED;
        RETURN_WITH_STATUS(but, PRESS_LONG_CLICK);
    }  
    RETURN_WITH_STATUS(but, CLICK_NOTHING);
}
    
