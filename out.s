.data
_str_newline: .asciiz "\n"
.text
.globl main
main:
  addi $sp, $sp, -16
  # READ X
  li $v0, 5
  syscall
  # ERROR: Attempt to store into undeclared variable/temp: X
  # t0 = X == 10
  li $t1, 10
  seq $t2, $t0, $t1
  sw $t2, -4($sp)
  # ifFalse t0 goto L0
  lw $t0, -4($sp)
  beqz $t0, L0
  # t1 = X + 10
  li $t1, 10
  add $t2, $t0, $t1
  sw $t2, -8($sp)
  # PRINT t1
  lw $a0, -8($sp)
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _str_newline
  syscall
  # goto L1
  j L1
L0:
  # t2 = X + 1
  li $t1, 1
  add $t2, $t0, $t1
  sw $t2, -12($sp)
  # Y = t2
  lw $t0, -12($sp)
  sw $t0, -0($sp)
  # PRINT Y
  lw $a0, -0($sp)
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _str_newline
  syscall
L1:
  # Restore stack and exit program
  addi $sp, $sp, 16
  li $v0, 10
  syscall
