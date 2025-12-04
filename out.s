.data
_str_newline: .asciiz "\n"
.text
.globl main
main:
  addi $sp, $sp, -4
  # X = 0
  li $t0, 0
  sw $t0, -0($sp)
  # Restore stack and exit program
  addi $sp, $sp, 4
  li $v0, 10
  syscall
