// ache; Cheng, Allan

#include <fstream>
#include <iostream>
using namespace std;
#include <string>
#include <cstring>
#include "classes.h"

void compression();
void decompression();
void hufftree(linkedlist<frequency> list);
void buildtable(node<frequency>* *codetable, int size);
void translate(linkedlist<frequency> tree, unsigned int totalchar);
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
  linkedlist<frequency> freq;
  node<frequency>* current;
  unsigned int length, streampos;
  unsigned char temp;

  cin.seekg(0, ios::end);
  length = cin.tellg();
  cin.seekg(0, ios::beg);

  for (streampos = 1; streampos <= length; streampos++)
  {
    temp = cin.peek();
    cin.seekg(streampos); 
    current = freq.find(temp);

    if (current == NULL)
    {
      frequency something(temp);
      freq.insert(something);
    }
    else
    {
      (current->data.freq)++;
    }
  }

  freq.qSort(0, freq.size()-1);

  if (freq.getFlag())
  {
    node<frequency>* tempnode = new node<frequency>;
    node<frequency>* tempnode2 = new node<frequency>;
    tempnode = freq.gethead();
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

/*
  node<frequency>* tempnode = new node<frequency>;
  tempnode = freq.get(0);
  for (int i = 0; i < freq.size(); i++)
  {
    cout << "freq = " << tempnode->data.freq << " char = " 
         << (int)tempnode->data.character << endl;
    tempnode = tempnode->next;
  }
*/

  hufftree(freq);
}

void hufftree(linkedlist<frequency> list)
{ //transform into huffman tree
  
  node<frequency> *node1, *node2, *combo;
  node<frequency>* codetable[list.size()];
  node1 = list.gethead();  //higher priority
  int i;

  for (i = 0; i < list.size(); i++)
  {
    codetable[i] = node1;
    node1 = node1->next;
  }

/*
node1 = list.gethead();
cout << "list size = " << list.size() << endl;
while (node1 != NULL)
{
cout << (int)node1->data.character << ": " << node1->data.freq << "\t";
node1 = node1->next;
}
cout << endl;
return;*/
/* 
  node1 = list.gethead();
  while (node1 != NULL)
  {
    cout << "char: " << (int)node1->data.character << " is repeated " << node1->data.freq << " times\n";
    node1 = node1->next;
  }
*/

  node1 = list.gethead();

  if (node1->next == NULL)
    node1->code.append("0");

  while (node1->next != NULL)
  {
    node2 = node1->next;  //lower priority
    combo = list.combine(node2, node1);
//cout << (int)node2->data.character << " + " << (int)node1->data.character 
//<< " - frequencies: " << node2->data.freq << " + " << node1->data.freq << endl;
//    list.deletefirsttwo();
    list.push(combo, node2);
    list.deletefirsttwo();

    node1 = list.gethead();
/*    while (node1!= NULL)
    {
cout << (int) node1->data.character << " ";
node1 = node1->next;
    }
node1 = list.get(0);
cout << endl;
*/
  } //got tree

  if (!decompress)
    buildtable(codetable, i);//make table next;
  else
  {
    node1 = list.gethead();
    translate(list, node1->data.freq);
  }
}

void buildtable(node<frequency>* *ctable, int size)
{
  node<frequency>* at;
  table ascii[256];
  int i;
  unsigned char byte;
  unsigned int bin, temp, mask1 = 255;

  for (i = 0; i < size; i++)
  {
    at = ctable[i];

    while (at->top != NULL && at->top->top != NULL)
    {
      at = at->top;
      ctable[i]->code.append(at->code);
    }

    ascii[ctable[i]->data.character].freq = ctable[i]->data.freq;
    ascii[ctable[i]->data.character].code = ctable[i]->code;
  }

  cout << "HUFFMAN\0";
  byte = 0; //for \0
  cout << byte;
//cout << endl;
//  for (i = 0; i < size; i++)
//  {
//    cout << (int)ctable[i]->data.character << "'s code is " << ctable[i]->code << endl;
//  }

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
/*
  for(it = input.begin(); it != input.end(); it++)
  {
    code.insert(0, ascii[(unsigned char)*it].code);
  }

  int remainder = (code.size() % 8); //+2 for 0a
  if (remainder != 0)
    for (i = 0; i < (8 - remainder); i++)
     code.insert(0, "0");
*/
  unsigned char output = 0, tempo = 0;

int count = 0, length;
string::iterator it2;
char sign;
unsigned char letter;

  length = cin.tellg();
  cin.seekg(0, ios::beg);

  for (i = 0; i < length; i++)
  {
    cin.get(sign);
    letter = sign;

    for (it2 = --ascii[letter].code.end(); it2 >= ascii[letter].code.begin(); it2--)
    {
      if (*it2 == '1')
      {
        tempo = 1;
        tempo <<= count;
        output = output | tempo;
      }

      count++;
      if (count == 8)
      {
        cout << output;
        count = 0;
        output = 0;
      }
    }
  }

  if (count%8 != 0)
    cout << output;

/*
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
*/
}

void decompression()
{
  char temp;
  unsigned char test, bit;
  unsigned int i, bits, byte = 0;
  linkedlist<frequency> defreq;

  while (cin >> test && test != '\0'); //HUFFMAN\0

  for (int j = 0; j < 256; j++)
  {
//cout << j << ": ";
    for (i = 0; i < 4; i++)
    {
      cin.get(temp);
      bit = temp;
//cout << (int)bit << " ";
      bits = bit << (i*8);
      byte = byte | bits;
    }
//if ((j+3)%4 == 0)
//cout << endl;
//else
//cout << "\t";
    if (byte > 0)
    {
//cout << (unsigned char)j << "'s byte = " << byte << endl;
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
    tempnode = defreq.gethead();
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

/*
  node<frequency>* tempnode = new node<frequency>;
  tempnode = defreq.gethead();
  for (int i = 0; i < defreq.size(); i++)
  {
    cout << "freq = " << tempnode->data.freq << " char = " 
         << (int)tempnode->data.character << endl;
    tempnode = tempnode->next;
  }
*/
  hufftree(defreq);

}

void translate(linkedlist<frequency> tree, unsigned int totalchar)
{
  node<frequency>* root = new node<frequency>;
  root = tree.gethead();
  unsigned char byte;
  unsigned int mask = 1, bit;
  int i;
  char temp;
//cout << totalchar <<endl;
//return;
  while (totalchar > 0)
  {
    cin.get(temp);
    byte = temp;

    for (i = 0; i < 8; i++)
    {
      if (totalchar == 0)
        break;
      bit = byte >> i;
      bit = bit & mask;

      if (bit == 1 && root->right != NULL)
      {
        root = root->right;
//cout << bit;
        if (root->right == NULL)
        {
          cout << root->data.character;
          root = tree.gethead();
          totalchar--;
//cout << endl;
        }
      }
      else if (bit == 0 && root->left != NULL)
      {
        root = root->left;
//cout << bit;
        if (root->left == NULL)
        {
          cout << root->data.character;
          root = tree.gethead();
          totalchar--;
//cout << endl;
        }
      }
    }
  }
  
}
