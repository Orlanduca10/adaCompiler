.data
_str_newline: .asciiz "\n"
.text
.globl main
main:
  addi $sp, $sp, -20
  # X = 0
  li $t0, 0
  sw $t0, -0($sp)
L0:
  # t0 = X != 2
  lw $t0, -0($sp)
  li $t1, 2
  sne $t2, $t0, $t1
  sw $t2, -8($sp)
  # ifFalse t0 goto L1
  lw $t0, -8($sp)
  beqz $t0, L1
  # X = 2
  li $t0, 2
  sw $t0, -0($sp)
  # t1 = 3 + 8
  li $t0, 3
  li $t1, 8
  add $t2, $t0, $t1
  sw $t2, -12($sp)
  # t2 = t1 + 16
  lw $t0, -12($sp)
  li $t1, 16
  add $t2, $t0, $t1
  sw $t2, -16($sp)
  # Y = t2
  lw $t0, -16($sp)
  sw $t0, -4($sp)
  # goto L0
  j L0
L1:
  # PRINT Y
  lw $a0, -4($sp)
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _str_newline
  syscall
  # Restore stack and exit program
  addi $sp, $sp, 20
  li $v0, 10
  syscall
