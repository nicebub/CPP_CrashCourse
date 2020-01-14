#include <cstdio>

struct Element {
	Element* next{};
	Element* previous{};
	
	void insert_after(Element* new_element){
		new_element->next = next;
		new_element->previous = this;
		if(next)
			next->previous = new_element;
		next = new_element;
	}
	
	void insert_before(Element* new_element){
		if(previous)
			previous->next = new_element;
		new_element->previous = previous;
		new_element->next = this;
		previous = new_element;
	}
	char prefix[2];
	short operating_number;
};

int main(){
	Element el1, el2, el3, el4, el5;
	
	el1.prefix[0] ='a';
	el1.prefix[1] = 'b';
	el1.operating_number = 8822;

	el2.prefix[0] = 'c';
	el2.prefix[1] = 'd';
	el2.operating_number = 8833;
	
	el3.prefix[0] = 'e';
	el3.prefix[1] = 'f';
	el3.operating_number = 8844;
	
	el4.prefix[0] = 'g';
	el4.prefix[1] = 'h';
	el4.operating_number = 8855;
	
	el5.prefix[0] = 'i';
	el5.prefix[1] = 'j';
	el5.operating_number = 8866;
	
	el1.insert_after(&el2);
//	el3.insert_after(&el4);
	el2.insert_after(&el5);
	el5.insert_before(&el3);
	el5.insert_before(&el4);
	printf("Printing List forwards\n");
	for(Element* el = &el1; el; el = el->next)
		printf("Element operating number: %d\n",el->operating_number);
	printf("\nPrinting List backwards\n");
	for(Element* el = &el5; el; el = el->previous)
		printf("Element operating number: %d\n",el->operating_number);
	
}