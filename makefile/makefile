#obj=main.o add.o sub.o mul.o div.o

target=app
#makefile中字节维护的变量
CC = gcc
#Makefile中的函数的使用
src=$(wildcard ./*.c)
obj=$(patsubst ./%.c, ./%.o, $(src))

$(target):$(obj)
	$(CC) $(obj) -o $(target)

%.o:%.c
	$(CC) -c $< -o $@

#声明clean是一个伪目标
.PHONY:clean
clean:
	#-的作用是如果该命令执行失败,忽略该命令执行下面的命令
	-mkdir /aa
	rm $(obj) $(target) -f

hello:
	echo "hello,makefile"
