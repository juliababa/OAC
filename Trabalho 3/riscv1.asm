
#  Organização e Arquitetura de Computadores - Turma C
#  TRABALHO 3 - o Programação Assembler
#  Aluno(a): Júlia Yuri Garcia Baba
#  Matrícula: 190057921

#  IDE e Compilador: RARS
#  Sistema operacional : Windows 10


.data
color: .word 0xFFFF # azul turquesa
dx: .word 64 # linha com 64 pixels
dy: .word 64 # 64 linhas
org: .word 0x10040000 # endereço da origem da imagem (heap)


#Escreve uma mensagem solicitando que o usuário entre
#com os valores (x0, y0) e (x1, y1) da linha a ser desenhada. 

msg: .asciz "Digite X0, Y0, X1 e Y1:"

    .text
la a0, msg
li a7, 4
ecall

#Lê os valores de (x0, y0) e (x1, y1)
#Lê X0
li a7, 5
ecall
	
mv t0, a0

#Lê Y0
li a7, 5
ecall

mv t1, a0

#Lê X1
li a7, 5
ecall

mv t2, a0

#Lê Y1
li a7, 5
ecall

mv t3, a0

jal linha

li a7, 10
ecall 



#Getaddress(x, y)
getaddress: 
	li a1, 4
	mul a2, a1, s10 #x*4
	
	la t4, dx
	lw t4, 0(t4)
	mul t5, s11, t4 #t5= y*dx
	mul t5, t5, a1 #t5= 4(y*dx)
	
	add a2, a2, t5 #a2= (x*4)+4(y*dx)
	
	la t4, org
	lw t4, 0(t4)
	add a2, a2, t4 #end(a2) = (x*4)+4(y*dx)+org
	
	ret
	
#Função ponto(x, y)
ponto:	
	mv a3, ra
	jal getaddress
	mv ra, a3
	
	la t6, color
	lw t6, 0(t6)
	
	sw t6, 0(a2)
	
	ret
	
linha:
	sub a5, t2, t0 #dx(a5)= x1-x0
	sub a6, t3, t1 #dy(a6)= y1-y0
	
	li  s1, 2
	mul s2, s1, a6 #s2 = 2*dy
	sub s3, s2, a5 #D(s3) = (2*dy) - dx
	
	mv s10, t0
	mv s11, t1
	
	mv s8, ra
	jal ponto
	mv s8, a3
	
	mv s4, t1 #y(s4)=y0
	
	mv s7, t0 #contador 

loop:
	addi s7, s7, 1
	
	bgt s7, t2, fim
	
	bgtz s3, cond #if(D>0)

else:	

	mv s10, s7
	mv s11, s4
	
	mv s8, ra
	jal ponto
	mv s8, a3
	
	add s3, s3, s2
	
	j loop	

cond: 
	addi s4, s4, 1 #y = y+1
	
	mv s10, s7
	mv s11, s4
	
	mv s8, ra
	jal ponto
	mv s8, a3
	
	mul s5, s1, a5 #s5 = 2*dx
	
	sub s6, s2, s5 #s6 = 2*dy - 2*dx
	
	#cond 2
	
	blt  s5, s2, cond2
	
	add s3, s3, s6 #D(s3) = D + (2*dy-2*dx)
		
	j loop

cond2:
	sub s3, s3, s6 #D(s3) = D - (2*dy-2*dx)
	j loop
fim:	
	li a7, 10
	ecall 

	
	
	 
	
	 
	
	
	
	
	
	





