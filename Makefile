blackjack: blackjack.o
	gcc -o blackjack blackjack.o -I.
clean:
	rm -rf *o blackjack
