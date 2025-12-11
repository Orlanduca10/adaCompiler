.data
_str_newline: .asciiz "\n"
.text
.globl main
main:
  addi $sp, $sp, -20
  # X = 0
  li $t0, 0
  sw $t0, -0($sp)
  # READ X
  li $v0, 5
  syscall
  sw $v0, -0($sp)
  # t0 = X == 10
  lw $t0, -0($sp)
  li $t1, 10
  seq $t2, $t0, $t1
  sw $t2, -8($sp)
  # ifFalse t0 goto L0
  lw $t0, -8($sp)
  beqz $t0, L0
  # t1 = X + 10
  lw $t0, -0($sp)
  li $t1, 10
  add $t2, $t0, $t1
  sw $t2, -12($sp)
  # PRINT t1
  lw $a0, -12($sp)
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _str_newline
  syscall
  # goto L1
  j L1
L0:
  # t2 = X + 1
  lw $t0, -0($sp)
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -16($sp)
  # Y = t2
  lw $t0, -16($sp)
  sw $t0, -4($sp)
  # PRINT Y
  lw $a0, -4($sp)
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _str_newline
  syscall
L1:
  # Restore stack and exit program
  addi $sp, $sp, 20
  li $v0, 10
  syscall
