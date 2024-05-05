#ifndef  key_INC
#define  key_INC

#include "main.h"
#include "stm32f103xb.h"
#include <stdint.h>
#include <stdbool.h>

#define BUT_MAX_BUTTONS         10
#define BUT_DEBOUNCE_TIME       5
#define BUT_NORMAL_PRESS_TIME   50
#define BUT_LONG_PRESS_TIME     500

#define REPEAT

typedef enum {
    CLICK_NOTHING,
    PRESS_CLICK,
    PRESS_LONG_CLICK
} BUT_Presed_type_t;

typedef enum {
  BT_PRESSED,
  BT_CLICKED,
  BT_LONG_CLICK,
  BT_UNRELEASED,
  BT_HIDLE,
  BT_DEBOUNCE
} BUT_State_t;

typedef struct BUT_Button_t BUT_Button_t;

struct BUT_Button_t{
    GPIO_TypeDef* GPIO_BUTTON; /* GPIOx port for button */
    uint16_t GPIO_PIN_BUTTON;  /* GPIO pin for button */
    uint8_t GPIO_state;        /* GPIO state for pin when pressed */
    BUT_State_t state;         /* Current button state */
    BUT_Presed_type_t presed;       /* Type of pressed */
    uint32_t pressed_time;     /* Pressed time */
    void (*BUT_Handler)(struct BUT_Button_t*, BUT_Presed_type_t); /* Button handler */
};


void BUT_Init(BUT_Button_t* but, GPIO_TypeDef* GPIO_B_Port,   uint16_t GPIO_B_Pin, uint8_t ButtonState, void (*BUT_Handler)(BUT_Button_t*, BUT_Presed_type_t));
void BUT_Process(BUT_Button_t* but); 
void BUT_Check(BUT_Button_t* but);
BUT_Presed_type_t BUT_Get(BUT_Button_t* but);

#endif   /* ----- #ifndef key_INC  ----- */
