//Program by Christina Ochsner
//PS#: 1166214

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class list
{
	private:
		//nodes for list defined
		struct node
		{
			string data;
			struct node *next = NULL;
			struct node *prev = NULL;
		};
		//head of list
		struct node *head = NULL;
	public:
		//checks if the list is empty
		bool empty()
		{
			if (head == NULL)
			{
				return true;
			}
			return false;
		}
		//returns the size of list's inherited classes. Pure virtual function makes list an abstract class.
		virtual int size() = 0;
	protected:
		//insert string x into list index pos
		void insert(string x, int pos)
		{
			//create node to be inserted
			struct node *newnode = new node;
			newnode->data = x;
			
			//if adding to head of list, change head to newnode
			if (pos == 0)
			{
				newnode->next = head;
				head = newnode;
				return;
			}

			//set variables
			struct node *current = head;
			int count = 0;

			//parse through list until currents next node is NULL (we're on the last node) or we've reach our desired position in the list
			while (current->next != NULL && count<pos)
			{
				current = current->next;
				count++;
			}

			//if the next node was NULL but the next position was not equal, do not add to the list as the pos is not valid
			if (current->next == NULL&&count<pos-1)
			{
				return;
			}

			//if the next node was NULL and was also going to be equal to our desired position, add the node to the end of the list
			if (current->next == NULL && count == pos-1)
			{
				newnode->prev = current;
				current->next = newnode;
				return;
			}

			//if our next node was not NULL, we know we are adding to the middle of the list at our position. Reset the links to add the node.
			newnode->prev = current->prev;
			newnode->next = current;
			current->prev = newnode;
			current->prev->next = newnode;
		}
		string remove(int pos, bool &success)
		{
			//set up variables
			struct node *temp = head;
			string item="";
			success = true;

			//if list is empty, cannot delete, return an empty string.
			if (head == NULL)
			{
				success = false;
				return item;
			}

			//if the list is not empty and we are deleting the head of the list
			if (pos == 0)
			{
				//store the data from head which we are deleting in item
				item = head->data;
				//set head to the variable after it
				head = head->next;

				//if there was another node after head, reset it's prev link to NULL
				if (head!=NULL&&head->prev != NULL)
				{
					head->prev = NULL;
				}

				//free the memory head was previously occupying
				free(temp);

				//return the string we just deleted from the list
				return item;
			}

			//if we are not deleting from the beginning of the list
			int count = 0;
			//parse through the list until we either reach the end of the list or the position of the list where we want to delete
			while (temp != NULL&&count<pos)
			{
				count++;
				temp = temp->next;
			}

			//the position given was not in the list
			if (temp == NULL)
			{
				success = false;
				return item;
			}

			//the position given was in the list, so store it's data into item
			item = temp->data;

			//if there's a node after what we're deleting, reset it's prev pointer
			if (temp->next != NULL)
			{
				temp->next->prev = temp->prev;
			}
			//reset the previous nodes next pointer to pointer to the node after the node we are trying to delete
			temp->prev->next = temp->next;

			//free the memory in temp
			free(temp);

			//return the data from the node we just deleted
			return item;
		}
		string retrieve(int pos, bool &success)
		{
			//set up variables
			struct node *temp = head;
			string item = "";
			success = true;

			//if list is empty set success to false and return an empty string
			if (head == NULL)
			{
				success = false;
				return item;
			}

			//return the data from the head of the list
			if (pos == 0)
			{
				item = head->data;
				return item;
			}

			//parse through the list until it is either empty or we reach are position
			int count = 0;
			while (temp != NULL && count < pos)
			{
				count++;
				temp = temp->next;
			}

			//the position was not occupied by the list, set success to false and return an empty string
			if (temp == NULL)
			{
				success = false;
				return item;
			}

			//set the data at our position in temp to string item
			item = temp->data;

			//return the data occupied by the passed position
			return item;
		}
};

class stack: public list
{
	private:
		bool success = true;
		int s = 0;
	public:
		int size();
		string top();
		string pop();
		void push(string el);
};
//return the size of the stack, override the list size() function
int stack::size()
{
	return s;
}
//return the data at the top of the stack
string stack::top()
{
	return retrieve(s-1, success);
}
//delete from the top of the stack
string stack::pop()
{
	string item;

	//if there something in the stack, you can pop
	if (s > 0)
	{
		//call remove to delete from end of list (also top of stack)
		item = remove(s - 1, success);

		//if we were able to delete, decrease the size
		if (success)
		{
			s--;
		}
	}
	
	//return the data we just deleted
	return item;
}
void stack::push(string el)
{
	//increase the size
	s++;

	//push an element at the end of the list (or top of the stack)
	insert(el, s-1);
}

class queue: public list
{
	private:
		bool success = true;
		int s= 0;
	public:
		//return the size of the queue, override list size() function
		int size()
		{
			return s;
		}
		//return the data occupied by the head of the list (also the front of the queue)
		string front()
		{
			return retrieve(0, success);
		}
		//add string el to the end of the list (or rear of the queue). Incrememnt s variable to track size
		void enqueue(string el)
		{
			s++;
			insert(el, s - 1);
		}
		//delete from head of list (also front of queue)
		string dequeue()
		{
			string item;
			
			//store removed item in string item
			item = remove(0, success);

			//if we successfully deleted, decrement s
			if (success)
			{
				s--;
			}

			//return deleted data
			return item;
		}
};

int main(int argc, char *argv[])
{
	char input[25]; //= "input.txt";
	char output[25]; //= "output.txt";
	char types;
	int i=0;
	int y=1;
	int o;
	
	//Parse command line to retrieve and store file names.
	while(argv[y][i]!='\0')
	{
		switch(argv[y][i])
		{
			case '=':
			{
				i++;
				o=0;
				while(argv[y][i]!='\0'&&argv[y][i]!=';'&&argv[y][i]!=' ')
				{
					switch(types)
					{
						case 'i':
						{
							input[o]=argv[y][i];
							if(argv[y][i+1]=='\0'||argv[y][i+1]==';')
							{
								input[o+1]='\0';
							}
						}
						break;
						
						case 'o':
						{
							output[o]=argv[y][i];
							if(argv[y][i+1]=='\0'||argv[y][i+1]==';')
							{
								output[o+1]='\0';
							}
						}
						break;
					}
					o++;
					i++;
				}
				if((argv[y][i]=='\0'||argv[y][i]==';')&&argc>2&&y<2)
				{
						i=0;
						y++;
				}
			}
			break;
			
			case 'i':
			{
				types='i';
				i++;
			}
			break;
			
			case 'o':
			{
				if(argv[y][i+1]=='u')
				{
					types='o';
				}
				i++;
			}
			break;
			
			default:
			{
				i++;
			}
			break;
		}
	}
	
	ifstream in (input);
	ofstream out (output);

	string line;

	queue q;
	stack s;

	while (getline(in, line))
	{
		istringstream ss(line);

		string title;
		string word;
		string command;

		ss >> command;
		ss >> word;

		title = word;

		while (ss >> word)
		{
			title = title + " " + word;
		}

		if (command == "buy")
		{
			s.push(title);
			q.enqueue(title);
		}
		else if (command == "sale")
		{
			int costQ = 0;
			int costS = 0;

			queue tempQ;
			queue tempQ2;

			while (!s.empty() && s.top() != title)
			{
				tempQ.enqueue(s.pop());
				costS++;
			}
			
			while (!q.empty() && q.front() != title)
			{
				tempQ2.enqueue(q.dequeue());
				costQ++;
			}
			
			if (q.empty() && s.empty())
			{
				cout << title << " not found" << endl;
				out << title << " not found" << endl;
			}
			else if (s.top() == title && q.front() == title)
			{
				s.pop();
				q.dequeue();
				costQ++;
				costS++;
				cout << title << " finding cost at stack: " << costS << ", at queue: " << costQ << endl;
				out << title << " finding cost at stack: " << costS << ", at queue: " << costQ << endl;
			}
			
			while (!tempQ.empty()) 
			{
				s.push(tempQ.dequeue());
			}
			
			while(!tempQ2.empty())
			{
				q.enqueue(tempQ2.dequeue());
			}
		}
	}
	
	in.close();
	out.close();

	//system("pause");

	return 0;
}
