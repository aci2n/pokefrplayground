set print pretty

file pokefirered_modern.elf

# break RunTurnActionsFunctions
# break i2n_swap_HandleAction
# break HandleChooseMoveAfterDma3
break i2n_swap_HandleInputChooseMove if gMain.newKeys & 0x8
# break src/i2n_swap.c:84

# watch gCurrentMove
# watch gCurrentTurnActionNumber
# watch gBattlerAttacker

target remote :2345
