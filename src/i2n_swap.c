#include "i2n_swap.h"
#include "battle_controllers.h"

void i2n_swap_HandleAction(void)
{
		DebugPrintf("i2n_swap_HandleAction");
		gCurrentActionFuncId = B_ACTION_FINISHED;
}

bool8 i2n_swap_HandleInputChooseAction(void)
{
		if (!JOY_NEW(SELECT_BUTTON))
		{
				return FALSE;
		}

		DebugPrintf("InputChooseAction: swap");
		// the value set here goes to gBattleBufferB,
		// which is read in the case for STATE_WAIT_ACTION_CHOSEN
		BtlController_EmitTwoReturnValues(1, B_ACTION_SWAP, 0);
		// this sets the bitmask on gBattleControllerExecFlags,
		// which is needed to advance states
		OpponentBufferExecCompleted();

		return TRUE;
}

void i2n_swap_WaitActionChosen(void)
{
		DebugPrintf("STATE_WAIT_ACTION_CHOSEN: B_ACTION_SWAP");
		// nothing to do here - gBattleCommunication gets increased after the switch
		// that calls this
}

void i2n_swap_WaitActionCaseChosen(void)
{
		DebugPrintf("STATE_WAIT_ACTION_CASE_CHOSEN: B_ACTION_SWAP");
		// just go to the next state
		gBattleCommunication[gActiveBattler]++;
}
