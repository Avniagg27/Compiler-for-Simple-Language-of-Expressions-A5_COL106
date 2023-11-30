
// /* Do NOT add/remove any includes statements from this header file */
// /* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"

// //Write your code below this line


void setheight(SymNode* root);
int getbal(SymNode* root);

SymbolTable::SymbolTable(){
    size = 0;
    root = NULL;
}

SymNode* ins_help(SymNode* node,string k,SymNode* p=NULL) {
    if (node == NULL) {
        SymNode* t=new SymNode(k);
        t->par=p;
        return t;
    }

    if (k < node->key) { 
        node->left = ins_help(node->left,k,node);
    } 
    else if (k > node->key) {
        node->right = ins_help(node->right,k,node);
    } 
    else {
        return node;
    }
    
    setheight(node);

    int bal = getbal(node);
    
    if(bal>1 && k<node->left->key){
        return node->RightRightRotation();
    }    
    else if(bal<-1 && k<node->right->key){
        return node->RightLeftRotation();
    }
    else if(bal>1 && k>node->left->key){
        return node->LeftRightRotation();
    }
    else if(bal<-1 && k>node->right->key){
        return node->LeftLeftRotation();
    }

    return node;
}

void SymbolTable::insert(string k){                                     
    if(search(k)!=-2){
        return;
    }
    root=ins_help(root,k,NULL);
    size++;
}

SymNode* findmin(SymNode* root){
    SymNode* node = root;
    while(node->left!= NULL){
        node = node->left;
    }
    return node;
}
SymNode* rem_help(SymNode* node,string k, SymNode* p=NULL) {
    if (node == NULL){
        return node;
    }
    if (k < node->key){
        node->left = rem_help(node->left,k,node);
    }
    else if (k > node->key){
        node->right = rem_help(node->right,k,node);
    }
    else { 
        if((node->left == NULL) || (node->right == NULL)) {
            SymNode* t;
            if(node->left!=NULL){
                t = node->left;
            }
            else{
                t = node->right;
            }
            if (t == NULL){
                t = node;
                node = NULL;
            }
            else{
                t->par=p;
                *node = *t;
            }
            free(t);

        }
        else{
            SymNode* t1 = findmin(node->right);
            node->key = t1->key;
            node->address = t1->address;
            node->right = rem_help(node->right,t1->key,node);
        }

    }
    if (node == NULL){
        return node;
    }

    node->par = p;
    setheight(node);

    int bal = getbal(node);
    if (bal > 1) {
        if(getbal(node->left) >= 0){
            return node->RightRightRotation();
        } 
        else{
            return node->LeftRightRotation();
        }
    }
    if(bal < -1){
        if(getbal(node->right) <= 0){
            return node->LeftLeftRotation();
        } 
        else{
            return node->RightLeftRotation();
        }
    }
    return node;
}

void SymbolTable::remove(string k) {
    if(search(k) != -2){  
        root = rem_help(root,k,NULL);
        size--;
    }
}
int SymbolTable::search(string k){
    SymNode* temp = root;
    while(temp!= NULL){
        if(temp->key ==k){
            return temp->address;
        }
        else if(k<temp->key){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
    }
    return -2;
}

void SymbolTable::assign_address(string k,int idx){
    SymNode* temp = root;
    while(temp!= NULL){
        if(temp->key == k){
            temp->address = idx;
            break;
        }
        else if(k< temp->key){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
    }
}

int SymbolTable::get_size(){
    return size;
}

SymNode* SymbolTable::get_root(){
    return root;
}

void del(SymNode* root){
    if(root){
        del(root->left);
        del(root->right);
        delete root;
    }
}

SymbolTable::~SymbolTable(){
    del(root);
}
