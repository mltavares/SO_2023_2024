all: jogoUI motor bot

jogoUI: jogoUI.c
	gcc -o jogoUI jogoUI.c -lncurses
		
motor: motor.c
	gcc -o motor motor.c -lncurses

bot: bot.c
	gcc -o bot bot.c -lncurses

clean: 
	rm jogoUI motor bot
	  