.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
li $v0, 4
la $a0, _prompt
syscall
li $v0, 5
syscall
jr $ra

write:
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
jr $ra


fact_:
addi $sp, $sp, -4
sw $fp, 0($sp)
move $fp, $sp
addi $sp, $fp, -36
lw $t0, 8($fp)
sw $t0, -4($fp)
li $t0, 1
sw $t0, -8($fp)
lw $t0, -4($fp)
lw $t1, -8($fp)
beq $t0, $t1, label1
j label2
label1:
lw $t0, 8($fp)
sw $t0, -12($fp)
lw $t0, -12($fp)
move $v0, $t0
move $sp, $fp
lw $fp, 0($sp)
addi $sp, $sp, 4
jr $ra
j label3
label2:
lw $t0, 8($fp)
sw $t0, -16($fp)
lw $t0, 8($fp)
sw $t0, -20($fp)
li $t0, 1
sw $t0, -24($fp)
lw $t0, -20($fp)
lw $t1, -24($fp)
sub $t0, $t0, $t1
sw $t0, -28($fp)
lw $t0, -28($fp)
addi $sp, $sp, -4
sw $t0, 0($sp)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal fact_
lw $ra, 0($sp)
addi $sp, $sp, 4
addi $sp, $sp, 4
move  $t0, $v0
sw $t0, -32($fp)
lw $t0, -16($fp)
lw $t1, -32($fp)
mul $t0, $t0, $t1
sw $t0, -36($fp)
lw $t0, -36($fp)
move $v0, $t0
move $sp, $fp
lw $fp, 0($sp)
addi $sp, $sp, 4
jr $ra
label3:

main:
addi $sp, $sp, -4
sw $fp, 0($sp)
move $fp, $sp
addi $sp, $fp, -40
addi $sp, $sp, -4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addi $sp, $sp, 4
move $t0, $v0
sw $t0, -4($fp)
lw $t0, -4($fp)
sw $t0, -8($fp)
lw $t0, -8($fp)
sw $t0, -12($fp)
li $t0, 1
sw $t0, -16($fp)
lw $t0, -12($fp)
lw $t1, -16($fp)
bgt $t0, $t1, label4
j label5
label4:
lw $t0, -8($fp)
sw $t0, -20($fp)
lw $t0, -20($fp)
addi $sp, $sp, -4
sw $t0, 0($sp)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal fact_
lw $ra, 0($sp)
addi $sp, $sp, 4
addi $sp, $sp, 4
move  $t0, $v0
sw $t0, -24($fp)
lw $t0, -24($fp)
sw $t0, -28($fp)
j label6
label5:
li $t0, 1
sw $t0, -32($fp)
lw $t0, -32($fp)
sw $t0, -28($fp)
label6:
lw $t0, -28($fp)
sw $t0, -36($fp)
lw $t0, -36($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
li $t0, 0
sw $t0, -40($fp)
lw $t0, -40($fp)
move $v0, $t0
move $sp, $fp
lw $fp, 0($sp)
addi $sp, $sp, 4
jr $ra
