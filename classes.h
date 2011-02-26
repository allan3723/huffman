// ache; Cheng, Allan

template<class T>
class node
{
  public:
    T data;
    node<T>* next;
    node<T>* back;
    node<T>* left;
    node<T>* right;
    node<T>* top;
    string code;
};
/*
template<class T>
class treenode
{
  public:
    T data;
    treenode<T>* left;
    treenode<T>* right;
};
*/
class huffcode
{
  public:
    int code;
    char character;
};
/*
template<class T>
class tree
{
  public:
    tree()
    {
      treesize = 0;
    }

    treenode<T>* combine(treenode<T>* left, treenode<T>* right)
    {
      treenode<T>* node = new treenode<T>;
      left->data.code.insert(left->data.code.begin(), '0');
      right->data.code.insert(right->data.code.begin(), '1');
      node->left = left;
      node->right = right;
      return node;
    }

    treenode<T>* makenode(char c)
    {
      treenode<T>* node = new treenode<T>;
      node->data.character = c;
      return node;
    }

  private:
    treenode<T>* root, current, temp;
    int treesize;
};
*/
class frequency
{
  public:
    frequency() {freq = 0;}
    frequency(unsigned char input)
    {
      freq = 1;
      character = input;
    }

    unsigned int freq;
    unsigned char character;
};

class table
{
  public:
    table() {freq = 0;}
    string code;
    unsigned int freq;
};

template<class T>
class linkedlist
{
  public:

  linkedlist()
  {
    lsize = 0;
  }

  void insert(T data)
  {
    if (lsize == 0)
    {
      head = new node<T>;
      end = new node<T>;
      head->data = data;
      head->next = NULL;
      head->back = NULL;
      head->left = NULL;
      head->right = NULL;
      head->top = NULL;
      end->back = head;
      current = head;
      lsize = 1;
    }
    else
    {
      temp = new node<T>;
      temp->data = data;
      temp->next = NULL;
      temp->back = current;
      temp->left = NULL;
      temp->right = NULL;
      temp->top = NULL;
      current->next = temp;
      end->back = temp;
      current = temp;
      lsize++;
    }
  }

  int size() {return lsize;}

  node<T>* find(char data)
  {
    if (lsize == 0)
      return NULL;

    temp = head;
  
    while (temp != NULL)
    {
      if ((temp->data).character == data)
        return temp;

      temp = temp->next;
    }

    return NULL;
  }

  void print()
  {
    if (lsize == 0)
      return;

    temp = head;
    while (temp != NULL)
    {
      if (temp->data.character == '\n')
        cout << "\\n" << " ascii = " << (int)'\n' << " " 
             << temp->data.freq << endl;
      else
        cout << temp->data.character << " ascii = " 
          << (int)temp->data.character << " " << temp->data.freq << endl;
      temp = temp->next;
    }
  }

  node<T>* get(int pos)
  {
    temp = head;
    for (int i = 0; i < pos; i++)
      temp = temp->next;

    return temp;
  }

  void push(node<T>* insertn, node<T>* low)
  {
    temp = low;

    while (temp->next != NULL && insertn->data.freq > temp->data.freq)
      temp = temp->next;

    if (insertn->data.freq == temp->data.freq)
    {
      while (temp->next != NULL && 
             insertn->data.character > temp->data.character)
        temp = temp->next;
    }

    if (temp->next == NULL && insertn->data.freq >= temp->data.freq)
    {
      if (insertn->data.freq == temp->data.freq && insertn->data.character <
          temp->data.character)
      {
        temp->back->next = insertn;
        insertn->next = temp;
        insertn->back = temp->back;
        temp->back = insertn;
      }
      else
      {
        temp->next = insertn;
        insertn->next = NULL;
        insertn->back = temp;
      }
    }
    else
    {
      temp->back->next = insertn;
      insertn->next = temp;
      insertn->back = temp->back;
      temp->back = insertn;
    }

  }

  void deletefirsttwo()
  {
    temp = head->next->next;
//    delete head->next;
//    delete head;
    head = temp;
    head->back = NULL;

  }

  node<T>* combine(node<T>* low, node<T>* high)
  {
    node<T>* combo = new node<T>;
    low->code.append("0");
    low->top = combo;
    high->code.append("1");
    high->top = combo;
    combo->left = low;
    combo->right = high;
    combo->top = NULL;
    combo->data.character = high->data.character; //takes highest priority
    combo->data.freq = low->data.freq + high->data.freq;

    return combo;
  }

  void qSort(const int left, const int right)
  {
    if (left < right)
    {
      int i = left,
          j = right + 1;
      node<T>* pivot = get(left);
      node<T>* temp1 = pivot;
      node<T>* temp2;

      if (j >= lsize) temp2 = end;
      else
        temp2 = get(j);

      do
      {
        do
        {
          i++;
          temp1 = temp1->next;

//if (temp1->data.freq < pivot->data.freq)
//{
//cout << "freq1: " << temp1->data.freq << " < " << pivot->data.freq << endl;
//cout << "temp1 = " << temp1->data.character << " and pivot = " 
//     << pivot->data.character << endl;
//}

          if (temp1->data.freq == pivot->data.freq) // && i != left)
            setFlag(true);
        } while(/*i < j*/ temp1->next != NULL && temp1->data.freq < pivot->data.freq);

        do
        {
          j--;
          temp2 = temp2->back;
          if (temp2->data.freq == pivot->data.freq) // && j != left)
            setFlag(true);
if (temp2->data.freq > pivot->data.freq)
{
//cout << "freq2: " << temp2->data.freq << " > " << pivot->data.freq << endl;
//cout << "temp2 = " << temp2->data.character << " and pivot = "
//     << pivot->data.character << endl;
}

        } while(/*j > i*/ temp2->back != NULL && temp2->data.freq > pivot->data.freq);
// cout << i << " < " << j << endl;
//if (i < j)
//cout << "swapped: " << temp1->data.character << " and " << temp2->data.character << endl; 
        if (i < j) swap(temp1, temp2);
      } while (i < j);

//cout << "outside swap: " << pivot->data.character << " and " << temp2->data.character << endl;
      swap(pivot, temp2);
      qSort(left, j-1);
      qSort(j+1, right);
    }
  }


  void qSort2(const int left, const int right)
  {
    if (left < right)
    {
      int i = left,
          j = right + 1;
      node<T>* pivot = get(left);
      node<T>* temp1 = pivot;
      node<T>* temp2;

      if (j >= lsize) temp2 = end;
      else
        temp2 = get(j);

      do
      {
        do
        {
          i++;
          temp1 = temp1->next;
        } while(temp1->next != NULL && temp1->data.character < pivot->data.character);// && i < right);

        do
        {
          j--;
          temp2 = temp2->back;
        } while(temp2->back != NULL && temp2->data.character > pivot->data.character); // && j >=i);
        if (i < j) swap(temp1, temp2);
      } while (i < j);

      swap(pivot, temp2);
      qSort2(left, j-1);
      qSort2(j+1, right);
    }
  }

  void swap(node<T>* node1, node<T>* node2)
  {
    node<T>* tempnode = new node<T>;
    tempnode->data.character = node1->data.character;
    tempnode->data.freq = node1->data.freq;
    node1->data.character = node2->data.character;
    node1->data.freq = node2->data.freq;

    node2->data.character = tempnode->data.character;
    node2->data.freq = tempnode->data.freq;

    delete tempnode;
  }

  void setFlag(bool something) {flag = something;}

  bool getFlag() {return flag;}

  private:
    int lsize;
    node<T> *head, *temp, *current, *end;
    bool flag;
};
