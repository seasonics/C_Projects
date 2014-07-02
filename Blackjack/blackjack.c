#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

typedef enum {HEART=100, DIAMOND, CLUB, SPADE} suit_t;
typedef enum {JACK=100, QUEEN, KING, ACE};

// basic card structure has a value which represents
// the face value of a card (e.g. 2,3...10,jack,queen,king,ace)
// and a enum suit which corresponds to Hearts, Diamonds, etc.
struct card
{
	int value;
	suit_t suit;	
};

//A linked list with a card as its data
struct node 
{
	struct card *c;
	struct node *next;
};

// A structure to represent a hand
// keeps track of aces in hand for conversions
// becasue an ace can be 1 or 11. Has a linked
// list to represent the hand
struct hand
{  
	int split;
	int aces;
	int value;
	struct node *c_root;	
	struct node *s_root;	
};

// a deck is 52 cards, as defined as above. When a 
// card is used in the blackjack game it is swapped
// to the last position and the deck size decreases by 
// one
struct deck 
{
	struct card *deck[52];
	int size;
};

// initilizes a card to specified value and suit
struct card *init_card(int val, suit_t suit)
{  
	struct card *c = (struct card *) malloc(sizeof(struct card));
	c->value = val;
	c->suit = suit;
	return c;
};

// initilizes a hand and a null linked list
struct hand *init_hand()
{
	struct hand *h = (struct hand *) malloc(sizeof(struct hand));
	struct node *r = (struct node *) malloc( sizeof(struct node) );
	struct node *s = (struct node *) malloc( sizeof(struct node) );
	h->value = 0;
	h->aces = 0;
	r->c = NULL;
	r->next = NULL;
	s->c = NULL;
	s->next = NULL;
	h->c_root = r;
	h->s_root = s;
	return h;
};

// inits the deck with 52 cards
struct deck *init_deck(void)
{	
	int i = 0,j,val,p = 0;
	suit_t suit;
	struct deck *d = malloc(sizeof(struct deck));
	d->size=52;
	while (i < 4)
	{   
		j = 0;
		switch (i)
		{
			case 0:
				suit = HEART;
				break;
			case 1:
				suit = DIAMOND;
				break;
			case 2:
				suit = CLUB;
				break;
			case 3:
				suit = SPADE;
				break;
		}
		while(j < 13) 
		{
			switch(j)
			{
				case 9:
					val = JACK;
					break;
				case 10:
					val = QUEEN;
					break;
				case 11:
					val = KING;
					break;
				case 12:
					val = ACE;
					break;
				default:
					val = j+2;
					break; 
			}
			p = (i*12)+j+i;
			d->deck[p] = init_card(val, suit);
			j++;
		}
		i++;
	}
	return d;	
}

void free_hand(struct hand *h)
{
	struct node *p, *next;
	 for (p = h->c_root; NULL != p; p = next)
	  {
    next = p->next;
    free(p);
	  }	
}

// converts a card to its value
int card_to_num(int value)
{
 	switch(value)
	{
		case JACK:
		case QUEEN:
		case KING:
			return 10;
			break;
		case ACE:
			return 11;
			break;
		default:
			return value;
			break;
	}	
	
}

// converts the card value to a string for
// easy printing
char *card_num_to_string(int value)
{  
	char *str = malloc(20);
 	switch(value)
	{
		case JACK:
			str = "JACK";
			break;
		case QUEEN:
			str= "QUEEN";
			break;
		case KING:
			str= "KING";
			break;
		case ACE:
			str= "ACE";
			break;
		default:
			sprintf(str, "%d", value);
			return str;
			break;
	}
	return str;	
	
}


int player_hand_value(struct node* n)
{
	int value = 0;
	struct node *r;	
	r = n;
	if ( r != NULL ) 
	{
        while ( r->next != NULL)
        {
        		value = value + card_to_num(r->c->value);
            r = r->next;
        }
        value = value + card_to_num(r->c->value);
    }
	return value;
	
}

// converts the card suit to a string for
// easy printing
char *card_suit_to_string(int value)
{  
	char *str = malloc(20);
 	switch(value)
	{
		case DIAMOND:
			str = "DIAMONDS";
			break;
		case HEART:
			str= "HEARTS";
			break;
		case CLUB:
			str= "CLUBS";
			break;
		case SPADE:
			str= "SPADES";
			break;

	}
	return str;	
	
}

// Prints the whole deck to see if constructed correctly
void print_deck (struct deck *d)
{	
	int i = 0;
	struct card *c;
	while(i<52)
	{
		c = d->deck[i];
		printf("card %d has val %d and suit %d", i, c->value, c->suit);
		fflush(stdout);
		i++;
	}	
}

//prints the hand of a player
void print_hand_player(struct hand *h)
{
	struct node *r;
	struct card *c;
	int i = 1;
	if (h->split)
	{
		i = 2;
	}
	while(i>0)
	{
		 if (i == 2) 
		 {	
		 	printf("First hand is\n");
		 	r = h->s_root;
		 }
		 else 
		 	r = h->c_root;
	    if ( r != NULL ) 
		{
	        while ( r->next != NULL)
	        {	
				c = r->c;
				printf("%s of %s\n", card_num_to_string(c->value), card_suit_to_string(c->suit));
	         r = r->next;
	            
	        }
	        c = r->c;
	        printf("%s of %s\n", card_num_to_string(c->value), card_suit_to_string(c->suit));
	    }
		 if (i == 2) 
		 {	
		 	printf("Second hand is\n");
		 }    
	    i--;
	  }
 }
 
 //same as above but only prints first card for dealer
 void print_hand_dealer(struct hand *h)
{
	struct node *r;
	struct card *c;
	c = h->c_root->c;
	printf("%s of %s\n", card_num_to_string(c->value), card_suit_to_string(c->suit));
 }

// takes a card out of the deck at random
// and returns it and moves it to 
// the back of the deck while decreasing deck size
struct card *hit(struct deck *d)
{
	//note: not truly random but close enough for a game of blackjack
	int r = rand() % (d->size);
	struct card* c;
	c = d->deck[r];
	void*temp = d->deck[d->size-1];
	d->deck[d->size-1]=d->deck[r];
	d->deck[r] = temp;
	d->size--;
	return c;
}

int player_hand_aces(struct node* n)
{
	int value = 0;
	struct node *r;	
	r = n;
	if ( r != NULL ) 
	{
        while ( r->next != NULL)
        {
        		if (r->c->value == ACE)
        			value++;
            r = r->next;
        }
        if (r->c->value == ACE)
        			value++;
    }
	return value;
	
}


// adds the value of the card to the hand
// builds a new node in the linked list and puts
// the new card there
void add_val(struct hand *h, struct card *c, int split)
{	
	struct node *n =(struct node *) malloc( sizeof(struct node) );
	struct node *r;
	
	n->next = NULL;
	n->c = c;
	if (split)
		r = h->s_root;
	else
		r = h->c_root;
	if ( r != NULL ) 
	{
        while ( r->next != NULL)
        {
            r = r->next;
        }
    }
	r->next = n;
	if (h->split)
	{
		if (split)
			h->value = player_hand_value(h->s_root);
		else 
			h->value = player_hand_value(h->c_root);
	}
	else 
		h->value = h->value + card_to_num(c->value);
}

// deals the two cards in the beginning
struct hand *deal(struct deck *d, int p_or_d)
{
	struct card *c,*t;
	struct hand *h = init_hand();
	c = hit(d);
	if (c->value == ACE)
		h->aces++;
	h->c_root->c = c;
	h->value = card_to_num(h->c_root->c->value);
	t = hit(d);
	if (t->value == ACE)
		h->aces++;
	if (c->value == t->value && p_or_d)
		{
			
			printf("Player was dealt:\n");
			printf("%s of %s\n", card_num_to_string(c->value), card_suit_to_string(c->suit));
			printf("%s of %s\n", card_num_to_string(t->value), card_suit_to_string(t->suit));
			char buf[80];
			char *p;
			while(1)
			{
				buf[0] = '\0';
				printf ("Do you want to split:Y/N ");
		   	fflush (stdout);
				p = fgets (buf, 80, stdin);
				if (strncmp ( p, "Y", 1) == 0)
				{
					h->split = 1;
					h->s_root->c = t;
					return h;
				}
				else if (strncmp ( p, "N", 1) == 0)
				{
					add_val(h,t,0);
					return h;
				}			
				else
					printf ("Not A Valid Command\n");
			}
		}
	add_val(h,t,0);
	return h;
}





// A turn where the player hits
// grabs a card, adds the value to the hand
// and prints new hand value, or values in case of soft hands(Aces)
int player_hit_turn(struct hand *h,struct deck *d ,int p_or_d,int split)
{	
	char *person;
	int aces;
	if (p_or_d == 0)
		person = "Dealer";
	else
		person = "Player";
	struct card* c;
	int i = 0;
	c = hit(d);
	if (c->value == ACE)
	 h->aces++;
	add_val(h,c,split);
	printf("\n%s was dealt:\n", person);
	printf("%s of %s\n", card_num_to_string(c->value), card_suit_to_string(c->suit));
	printf("\n%s has hand:\n", person);
	print_hand_player(h);
	if (h->split)
		 if (split)
		 	aces = player_hand_aces(h->s_root);
		 else 
		 	aces = player_hand_aces(h->c_root);
	else 
		aces = h->aces;
	if (h->aces > 0)
	{	printf("With values:\n");
		i = h->aces;
		while (i>-1)
		{
			printf("%d\n", h->value - (i*10));
			i--;
		}
		sleep(3);
		if (h->value -(h->aces*10) > 21)
		{
			return 0;	
		}
		return 1;
	}
	printf("With value:\n");
	printf("%d\n", h->value);
	sleep(3);
	if (h->value > 21)
	{
		return 0;	
	}
	return 1;
}



//returns the highest hand with ace
//thats under 22
int highest_w_ace(struct hand *h)
{
	int i = 0;
	while (i<h->aces+1)
	{
		if (h->value - (i*10) <22)
			return h->value - (i*10);
		i++;
	}
}

int highest_w_ace_split(struct node *n)
{
	int i = 0;
	int value = player_hand_value(n);
	int aces = player_hand_aces(n);
	while (i<aces+1)
	{
		if (value - (i*10) <22)
			return value - (i*10);
		i++;
	}
}




// checks for blackjack in beginning deal
int check_for_blackjack(struct hand *h)
{
	if ((card_to_num(h->c_root->c->value) == 10 && card_to_num(h->c_root->next->c->value) == 11)
		|| (card_to_num(h->c_root->c->value) == 11 && card_to_num(h->c_root->next->c->value) == 10))
		return 1;
	return 0;
}

// the continue playing loop prompt
int continue_playing(void)
{
	char buf[80];
	char *p;
	
	while(1)
	{
		buf[0] = '\0';
		printf ("Do you want to continue:Y/N ");
    	fflush (stdout);
		p = fgets (buf, 80, stdin);
		if (strncmp ( p, "Y", 1) == 0)
		{
			return 1;
		}
		else if (strncmp ( p, "N", 1) == 0)
		{
			return 0;
		}			
		else
			printf ("Not A Valid Command\n");
	}
}

//Asks for a bet from the player, checks if 
//its a valid bet and returns it.
int prompt_for_bet(int player_bet)
{
	int bet;
	char buf[80];
	char *p;
	while(1)
	{
		buf[0] = '\0';
		printf ("Player has %d chips\n", player_bet);
		printf ("Enter your bet: ");
		fflush (stdout);
		p = fgets (buf, 80, stdin);
		bet = strtol(p, &p,10);
		if (bet == 0)
		{
			printf("Not a valid bet \n");
			continue;
		}
		if (bet > player_bet)
		{
			printf("You do not have that much money \n");
			continue;
		}
		return bet;
	}
}

//Checks the initial deal for blackjack
// and prints the initial deal hands
int initial_deal(struct hand *player_hand, struct hand *dealer_hand)
{
	int p_bj = 0, d_bj =0;
	printf("\nDealing \n");
	fflush(stdout);
	printf("Player has hand\n");
	print_hand_player(player_hand);
	printf("Dealer has hand\n");
	print_hand_dealer(dealer_hand);
	printf("and 1 unknown\n");
	if (player_hand->split)
		p_bj =0;
	else
		p_bj = check_for_blackjack(player_hand);
	d_bj = check_for_blackjack(dealer_hand);
	if (p_bj == 1 && d_bj == 0)
	{
		printf ("Player has blackjack, player wins\n");
		printf ("Dealer has:\n");
		print_hand_player(dealer_hand);
		return 1;
	}
	if (p_bj == 1 && d_bj == 1)
	{
		printf ("Player has blackjack, dealer has blackjack, tie\n");
		return 0;
	}
	return -1;
}

//The loop for the player turn, where the player can hit or stand
int player_hit_loop(struct hand *player_hand,struct deck *d )
{
	while(1)
	{
		int k;
		char buf[80];
		char *p;
		printf ("\nEnter H for hit or S for stand: ");	
		fflush (stdout);
		p = fgets (buf, 80, stdin);
		if (strncmp ( p, "S", 1) == 0)
		{
			if (player_hand->aces > 0)
				player_hand->value = highest_w_ace(player_hand);
			return 1;
		}
		else if (strncmp ( p, "H", 1) == 0)
		{
			k = player_hit_turn(player_hand,d, 1,0);
			if (k == 0)
			{
				printf ("Player has busted, player loses\n");
				return 0;
			}	
		}
		else 
		{
			printf ("Invalid...");	
		}
   }
}

int player_hit_loop_split(struct hand *player_hand,struct deck *d )
{
	int i = 0;
	int bust_1= 0;
	int bust_2 = 0;
	i = 2;
	int play_num;
	int hand_value= 0;
	int p_hand;
	while(i>0)
	{
		int k;
		char buf[80];
		char *p;
		if (i == 2)
			play_num = 1;
		else 
			play_num = 2;
		printf ("\nEnter H for hit or S for stand for hand %d:", play_num);	
		fflush (stdout);
		p = fgets (buf, 80, stdin);
		if (strncmp ( p, "S", 1) == 0)
		{
			if (player_hand->aces > 0)
			{
				if (i ==2)
					p_hand = highest_w_ace_split(player_hand->s_root);
				else 
					p_hand = highest_w_ace_split(player_hand->c_root);
				if (p_hand > hand_value) 
					hand_value = p_hand;
				
			}
			else 
			{
				if(i==2)
					p_hand = player_hand_value(player_hand->s_root);
				else 
					p_hand = player_hand_value(player_hand->c_root);
				if (p_hand > hand_value) 
					hand_value = p_hand;
			
			}
			print_hand_player(player_hand);
			i--;
		}
		else if (strncmp ( p, "H", 1) == 0)
		{
			if (i ==2)
				k = player_hit_turn(player_hand,d, 1,1);
			else
				k = player_hit_turn(player_hand,d, 1,0);
			if (k == 0)
			{
				if (i ==2)
				{
					printf ("Player hand %d has busted\n\n", play_num);
					bust_1 = 1;
					print_hand_player(player_hand);
					i--;
					hand_value = 0;
				}
				else
				{
					printf ("Player hand %d has busted\n\n", i);
					bust_2 = 1;
					print_hand_player(player_hand);
					hand_value = 0;
					i--;
				}
			}	
		}
		else 
		{
			printf ("Invalid...please enter H for hit or S for stand: ");	
		}
	}
	player_hand->value = hand_value;
	if (bust_1 == 1 && bust_2 == 1)
	{
		printf ("Player has busted both hands\n");
		player_hand->value = 0;
		return 0;
	}
	return 1;
   
}






//The loop that models the dealers turn
int dealer_hit_loop(struct hand *dealer_hand,struct deck *d )
{
	printf("\nDealer has hand\n");
	print_hand_player(dealer_hand);
	int k=1,d_bj = check_for_blackjack(dealer_hand);
	if (d_bj == 1)
	{
		printf ("Dealer has blackjack, dealer wins\n");
		return 1;
	}
	while (dealer_hand->value -(dealer_hand->aces*10) <20)	
	{	
	
		if (dealer_hand->aces == 0)
		{
			if(dealer_hand->value>16)
				break;
		}
		k = player_hit_turn(dealer_hand,d, 0,0);
		if (highest_w_ace(dealer_hand) > 19)
		{
			return -1;		
		}
	}
	if (k == 0)
	{	
		printf ("Dealer has busted, player wins\n");
		return 0;
	}
	return -1;
}





int
main (void)
{	
	//inits the random
	srand(time(NULL));
	int s_loop;
	//variables
	char buf[80];
	char *p;
	int deal_i;
	struct hand* player_hand, *dealer_hand;
	int bet = 0;
	int j,k,p_bj,d_bj,i = 1;
	int player_bet = 100;
	struct deck *d = init_deck();
	//main loop
    while(i)
	{	
		//resets deck size and char buffer
		// and other variables
		d->size = 52;
		buf[0] = '\0';
		j = 1,k = 1,p_bj = 1,d_bj = 1;
		//enter bet
		bet = prompt_for_bet(player_bet);
		//initial deal
		player_hand = deal(d,1);
		dealer_hand = deal(d,0);
		deal_i = initial_deal(player_hand, dealer_hand);
		if (deal_i == 1 || deal_i == 0)
		{	
			if (deal_i == 1)
				player_bet = player_bet + bet *1.5;
			free_hand(player_hand);
			free_hand(dealer_hand);
			if (continue_playing())
				continue;
			else
				break;
		}
		// player hit loop
		if (player_hand->split)
		{
				s_loop = player_hit_loop_split(player_hand,d);
				bet = bet + bet;
		}
		else 	
			s_loop = player_hit_loop(player_hand,d);
				
      if (s_loop == 0)
		{
			player_bet = player_bet - bet;
			free_hand(player_hand);
			free_hand(dealer_hand);
			if (continue_playing())
				continue;
			else
				break;
		}
	   // dealer hit loop
	   j = dealer_hit_loop (dealer_hand,d);
       if (j == 0 || j == 1)
	   {
			if (j == 0)
				player_bet = player_bet + bet;
			if (j ==1)
				player_bet = player_bet - bet;
			free_hand(player_hand);
			free_hand(dealer_hand);
			if (continue_playing())
				continue;
			else
				break;
		}
		printf ("Dealer stands\n");
		if (dealer_hand->aces > 0)
			dealer_hand->value = highest_w_ace(dealer_hand);
		if (dealer_hand->value < player_hand->value)
		{	
			printf ("Player wins\n");
			printf ("Dealer's hand value is %d, Players hand value is %d\n",dealer_hand->value,player_hand->value );
			player_bet = player_bet + bet;
		}
		if (dealer_hand->value > player_hand->value)
		{	
			printf ("Dealer wins\n");
			printf ("Dealer's hand value is %d, Players hand value is %d\n",dealer_hand->value,player_hand->value );
			player_bet = player_bet - bet;
		}
		if (dealer_hand->value == player_hand->value)
		{	
			printf ("Tie\n");
			printf ("Dealer's hand value is %d, Players hand value is %d\n",dealer_hand->value,player_hand->value );
			player_bet = player_bet;
		}	
		if (player_bet < 1)
		{
			printf ("Player has lost all money\n");
			break;	
		}
		free_hand(player_hand);
	   free_hand(dealer_hand);
	   if (continue_playing())
			continue;
		else
			break;
	}
	free(d);		
	printf ("Thanks for Playing\n");	
}


