// ache; Cheng, Allan

#include <iostream>
using namespace std;
#include <string>
#include <cstring>
#include "classes.h"

void compression();
void decompression();
void hufftree(linkedlist<frequency> list, string);
void buildtable(node<frequency>* *codetable, int size, string);
bool decompress = false;

int main(int argc, char* argv[])
{
  if (argc >= 2 && strcmp(argv[1], "-d") == 0)  
  {
    decompress = true;
    decompression();
  }
  else
    compression();

  return 0;
}

void compression()
{
  string input;
  char temp;
  unsigned char temp2;
  linkedlist<frequency> freq;
  node<frequency>* current;

  while (!cin.eof())
  {
    if (cin.peek() == -1)
      continue;
    cin.get(temp); 
    temp2 = temp;
    input.insert(input.end(), temp2);
    current = freq.find(temp2);

    if (current == NULL)
    {
      frequency something(temp2);
      freq.insert(something);
    }
    else
    {
      (current->data.freq)++;
    }
  }

//  unsigned char in[input.size()+1];
//  for (int c = 0; c < input.size(); c++)
//    in[c] = input[c];
//  strcpy(in, input.c_str());

//  in[input.size()] = '\0';
  freq.qSort(0, freq.size()-1);

  if (freq.getFlag())
  {
    node<frequency>* tempnode = new node<frequency>;
    node<frequency>* tempnode2 = new node<frequency>;
    tempnode = freq.get(0);
    tempnode2 = tempnode->next;

    for (int i = 0; i < freq.size()-2; i++)
    {
      if(tempnode->data.freq == tempnode2->data.freq)
      {
        int j = i;
        while (tempnode2->next != NULL && tempnode->data.freq == tempnode2->data.freq)
        {
          i++;
          tempnode = tempnode->next;
          tempnode2 = tempnode2->next;
        }

        if (tempnode2->next == NULL && tempnode->data.freq == tempnode2->data.freq)
          freq.qSort2(j, i+1);
        else
          freq.qSort2(j, i);
        freq.setFlag(false);
      }

      tempnode = tempnode->next;
      tempnode2 = tempnode2->next;
    }
  }
  hufftree(freq, input);
}

void hufftree(linkedlist<frequency> list, string input)
{ //transform into huffman tree
  
  node<frequency> *node1, *node2, *combo;
  node<frequency>* codetable[list.size()];
  node1 = list.get(0);  //higher priority
  int i;

  for (i = 0; i < list.size(); i++)
  {
    codetable[i] = node1;
    node1 = node1->next;
  }

  node1 = list.get(0);

  if (node1->next == NULL)
    node1->code.append("0");

  while (node1->next != NULL)
  {
    node2 = node1->next;  //lower priority
    combo = list.combine(node2, node1);

    list.push(combo, node2);
    list.deletefirsttwo();

    node1 = list.get(0);
  } //got tree

  buildtable(codetable, i, input);//make table next;
}

void buildtable(node<frequency>* *ctable, int size, string input)
{ //since little endian code = reversed;
  node<frequency>* at;
  table ascii[256];
  int i;
  unsigned char byte;
  unsigned int bin, temp, mask1 = 255;

  string code;

  for (i = 0; i < size; i++)
  {
    at = ctable[i];
    while (at->top != NULL)
    {
      at = at->top;
      ctable[i]->code.append(at->code);
//      ctable[i]->code.insert(0, at->code);
    }
    ascii[ctable[i]->data.character].freq = ctable[i]->data.freq;
    ascii[ctable[i]->data.character].code = ctable[i]->code;
  }

  if (!decompress)
  {
  cout << "HUFFMAN\0";
  byte = 0; //for \0
  cout << byte;

  for (i = 0; i < 256; i++)
  {
    bin = ascii[i].freq;
    for (int j = 0; j < 4; j++)
    {
      temp = bin & mask1;
      byte = temp;
      bin = bin >> 8;
      cout << byte; 
    }
        
  }

string::iterator it;

  for(it = input.begin(); it != input.end(); it++)
  {
    code.insert(0, ascii[(unsigned char)*it].code);
  }

  int remainder = (code.size() % 8); //+2 for 0a
  if (remainder != 0)
    for (i = 0; i < (8 - remainder); i++)
     code.insert(0, "0");

  unsigned char output = 0, tempo = 0;


  for (i = (code.size()/8) - 1; i >= 0; i--)
  {
    int k = 0;
    for (int j = 7; j >= 0; j--)
    {
      if (code[k + (i*8)] == '1') //<---SEGS HERE PROLLY
      {
        tempo = 1;
        tempo <<= j;
        output = output | tempo;
      }
      k++;
    }
  
    cout << output;
    output = 0;
  }
  }
  else //decompress
  {
    string code;
    while (!cin.eof())
    {
      cin >> byte;
      cout << byte;
    } 
  }
}

void decompression()
{
  unsigned char test, bit;
  unsigned int i, bits, byte = 0;
  linkedlist<frequency> defreq;

  while (cin >> test && test != '\0'); //HUFFMAN\0

  for (int j = 0; j < 256; j++)
  {
    for (i = 0; i < 4; i++)
    {
      cin >> bit;
      bit <<= (i*8);
      byte = byte | bit;
    }

    if (byte > 0)
    {
      frequency data(j);
      data.freq = byte;
      defreq.insert(data);
    }
//      cout << j << "'s freq = " << byte << endl;
    
    byte = 0;
  }

  defreq.qSort(0,defreq.size()-1);

  if (defreq.getFlag())
  {
    node<frequency>* tempnode = new node<frequency>;
    node<frequency>* tempnode2 = new node<frequency>;
    tempnode = defreq.get(0);
    tempnode2 = tempnode->next;

    for (int i = 0; i < defreq.size()-2; i++)
    {
      if(tempnode->data.freq == tempnode2->data.freq)
      {
        int j = i;
        while (tempnode2->next != NULL && tempnode->data.freq == tempnode2->data.freq)
        {
          i++;
          tempnode = tempnode->next;
          tempnode2 = tempnode2->next;
        }

        if (tempnode2->next == NULL && tempnode->data.freq == tempnode2->data.freq)
          defreq.qSort2(j, i+1);
        else
          defreq.qSort2(j, i);
        defreq.setFlag(false);
      }

      tempnode = tempnode->next;
      tempnode2 = tempnode2->next;
    }
  }

  string placeholder;
  hufftree(defreq, placeholder);

}

