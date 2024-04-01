#ifndef GUARD_I2N_SWAP
#define GUARD_I2N_SWAP

#include "battle.h"
#include "gba/types.h"

#define B_ACTION_SWAP (B_ACTION_NOTHING_FAINTED + 1)

void i2n_swap_HandleAction(void);
bool8 i2n_swap_HandleInputChooseAction(void);
void i2n_swap_WaitActionChosen(void);
void i2n_swap_WaitActionCaseChosen(void);

#endif
