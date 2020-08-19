/******************************
This program will read a text file containing characters and frequencies, from this information it will build a huffman tree. It will encode a string to its huffman code and then decode back to its the original string
 ******************************/
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;
//Struct for huffman tree nodes
struct HT_Node
{
  float weight;
  HT_Node *lc, *rc, *parent;

  HT_Node(float value)
  {
    weight = value;
    lc = NULL;
    rc = NULL;
    parent = NULL;
  }
};

/******************************
This function will read the freq.txt file to extract the characters and frequencies
Parameters: vector<char>& lett - vector to hold the characters, vector<float>& freq - hold the frequencies
Return: void
 ******************************/
void initVectors(vector<char>& lett, vector<float>& freq)
{
  string line;//each line from the file
  char letter;//individual character
  float numfreq;//individual frequencies

  ifstream fin;
  fin.open("freq.txt");//open the file

  if(fin.fail())//if file couldnt open
      {    
	cout<<"Input file doesnt exist"<<endl;
	return;
      }
    
  while(getline(fin,line))//whie it still reads information
      {
	letter = line[3];//get the individual character
	lett.push_back(letter);//push it into the vector
	
	string num = line.substr(9,5);//get the frequencie
	numfreq = atof(&num[0]);//convert from string to float
	freq.push_back(numfreq);//push into freq vector
      }
  fin.close();//clode input file
}

/******************************
This function will create a collection of individual tree nodes for each frequency
Parameters: vector<HT_Node**>& nodes - collection of tree nodes, vector<float>& freq - frequencies
Return: void
 ******************************/
void initTreeNodes(vector<HT_Node*>& nodes,vector<float>& freq)
{
  int size = freq.size();

  for(int i = 0;i < size;i++)
    {
      HT_Node* newnode = new HT_Node(freq[i]);
      nodes.push_back(newnode);
    }
}

/******************************
This function finds the smallest frequency value in the node collection, second time it gets called it will return the secodn smallest
Parameter: vector<HT_Node*>& nodes - collection of nodes, int prevsmall - previously found smallest value 
Return: index of the smallest value
 ******************************/
int findSmallest(vector<HT_Node*>& nodes,int prevsmall)
{
  int index;
  if(prevsmall == 0)//if the first smallest number found was at first index then only search size-1
    {
      index = 1;//start at index 1
      for(int i = 2;i < nodes.size();i++)//go through all nodes
	{
	  HT_Node* smallest = nodes[index];//smallest value
	  HT_Node* next = nodes[i];//node to check and compare
	  
	  if(next->weight < smallest->weight && i != index && i != prevsmall)//if this value is smaller and its not the index we already set and its not the previously found
	    {
	      index = i;//new smallest
	    }
	}
    }
  else
    {
      index = 0;//initialize to first index
      for(int i = 1;i < nodes.size();i++)//go through all nodes
	{
	  HT_Node* smallest = nodes[index];
	  HT_Node* next = nodes[i];
	  if(next->weight < smallest->weight && i != index && i != prevsmall)
	    {
	      index = i;
	    }
	}
    }
  return index;//return index of smallest
}

/******************************
This unction goes the node collection and builds the huffman tree
Parameters:vector<HT_Node*>& nodes - collection of tree nodes
Return: void
 ******************************/
void buildHuffTree(vector<HT_Node*>& nodes)
{
  while(nodes.size() > 1)//while we still have more than one node in the vector
    {
      int index1 = findSmallest(nodes,-1);//find smallest value
      int index2 = findSmallest(nodes,index1);//second smallest

      HT_Node* min1 = nodes[index1];//get the smallest node
      HT_Node* min2 = nodes[index2];//second smallest

      float sum = min1->weight + min2->weight;//add the weight of the two smallest
      HT_Node* sumNode = new HT_Node(sum);//create new node with weight at sum

      //create smallest element left child and next smallest right child
      sumNode->lc = min1;
      sumNode->rc = min2;
      //sum node becomes root node of the smaller nodes
      min1->parent = min2->parent = sumNode;

      nodes.push_back(sumNode);//push new sum node into collection

      //remove the 2 small nodes from the collection
      nodes.erase (nodes.begin()+index1);
      if(index1 < index2)//if first node is reased first then adjust the index for the second node
	nodes.erase(nodes.begin()+(index2-1));
      else
	nodes.erase(nodes.begin()+index2);     
    }
}

/******************************
This function will generate an individual character of the huffman code for the corresponding node
Parameters:HT_Node* node - node we want to encode
Return: string with huffman code for the corresponding node
 ******************************/
string genCode(HT_Node* node)
{
  string code;
  HT_Node* curr = node;
  HT_Node* parent = curr->parent;
  
  while(parent != NULL)//while we havnt reached the top of the tree
    {
      if(parent->lc == curr)//node we were at is on the left
	code.insert(0,"0");
      else//node is on the right
	code.insert(0,"1");
      curr = parent;    
      parent = curr->parent;
    }
  return code;//return code for the node provided
}

/******************************
This function will check if the node we are currently at is a leaf node which means its a node with one of the initial frequencies
Parameters: HT_Node* node - node to check if its a leaf
Return: true/false
 ******************************/
bool isLeaf(HT_Node* node)
{
  if(node->lc == NULL && node ->rc == NULL)//we are at a leaf node
    return true;
  else
    return false;
}

/******************************
This function will get the frequency of the corresponding character
Parameters: vectpr<float> freq -  vector of frequencies, char input - character we are working with
Return: corresponding frequency
 ******************************/
float getFreq(vector<float> freq,char input)
{
  int index;//index where the character is
  if(input > 64 && input < 91)//capital letter
    index = input - 64;
  else if(input > 96 && input < 123)//lowercase letter
    index = input - 96;
  else if(input == ' ')//space
    index = 27;
  else //default
    index = 0;
  
  return freq[index-1];//return the corresponding frequecy
}

/******************************
This function finds the index of the frequency we are looking for
Parameters: vector<HT_Node*> nodevect - vector of individual tree nodes, float freq - frequency whos index we want to find
Return: index of the node
 ******************************/
int getIndex(vector<HT_Node*> nodevect, float freq)
{
  HT_Node* current;
  int i;
  for(i = 0;i<nodevect.size();i++)//go through all the nodes
    {
      current = nodevect[i];
      if(current->weight == freq)
	return i;
    }
  return -1;
}

/******************************
This function will ask for a string and then it will produce the corresponding huffman code
Parameters: HT_Node* root - root node of hufftree, vector<float> freq - vector of frequencies, vector<HT_Node*> nodeVect - vector of individual tree nodess
Return: huffman code
 ******************************/
string encode(HT_Node* root,vector<float> freq,vector<HT_Node*> nodeVect)
{
  string huffcode;
  string input;//string to encode
  char find;//each individual character of the string
  float charFreq;//frequency of the character
  HT_Node* node;//pointer to node of character we will generate a code for
  int index;
 
  cout<<"\nEnter string you wish to encode: ";
  getline(cin,input);//read input
 
  for(int i = 0;i<input.size();i++)//go through entire string 
   {
     find = input[i];//individual char of the string
     charFreq = getFreq(freq,find);//frequency of the character
     index = getIndex(nodeVect,charFreq); //find index of the frequency
     
     if(index >= 0)//if frequency exists
       {
	 node = nodeVect[index];//get node corresponding to the frequency
	 huffcode += genCode(node);//generate huffman code for that frequency
       }
     else//that character isnt valid
       {
	 cout<<"Character "<<find<<" doesnt exist in the huffman tree"<<endl; 
	 break;  
	 
       }   
   }
  return huffcode;//return the huffman code
}

/******************************
This function get the letter character corresponding to a frequency
Parameters: float num - frequency whose character we want to find, vector<float> freq - vector of the frequencies 
Return: index of the character
 ******************************/
int getLett(float num,vector<float> freq)
{
  int index;
  
  for(int i = 0; i < freq.size(); i++)//go through frequency vector
    {
      if(freq[i] == num)//if we find the frequency then return its index
	return i;
    }
}

/******************************
This function decodes a huffman code and returns its original text
Parameters: string code - huffman code, HT_Node* root - root node of hufftree, vector<char>& lettV - vector of characters, vector<float> freq - vector of frequencies
Return: string with original text
 ******************************/
string decode(string code,HT_Node* root,vector<char>& lettV,vector<float> freq)
{
  string original;
  HT_Node* child = root;
  
  for(int i = 0;i < code.size();i++)//go entire huffcode
    {
      char lett = code[i];//char from code string 0/1
      if(lett == '0')//go left
	child = child->lc;
      else//go right
	  child = child->rc;
      
      if(isLeaf(child))//reached the node 
	{
	  int index = getLett(child->weight,freq);
	  char lett = lettV[index];
	  original += lett;
	  child = root;//reset pointer to root
	}
    }
  return original;//return decodes string
}

int main()
{
  vector<char> lett;//holds the characters from freq.txt
  vector<float> freq;//holds the frequencies from text file
  vector<HT_Node*> nodeVect;//individual tree nodes
  vector<HT_Node*> huffVect;//hold all nodes used to build hufftree
  string huffmanCode;//code string after encoding
  string text;//text string after decoding
  
  initVectors(lett, freq);//will read freq.txt to extract the characters and frequencies
  initTreeNodes(nodeVect, freq);//make a tree node for each frequency
  huffVect = nodeVect;//make a copy of the collection of nodes
  buildHuffTree(huffVect);//buid the huffman tree
 
  HT_Node* root = huffVect[0];//only node left is the root node to hufftree
  
  huffmanCode = encode(root,freq,nodeVect);//encode input and get huffman code
  cout<<"Huffman Code: "<<huffmanCode<<endl;//output code
  
  text = decode(huffmanCode,root,lett,freq);//decode huffman code and get original string
  cout<<"\nDecoding huffman code..."<<endl;//
  cout<<"\nOriginal text: "<<text<<endl<<endl;//output original string 
  return 0;
}
