#include <stdio.h>
#include <stdlib.h>
#include <structure.h>
#include <string.h>
struct list_node* TaskList;
typedef struct rb_node{
	char color;
	struct rb_node* right;
	struct rb_node* left;
	struct rb_node* parent;
	task* mytask;
	int data;
}rb_node;

typedef struct rb_tree{
	rb_node* root;
	int size;
}rb_tree;
int No_Of_Tasks=0;
rb_tree* TaskTree;
rb_node* create_node(int value,task* cur_task ){
	rb_node* node = malloc(sizeof(rb_node));
	node->color = 'R';
	node->right = NULL;
	node->left = NULL;
	node->parent = NULL;
	node->data = value;
	node->mytask = cur_task;
	return node;
}

rb_tree* create_tree(){
	rb_tree* TaskTree = (rb_tree*)malloc(sizeof(rb_tree));
	TaskTree->root = NULL;
	TaskTree->size = 0;
	return TaskTree;
}

void display(rb_node* root){
	if(root != NULL){
		display(root->left);
		printf("%d:%d ",root->data, root->color);
		display(root->right);
	}
}

rb_node* BSTinsert(rb_node* parent,rb_node* root,rb_node* node){
	if (root == NULL){
		printf("Root==NULL\n");
		root = node;
		root->color = 'R';
		root->parent = parent;
		return root;
	}
	
	else if (node->data>root->data){
		printf("node->data>root->data\n");
		root->right = BSTinsert(root,root->right,node);
	}
	
	else if (node->data<root->data){
		printf("node->data<root->data\n");
		root->left = BSTinsert(root,root->left,node);
	}

	return root;
}

void recolor(rb_node* node){
	rb_node* parent = NULL;
	rb_node* grandparent = NULL;
	rb_node* uncle = NULL;
	int temp;

	if (node){	
		// no recoloring if node is root
		if (node != TaskTree->root){
			// as there will be no recoloring then
			if(node->parent != TaskTree->root){
				// to see if there's a violation
				if (node->parent->color == 'R'){
					// initialize parent and grand parent
					parent = node->parent;
					grandparent = node->parent->parent;

					// find out uncle node
					if (parent == grandparent->left){
						uncle = grandparent->right;
					}
					else{
						uncle = grandparent->left;
					}
					
					// check if uncle's red or black

					// if uncle's red
					if((uncle != NULL) && (uncle->color == 'R')){
						uncle->color = 'B';
						parent->color = 'B';
						grandparent->color = 'R';

						// perform recoloring for node's grandparent
						node = grandparent;
						recolor(node);
					}
					
					// uncle's black
					else{
						
						if((parent->left == node) && (grandparent->left == parent)){
							if(grandparent == TaskTree->root){
								TaskTree->root = parent;
							}
							else if(grandparent == grandparent->parent->left){
								grandparent->parent->left = parent;
							}
							else{
								grandparent->parent->right = parent;
							}
							grandparent->left = parent->right;
							if (parent->right){
								parent->right->parent = grandparent;
							}
							parent->right = grandparent;
							parent->parent = grandparent->parent;
							grandparent->parent = parent;
							

							// swapping parent and grand parents colors
							temp = parent->color;
							parent->color = grandparent->color;
							grandparent->color = temp;

							node = parent;
							recolor(node);
						}

						else if((parent->right == node) && (grandparent->left == parent)){
							if(grandparent == TaskTree->root){
								TaskTree->root = node;
							}
							else if(grandparent == grandparent->parent->left){
								grandparent->parent->left = node;
							}
							else{
								grandparent->parent->right = node;
							}
							
							parent->right = node->left;
							parent->parent = node;
							node->parent = grandparent->parent;
							grandparent->left = node->right;
							grandparent->parent = node;
							node->left = parent;
							node->right= grandparent;
							temp = node->color;
							node->color = grandparent->color;
							grandparent->color = temp;

							recolor(node);
						}

						else if((parent->right == node) && (grandparent->right == parent)){

							if(grandparent == TaskTree->root){
								TaskTree->root = parent;
							}
							else if(grandparent == grandparent->parent->left){
								grandparent->parent->left = parent;
							}
							else{
								grandparent->parent->right = parent;
							}
							grandparent->right = parent->left;
							if (parent->left){
								parent->left->parent = grandparent;
							}
							parent->left = grandparent;
							parent->parent = grandparent->parent;
							grandparent->parent = parent;

							// swapping parent and grand parents colors
							temp = parent->color;
							parent->color = grandparent->color;
							grandparent->color = temp;
							
							node = parent;
							recolor(node);
						}

						else if((parent->right == node) && (grandparent->left == parent)){
							if(grandparent == TaskTree->root){
								TaskTree->root = node;
							}
							else if(grandparent == grandparent->parent->left){
								grandparent->parent->left = node;
							}
							else{
								grandparent->parent->right = node;
							}

							node->parent = grandparent->parent;
							grandparent->parent = node;
							parent->parent=node;

							grandparent->right = node->left;
							node->left = grandparent;
							node->right = parent;

							recolor(node);
						}
					}
				}
			}
		}
	}	
	TaskTree->root->color = 'B';
}

void insert(rb_node* node){
	if (TaskTree->root == NULL){
		TaskTree->root = node;
		TaskTree->root->color = 'B';
		TaskTree->size++;
	}

	else{
		TaskTree->root = BSTinsert(NULL, TaskTree->root, node);
		TaskTree->size++;
		recolor(node);
	}
} 

rb_node* BSTsearch(rb_node* root, int data){
	if(root == NULL || root->data == data){
		return root;
	}

	else if(root->data < data){
		//printf("gone right\n");
		return BSTsearch(root->right, data);
	}

	//printf("gone left\n");
	return BSTsearch(root->left, data);
}

rb_node* minValueNode(rb_node* node) { 
    rb_node* current = node; 
  
    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL) 
        current = current->left; 
  
    return current; 
}
rb_node* fixDoubleBlack(rb_node* root){
	rb_node* temp = root;
	if (root != TaskTree->root){
		rb_node *sibling, *parent, *r;

		parent = root->parent;
		if (root == root->parent->left){
			sibling = root->parent->right;
		}
		else{
			sibling = root->parent->left;
		}
		if (sibling != NULL && sibling->color == 'R'){
			
			sibling->color = 'B';
			parent->color = 'R';
			if (parent == TaskTree->root){
				TaskTree->root = sibling;

			}
			else if (parent->parent->left == parent){
				parent->parent->left = sibling;
			}

			else{
				parent->parent->right = sibling;
			}

			if (sibling->parent->left == sibling){
				sibling->right->parent = parent;
				parent->left = sibling->right;
				sibling->right = parent;
			}

			else{
				sibling->left->parent = parent;
				parent->right = sibling->left;
				sibling->left = parent;
			}
			
			sibling->parent = parent->parent;
			parent->parent = sibling;
			temp = NULL;

			TaskTree->root = fixDoubleBlack(root);

		}

		else if (sibling!= NULL && sibling->color == 'B'){

 			if ((sibling->left == NULL || sibling->left->color == 'B') && (sibling->right == NULL || sibling->right->color == 'B')){
 				if(parent->color == 'R'){
 					parent->color = 'B';
 				}
 				
 				else{
 					sibling->color = 'R';
					TaskTree->root = fixDoubleBlack(parent);
 				}
			}
			if((sibling->left != NULL && sibling->left->color == 'R') || (sibling->right != NULL && sibling->right->color == 'R')){

				rb_node *nearChild, *farChild;
				if (sibling->parent->left == sibling){
					nearChild = sibling->right;
					farChild = sibling->left;
				}

				else{
					nearChild = sibling->left;
					farChild = sibling->right;
				}

				if ((farChild == NULL || farChild->color == 'B') && (nearChild && nearChild->color == 'R')){
					sibling->color = 'R';
					nearChild->color = 'B';
					nearChild->parent = sibling->parent;
					sibling->parent = nearChild;
						
					
					if (sibling == sibling->parent->right){
						sibling->left = nearChild->right;
						nearChild->right = sibling;
						sibling = nearChild;
						farChild = sibling->right;
						nearChild = sibling->left;
					}
					else{
						sibling->right = nearChild->left;
						nearChild->left = sibling;
						sibling = nearChild;
						farChild = sibling->left;
						nearChild = sibling->right;
					}

				}
				if (farChild && farChild->color == 'R'){
					int temp_color = parent->color;
					parent->color = sibling->color;
					sibling->color = temp_color;
					farChild->color = 'B';
					if (parent == TaskTree->root){
						TaskTree->root = sibling;
					}
					else if (parent->parent->left == parent){
						parent->parent->left = sibling;
					}
					else{
						parent->parent->right = sibling;
					}

					sibling->parent = parent->parent;
					parent->parent = sibling;
					if (nearChild){
						nearChild->parent = parent;
					}
					if (sibling == parent->left){
						parent->left = nearChild;
						sibling->right = parent;
					}

					else{
						parent->right = nearChild;
						sibling->left = parent;
					}
					display(TaskTree->root);

				}

			}
		}
	}
	return TaskTree->root;
}

rb_node* restructure(rb_node* root){
		rb_node* temp;
		if ((root->color == 'R') && (root->left == NULL && root->right == NULL)){

			if (root->parent->left == root){
				root->parent->left = NULL;
			}
			else{
				root->parent->right = NULL;
			}
			free(root);
			root = NULL;
			temp = NULL;
		}
		
		else if((root->color == 'R') && (root->left)){

			if (root->parent->left == root){
				root->parent->left = root->left;
			}
			else{
				root->parent->right = root->left;
			}
			root->left->parent == root->parent;
			free(root);
			root = NULL;
		}
		
		else if((root->color == 'R') && (root->right)){
			if (root->parent->left == root){
				root->parent->left = root->right;
			}
			else{
				root->parent->right = root->right;
			}
			root->right->parent == root->parent;
			free(root);
			root = NULL;
		}

		else if (root->color == 'B'){

			rb_node* doubleBlack;
			if ((root->left) && (root->left->color == 'R')){
				if(root == TaskTree->root){
					TaskTree->root = root->left;
					root->left->parent = root->parent;
					TaskTree->root->color = 'B';
					free(root);
					root = NULL;
				}
				else{
					if (root->parent->left == root){
						root->parent->left = root->left;
					}
					else{
						root->parent->right = root->left;
					}
					temp = root->left;
					temp->parent == root->parent;
					temp->color = 'B';
				
					free(root);
					root = NULL;
				}
			}
			else if ((root->right) && (root->right->color == 'R')){
				if(root == TaskTree->root){
					TaskTree->root = root->right;
					root->right->parent = root->parent;
					TaskTree->root->color = 'B';
					free(root);
					root = NULL;
				}
				else{
					if (root->parent->left == root){
						root->parent->left = root->right;
					}
					else{
						root->parent->right = root->right;
					}
					temp = root->right;
					temp->parent == root->parent;
					temp->color = 'B';
				
					free(root);
					root = NULL;
				}
			}
			else if (root->left == NULL && root->right == NULL){
				if (root == TaskTree->root){
					TaskTree->root = NULL;
					free(root);
					root = NULL;
				}
				else{
					temp = create_node(0,NULL);       //check i have added NULL
					//printf("\n\n\n\n\n\n");
					temp->parent = root->parent;
					
					if (root->parent->left == root){
						root->parent->left = temp;
					}
					else{
						root->parent->right = temp;
					}
					
					free(root);
					root = NULL;
					
					TaskTree->root = fixDoubleBlack(temp);

					if (temp->parent->left == temp){
					 	temp->parent->left = NULL;
					}
					else{
					 	temp->parent->right = NULL;
					}
					free(temp);
					temp = NULL;
					display(TaskTree->root);
					printf("\n");
					printf("TaskTree->root->data: %d\n", TaskTree->root->data);
				}
			}
			else if (root->left && root->left->color == 'B'){
				if (root == TaskTree->root){
					TaskTree->root = root->left;
					free(root);
					root = NULL;
				}

				else{
					temp = root->left;
					temp->parent = root->parent;
					if (root->parent->left == root){
					 	root->parent->left = temp;
					}
					else{
					 	root->parent->right = temp;
					}
					free(root);
					root = NULL;
					TaskTree->root = fixDoubleBlack(temp);
				}
			}
			else if(root->right && root->right->color == 'B'){
				if (root == TaskTree->root){
					TaskTree->root = root->right;
					free(root);
					root = NULL;
				}
				else{
					temp = root->right;
					temp->parent = root->parent;
					if (temp->parent->left == root){
					 	temp->parent->left = temp;
					}
					else{
					 	temp->parent->right = temp;
					}
					free(root);
					root = NULL;
					TaskTree->root = fixDoubleBlack(temp);
				}
			}

		}
		printf("Displaying from end of restructure.\n");
		display(TaskTree->root);
		printf("\n");
		return TaskTree->root;
}

rb_node* delete(rb_node* root, int data){
    if (root == NULL) return root; 
    if (data < root->data) 
        delete(root->left, data); 
    else if (data > root->data) 
        delete(root->right, data); 
    else
    { 
        rb_node* temp;
		if (root->left && root->right){
        	temp = minValueNode(root->right); 
        	root->data = temp->data;
  
        	// Delete the inorder successor 
        	delete(root->right, temp->data);
		}

		else{
			TaskTree->root = restructure(root);
		}
		TaskTree->size--;
    }
    
    return TaskTree->root; 
}
void search(rb_node* root, char *name){
	if(root == NULL) return;
	search(root->left, name);
	int same = strcmp(name, root->mytask->name);
	if(same == 0) {
		delete(TaskTree->root, root->data);
		return;
	}
	search(root->right, name);	
}
void printTree(rb_node* root){
	if(root!=NULL){
	printTree(root->left);
	printf("Name=%s  Priority %d  Root Value= %d\n",root->mytask->name,root->data,TaskTree->root->data);
	printTree(root->right);	
	}
}

