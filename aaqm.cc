/*
 * sampleelt.{cc,hh} -- sample package element
 * Eddie Kohler
 *
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Mazu Networks, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

// ALWAYS INCLUDE <click/config.h> FIRST
#include <click/config.h>

#include "aaqm.hh"
#include <click/error.hh>

CLICK_DECLS

enum {
	COUNT1 = 0,
	COUNT2,
	COUNT3,
	COUNT4,
	COUNT5,
	DROP1,
	DROP2,
	DROP3,
	DROP4,
	DROP5,
	TOTAL1,
	TOTAL2,
	TOTAL3,
	TOTAL4,
	TOTAL5,
	PULL_COUNT,
	DELETE_COUNT
};

AAQM::AAQM()
{
}

AAQM::~AAQM()
{
	PacketHolder* phl=Queue1->head;
	PacketHolder* temp;
	while(phl!=NULL){
		phl->p->kill();
		temp=phl;
		phl=phl->next;
		delete temp;
	}
	delete Queue1;

	phl=Queue2->head;
	while(phl!=NULL){
		phl->p->kill();
		temp=phl;
		phl=phl->next;
		delete temp;
	}
	delete Queue2;

	phl=Queue3->head;
	while(phl!=NULL){
		phl->p->kill();
		temp=phl;
		phl=phl->next;
		delete temp;
	}
	delete Queue3;

	phl=Queue4->head;
	while(phl!=NULL){
		phl->p->kill();
		temp=phl;
		phl=phl->next;
		delete temp;
	}

	delete Queue4;

	phl=Queue5->head;
	while(phl!=NULL){
		phl->p->kill();
		temp=phl;
		phl=phl->next;
		delete temp;
	}
	delete Queue5;

	Queue1 = NULL;
	Queue2 = NULL;
	Queue3 = NULL;
	Queue4 = NULL;
	Queue5 = NULL;
	printk("Delete all queues.\n");
}

Packet* 
AAQM::dequeuePacket(packetHolderList **Queue)
{
	pull_counter++;
	if((*Queue)->head==NULL)
	{
		return 0;
	}

	else
	{
		PacketHolder *ph=(*Queue)->head;
		Packet *p;

		if(((*Queue)->head)==((*Queue)->tail))
		{
			p = ph->p;
			delete ph;
			(*Queue)->head=NULL;
			(*Queue)->tail=NULL;
		}

		else
		{
			p = ph->p;
			(*Queue)->head=ph->next;
			delete ph;
			delete_counter++;
		}
		(*Queue)->counter--;
		return p;
	}
}

void
AAQM::insertIntoQueue(packetHolderList **Queue, Packet **pkt)
{
	//if((*Queue)->head==NULL && (*Queue)->counter == 0)
	if((*Queue)->head==NULL )
	{
		//PacketHolder *pHolder=new PacketHolder;
    		PacketHolder *pHolder = (PacketHolder*) CLICK_LALLOC(sizeof(PacketHolder) );
		pHolder->next=NULL;
		pHolder->pre=NULL;
		pHolder->p=*pkt;

		(*Queue)->head=pHolder;
		(*Queue)->tail=pHolder;
		(*Queue)->counter=1;
	}

	else
	{
		//PacketHolder *pHolder=new PacketHolder;
    		PacketHolder *pHolder = (PacketHolder*) CLICK_LALLOC(sizeof(PacketHolder) );
		if (pHolder == NULL)
		{
			printk("Not enough memory.\n");
		}
		pHolder->pre=(*Queue)->tail;
		pHolder->next=NULL;
		pHolder->p=*pkt;
		
		(*Queue)->tail->next=pHolder;
	
		(*Queue)->tail=pHolder;
		(*Queue)->counter++;
	}
}

void
AAQM::deleteTail(packetHolderList **Queue){
	if((*Queue)->counter>=1)
	{
		PacketHolder *tail=(*Queue)->tail;
		tail->p->kill();
		(*Queue)->tail=(*Queue)->tail->pre;
		delete tail;
		tail=NULL;
		(*Queue)->counter--;
	}
}
			

int
AAQM::initialize(ErrorHandler *errh)
{
    errh->message("Successfully linked AAQM with package!");

    Queue1=new packetHolderList;
    Queue1->counter=0;
    Queue1->total=0;
    Queue1->dropNum=0;
    Queue1->head=NULL;
    Queue1->tail=NULL;

    //Queue2=new packetHolderList;
    Queue2 = (packetHolderList *) CLICK_LALLOC(sizeof(packetHolderList) );
    Queue2->counter=0;
    Queue2->total=0;
    Queue2->dropNum=0;
    Queue2->head=NULL;
    Queue2->tail=NULL;

    Queue3=new packetHolderList;
    Queue3->counter=0;
    Queue3->total=0;
    Queue3->dropNum=0;
    Queue3->head=NULL;
    Queue3->tail=NULL;

    Queue4=new packetHolderList;
    Queue4->counter=0;
    Queue4->total=0;
    Queue4->dropNum=0;
    Queue4->head=NULL;
    Queue4->tail=NULL;

    Queue5=new packetHolderList;
    Queue5->counter=0;
    Queue5->total=0;
    Queue5->dropNum=0;
    Queue5->head=NULL;
    Queue5->tail=NULL;

    //log_file = fopen("log.txt","rw");
    
    return 0;
}


void
AAQM::push(int port, Packet *p)
{
	switch(port)
	{
		case 0: checkq1(p);
				break;

		case 1: checkq2(p);
				break;

		case 2: checkq3(p);
				break;

		case 3: checkq4(p);
				break;

		case 4: checkq5(p);
				break;
		default:
			printk("Invalid port\n");
			break;
	}

}



Packet *
AAQM::pull(int port)
{
	switch(port)
	{
		case 0: return dequeuePacket(&Queue1);
				

		case 1: return dequeuePacket(&Queue2);
				

		case 2: return dequeuePacket(&Queue3);
				

		case 3: return dequeuePacket(&Queue4);
				

		case 4: return dequeuePacket(&Queue5);
		default:
			printk("Invalid port\n");
			break;
				
	}

}

//if the Packet belongs to q5, this function will be called before it is inserted
void AAQM::checkq5(Packet* p){
	Queue5->total++;
	if(((Queue1)->counter+(Queue2)->counter+(Queue3)->counter+(Queue4)->counter+(Queue5)->counter)<BUFFER_SIZE)//assume the max capacity of buffer is 1000 Packets
	{
		//insert the Packet into q5
		insertIntoQueue(&Queue5,&p);
		return;
	}else{

		if(MQOS5<=(Queue5->counter))
		{
			//the network has been overloaded, and drop that Packet
			Queue5->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			p->kill();
			return;
		}

		if(Queue5->counter<MQOS5){
			//other queues have taken too much space
			if(Queue4->counter>MQOS4){
				deleteTail(&Queue4);
				Queue4->dropNum++;
				//printk("Queue4 is dropping a packet, the total number of packets dropped by queue4 is %d\n",Queue4->dropNum);
				insertIntoQueue(&Queue5,&p);
				return;
			}
			if(Queue3->counter>MQOS3){
				deleteTail(&Queue3);
				Queue3->dropNum++;
				//printk("Queue3 is dropping a packet, the total number of packets dropped by queue3 is %d\n",Queue3->dropNum);
				insertIntoQueue(&Queue5,&p);
				return;
			}
			if(Queue2->counter>MQOS2){
				deleteTail(&Queue2);
				Queue2->dropNum++;
				//printk("Queue2 is dropping a packet, the total number of packets dropped by queue2 is %d\n",Queue2->dropNum);
				insertIntoQueue(&Queue5,&p);
				return;
			}
			deleteTail(&Queue1);
			Queue1->dropNum++;
			//printk("Queue1 is dropping a packet, the total number of packets dropped by queue1 is %d\n",Queue1->dropNum);
			insertIntoQueue(&Queue5,&p);
			return;
		}
	}
	
}

//if the Packet belongs to q4, this function will be called before it is inserted
void AAQM::checkq4(Packet* p){
	Queue4->total++;
	if(((Queue1->counter)+(Queue2->counter)+(Queue3->counter)+(Queue4->counter)+(Queue5->counter))<BUFFER_SIZE)
	{
		//insert the Packet into q4
		insertIntoQueue(&Queue4,&p);
	}else{

		if(MQOS4<=(Queue4->counter))
		{
			//the network has been overloaded, and drop that Packet
			Queue4->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			p->kill();
			return;
		}

		//the network has been overloaded
		if(MQOS5<(Queue5)->counter){
			//drop one Packet from queue 5
			//insert the Packet into queue 4
			deleteTail(&Queue5);
			Queue5->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			insertIntoQueue(&Queue4,&p);
			return;
		}
		if(Queue4->counter<MQOS4){
			if(Queue3->counter>MQOS3){
				deleteTail(&Queue3);
				Queue3->dropNum++;
				//printk("Queue3 is dropping a packet, the total number of packets dropped by queue3 is %d\n",Queue3->dropNum);
				insertIntoQueue(&Queue4,&p);
				return;
			}
			if(Queue2->counter>MQOS2){
				deleteTail(&Queue2);
				Queue2->dropNum++;
				//printk("Queue2 is dropping a packet, the total number of packets dropped by queue2 is %d\n",Queue2->dropNum);
				insertIntoQueue(&Queue4,&p);
				return;
			}
			deleteTail(&Queue1);
			Queue1->dropNum++;
			//printk("Queue1 is dropping a packet, the total number of packets dropped by queue1 is %d\n",Queue1->dropNum);
			insertIntoQueue(&Queue4,&p);
			return;
		}
		//drop that Packet
		Queue4->dropNum++;
		//printk("Queue4 is dropping a packet, the total number of packets dropped by queue4 is %d\n",Queue4->dropNum);
		p->kill();
	}
	return;
} 

void AAQM::checkq3(Packet* p){
	Queue3->total++;
	if(((Queue1->counter)+(Queue2->counter)+(Queue3->counter)+(Queue4->counter)+(Queue5->counter))<BUFFER_SIZE)
	{
		//insert the Packet into q3
		insertIntoQueue(&Queue3,&p);
	}else{

		if(MQOS3<=(Queue3->counter))
		{
			//the network has been overloaded, and drop that Packet
			Queue3->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			p->kill();
			return;
		}


		//the network has been overloaded
		if(MQOS5<(Queue5->counter)){
			//drop one Packet from queue 5
			//insert the Packet into queue 3
			deleteTail(&Queue5);
			Queue5->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			insertIntoQueue(&Queue3,&p);
			return;
		}
		if(MQOS4<(Queue4->counter)){
			//drop one Packet from queue 4
			//insert the Packet into queue 3
			deleteTail(&Queue4);
			Queue4->dropNum++;
			//printk("Queue4 is dropping a packet, the total number of packets dropped by queue4 is %d\n",Queue4->dropNum);
			insertIntoQueue(&Queue3,&p);
			return;
		}
		if(Queue3->counter<MQOS3){
			if(Queue2->counter>MQOS2){
				deleteTail(&Queue2);
				Queue2->dropNum++;
				//printk("Queue2 is dropping a packet, the total number of packets dropped by queue2 is %d\n",Queue2->dropNum);
				insertIntoQueue(&Queue3,&p);
				return;
			}
			deleteTail(&Queue1);
			Queue1->dropNum++;
			//printk("Queue1 is dropping a packet, the total number of packets dropped by queue1 is %d\n",Queue1->dropNum);
			insertIntoQueue(&Queue3,&p);
			return;
		}
		//drop that Packet
		Queue3->dropNum++;
		//printk("Queue3 is dropping a packet, the total number of packets dropped by queue3 is %d\n",Queue3->dropNum);
		p->kill();
	}
	return;
} 

void AAQM::checkq2(Packet* p){
	Queue2->total++;
	if(((Queue1->counter)+(Queue2->counter)+(Queue3->counter)+(Queue4->counter)+(Queue5->counter))<BUFFER_SIZE)
	{
		//insert the Packet into q2
		insertIntoQueue(&Queue2,&p);
	}else{

		if(MQOS2<=(Queue2->counter))
		{
			//the network has been overloaded, and drop that Packet
			Queue2->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			p->kill();
			return;
		}

		//the network has been overloaded
		if(MQOS5<Queue5->counter){
			//drop one Packet from queue 5
			//insert the Packet into queue 2
			deleteTail(&Queue5);
			Queue5->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			insertIntoQueue(&Queue2,&p);
			return;
		}
		if(MQOS4<(Queue4->counter)){
			//drop one Packet from queue 4
			//insert the Packet into queue 2
			deleteTail(&Queue4);
			Queue4->dropNum++;
			//printk("Queue4 is dropping a packet, the total number of packets dropped by queue4 is %d\n",Queue4->dropNum);
			insertIntoQueue(&Queue2,&p);
			return;
		}
		if(MQOS3<(Queue3->counter)){
			//drop one Packet from queue 3
			//insert the Packet into queue 2
			deleteTail(&Queue3);
			Queue3->dropNum++;
			//printk("Queue3 is dropping a packet, the total number of packets dropped by queue3 is %d\n",Queue3->dropNum);
			insertIntoQueue(&Queue2,&p);
			return;
		}		
		if(Queue2->counter<MQOS2){
			deleteTail(&Queue1);
			Queue1->dropNum++;
			//printk("Queue1 is dropping a packet, the total number of packets dropped by queue1 is %d\n",Queue1->dropNum);
			insertIntoQueue(&Queue2,&p);
			return;
		}
		//drop that Packet
		Queue2->dropNum++;
		//printk("Queue2 is dropping a packet, the total number of packets dropped by queue1 is %d\n",Queue2->dropNum);
		temp_count++;
		p->kill();
	}
	return;
} 

void AAQM::checkq1(Packet* p){
	Queue1->total++;
	if((Queue1->counter+Queue2->counter+Queue3->counter+Queue4->counter+Queue5->counter)<BUFFER_SIZE)
	{
		//insert the Packet into q1
		insertIntoQueue(&Queue1,&p);
	}else{

		if(MQOS1<=(Queue1->counter))
		{
			//the network has been overloaded, and drop that Packet
			Queue1->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			p->kill();
			return;
		}


		//the network has been overloaded
		if(MQOS5<(Queue5->counter)){
			//drop one Packet from queue 5
			//insert the Packet into queue 1
			deleteTail(&Queue5);
			Queue5->dropNum++;
			//printk("Queue5 is dropping a packet, the total number of packets dropped by queue5 is %d\n",Queue5->dropNum);
			insertIntoQueue(&Queue1,&p);
			return;
		}
		if(MQOS4<(Queue4->counter)){
			//drop one Packet from queue 4
			//insert the Packet into queue 1
			deleteTail(&Queue4);
			Queue4->dropNum++;
			//printk("Queue4 is dropping a packet, the total number of packets dropped by queue4 is %d\n",Queue4->dropNum);
			insertIntoQueue(&Queue1,&p);
			return;
		}
		if(MQOS3<(Queue3->counter)){
			//drop one Packet from queue 3
			//insert the Packet into queue 1
			deleteTail(&Queue3);
			Queue3->dropNum++;
			//printk("Queue3 is dropping a packet, the total number of packets dropped by queue3 is %d\n",Queue3->dropNum);
			insertIntoQueue(&Queue1,&p);
			return;
		}
		if(MQOS2<(Queue2->counter)){
			//drop one Packet from queue 2
			//insert the Packet into queue 1
			deleteTail(&Queue2);
			Queue2->dropNum++;
			//printk("Queue2 is dropping a packet, the total number of packets dropped by queue2 is %d\n",Queue2->dropNum);
			insertIntoQueue(&Queue1,&p);
			return;
		}
		//drop that Packet
		Queue1->dropNum++;
		//printk("Queue1 is dropping a packet, the total number of packets dropped by queue1 is %d\n",Queue1->dropNum);
		p->kill();
	}
	return;
}
String
AAQM::read_handler(Element *e, void *thunk)
{
    AAQM *aaqm = (AAQM *)e;
    switch ((intptr_t)thunk) {
      case COUNT1:
	return String(aaqm->Queue1->counter);
      case COUNT2:
	return String(aaqm->Queue2->counter);
      case COUNT3:
	return String(aaqm->Queue3->counter);
      case COUNT4:
	return String(aaqm->Queue4->counter);
      case COUNT5:
	return String(aaqm->Queue5->counter);
      case DROP1:
	return String(aaqm->Queue1->dropNum);
      case DROP2:
	return String(aaqm->Queue2->dropNum);
      case DROP3:
	return String(aaqm->Queue3->dropNum);
      case DROP4:
	return String(aaqm->Queue4->dropNum);
      case DROP5:
	return String(aaqm->Queue5->dropNum);
      case TOTAL1:
	return String(aaqm->Queue1->total);
      case TOTAL2:
	return String(aaqm->Queue2->total);
      case TOTAL3:
	return String(aaqm->Queue3->total);
      case TOTAL4:
	return String(aaqm->Queue4->total);
      case TOTAL5:
	return String(aaqm->Queue5->total);
      case PULL_COUNT:
	return String(aaqm->pull_counter);
      case DELETE_COUNT:
	return String(aaqm->delete_counter);
      default:
	return 0;
    }
}
void::
AAQM::add_handlers()
{
    add_read_handler("count1", read_handler, COUNT1);
    add_read_handler("count2", read_handler, COUNT2);
    add_read_handler("count3", read_handler, COUNT3);
    add_read_handler("count4", read_handler, COUNT4);
    add_read_handler("count5", read_handler, COUNT5);
    add_read_handler("drop1", read_handler, DROP1);
    add_read_handler("drop2", read_handler, DROP2);
    add_read_handler("drop3", read_handler, DROP3);
    add_read_handler("drop4", read_handler, DROP4);
    add_read_handler("drop5", read_handler, DROP5);
    add_read_handler("total1", read_handler, TOTAL1);
    add_read_handler("total2", read_handler, TOTAL2);
    add_read_handler("total3", read_handler, TOTAL3);
    add_read_handler("total4", read_handler, TOTAL4);
    add_read_handler("total5", read_handler, TOTAL5);
    add_read_handler("pull", read_handler, PULL_COUNT);
    add_read_handler("delete", read_handler, DELETE_COUNT);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(AAQM)
ELEMENT_REQUIRES(linuxmodule)
