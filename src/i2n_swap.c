#include "i2n_swap.h"
#include "battle_controllers.h"
#include "battle_scripts.h"
#include "constants/songs.h"
#include "palette.h"
#include "sound.h"
#include "constants/moves.h"
#include "battle_anim.h"
#include "util.h"

void i2n_swap_HandleAction()
{
		DebugPrintf("i2n_swap_HandleAction");

		u16 const move = MOVE_I2N_SWAP;

		// copy init from HandleAction_UseMove
		gCritMultiplier = 1;
		gBattleScripting.dmgMultiplier = 1;
		gBattleStruct->atkCancellerTracker = 0;
		gMoveResultFlags = 0;
		gMultiHitCounter = 0;
		gBattleCommunication[MISS_TYPE] = 0;
		gCurrentMove = move;
		gChosenMove = move;
		gBattleResults.lastUsedMovePlayer = move;
		gHitMarker |= HITMARKER_NO_PPDEDUCT;
		gBattleStruct->moveTarget[gBattlerAttacker] = GetMoveTarget(move, NO_TARGET_OVERRIDE);
		gBattlerTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[move].effect];
		gCurrentActionFuncId = B_ACTION_EXEC_SCRIPT;
}

bool8 i2n_swap_HandleInputChooseAction()
{
		// DISABLED: changing approach
		return FALSE;

		if (!JOY_NEW(SELECT_BUTTON))
		{
				return FALSE;
		}

		DebugPrintf("InputChooseAction: swap");

		PlaySE(SE_SELECT);

		// the value set here goes to gBattleBufferB,
		// which is read in the case for STATE_WAIT_ACTION_CHOSEN
		// to set gCurrentActionFuncId
		BtlController_EmitTwoReturnValues(1, B_ACTION_SWAP, 0);
		// this sets the bitmask on gBattleControllerExecFlags,
		// which is needed to advance states
		OpponentBufferExecCompleted();

		return TRUE;
}

void i2n_swap_WaitActionChosen()
{
		DebugPrintf("STATE_WAIT_ACTION_CHOSEN: B_ACTION_SWAP");
		// nothing to do here - gBattleCommunication gets increased after the switch
		// that calls this
}

void i2n_swap_WaitActionCaseChosen()
{
		DebugPrintf("STATE_WAIT_ACTION_CASE_CHOSEN: B_ACTION_SWAP");
		// just go to the next state
		gBattleCommunication[gActiveBattler]++;
}

bool8 i2n_swap_HandleInputChooseMove(void (*PlayerBufferExecCompleted)(),
																		 void (*HandleInputChooseTarget)())
{
		if (!JOY_NEW(START_BUTTON)) {
				return FALSE;
		}

		bool32 canSelectTarget = FALSE;
		u8 moveTarget;
		struct ChooseMoveStruct* const moveInfo = (struct ChooseMoveStruct *)(&gBattleBufferA[gActiveBattler][4]);
		u8 const moveIndex = 0;
		struct BattlePokemon* const battlePokemon = &gBattleMons[gActiveBattler];
		u16 const moveId = MOVE_I2N_SWAP;

		gMoveSelectionCursor[gActiveBattler] = moveIndex;
		battlePokemon->moves[moveIndex] = moveId;
		moveInfo->moves[moveIndex] = moveId;
		moveInfo->currentPp[moveIndex] = 1;
		moveInfo->maxPp[moveIndex] = 1;

		PlaySE(SE_SELECT);
		if (moveInfo->moves[gMoveSelectionCursor[gActiveBattler]] == MOVE_CURSE)
		{
				if (moveInfo->monType1 != TYPE_GHOST && moveInfo->monType2 != TYPE_GHOST)
						moveTarget = MOVE_TARGET_USER;
				else
						moveTarget = MOVE_TARGET_SELECTED;
		}
		else
		{
				moveTarget = gBattleMoves[moveInfo->moves[gMoveSelectionCursor[gActiveBattler]]].target;
		}

		if (moveTarget & MOVE_TARGET_USER)
				gMultiUsePlayerCursor = gActiveBattler;
		else
				gMultiUsePlayerCursor = GetBattlerAtPosition((GetBattlerPosition(gActiveBattler) & BIT_SIDE) ^ BIT_SIDE);

		if (!gBattleBufferA[gActiveBattler][1]) // not a double battle
		{
				if (moveTarget & MOVE_TARGET_USER_OR_SELECTED && !gBattleBufferA[gActiveBattler][2])
						++canSelectTarget;
		}
		else // double battle
		{
				if (!(moveTarget & (MOVE_TARGET_RANDOM | MOVE_TARGET_BOTH | MOVE_TARGET_DEPENDS | MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_OPPONENTS_FIELD | MOVE_TARGET_USER)))
						++canSelectTarget; // either selected or user
				if (moveInfo->currentPp[gMoveSelectionCursor[gActiveBattler]] == 0)
				{
						canSelectTarget = FALSE;
				}
				else if (!(moveTarget & (MOVE_TARGET_USER | MOVE_TARGET_USER_OR_SELECTED)) && CountAliveMonsInBattle(BATTLE_ALIVE_EXCEPT_ACTIVE) <= 1)
				{
						gMultiUsePlayerCursor = GetDefaultMoveTarget(gActiveBattler);
						canSelectTarget = FALSE;
				}
		}
		ResetPaletteFadeControl();
		BeginNormalPaletteFade(0xF0000, 0, 0, 0, RGB_WHITE);
		if (!canSelectTarget)
		{
				BtlController_EmitTwoReturnValues(1, 10, gMoveSelectionCursor[gActiveBattler] | (gMultiUsePlayerCursor << 8));
				PlayerBufferExecCompleted();
		}
		else
		{
				gBattlerControllerFuncs[gActiveBattler] = HandleInputChooseTarget;
				if (moveTarget & (MOVE_TARGET_USER | MOVE_TARGET_USER_OR_SELECTED))
						gMultiUsePlayerCursor = gActiveBattler;
				else if (gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)])
						gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
				else
						gMultiUsePlayerCursor = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
				gSprites[gBattlerSpriteIds[gMultiUsePlayerCursor]].callback = SpriteCB_ShowAsMoveTarget;
		}

		return TRUE;
}
