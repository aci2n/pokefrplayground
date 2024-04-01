#ifndef GUARD_I2N_SWAP
#define GUARD_I2N_SWAP

#include "battle.h"
#include "gba/types.h"

#define B_ACTION_SWAP (B_ACTION_NOTHING_FAINTED + 1)

void i2n_swap_HandleAction();
bool8 i2n_swap_HandleInputChooseAction();
void i2n_swap_WaitActionChosen();
void i2n_swap_WaitActionCaseChosen();
bool8 i2n_swap_HandleInputChooseMove(void (*PlayerBufferExecCompleted)(),
																		 void (*HandleInputChooseTarget)());

#endif
