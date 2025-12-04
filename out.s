.data
_str_newline: .asciiz "\n"
.text
.globl main
main:
  addi $sp, $sp, -12
  # X = 0
  li $t0, 0
  sw $t0, -0($sp)
L0:
  # t0 = X + 2
  lw $t0, -0($sp)
  li $t1, 2
  add $t2, $t0, $t1
  sw $t2, -8($sp)
  # ifFalse t0 goto L1
  lw $t0, -8($sp)
  beqz $t0, L1
  # X = 2
  li $t0, 2
  sw $t0, -0($sp)
  # Y = 3
  li $t0, 3
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
  addi $sp, $sp, 12
  li $v0, 10
  syscall
