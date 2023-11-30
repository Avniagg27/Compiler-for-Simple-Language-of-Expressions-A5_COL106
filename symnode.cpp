/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"

//Write your code below this line

SymNode::SymNode(){
    key = "";
    height = -1;
    par = NULL;
    left = NULL;
    right = NULL;
}

SymNode::SymNode(string k){
    key = k;
    height = 0;
    par = NULL;
    left = NULL;
    right = NULL;
}

int getheight(SymNode* root){
    
    if(root==NULL){
        return -1;
    }
    if(root->left == NULL && root->right == NULL){
        return 0;
    }
    return root->height;
}

void setheight(SymNode* root){
    if(root==NULL){
        root->height = -1;
    }
    if(root->left != NULL || root->right != NULL){
        root->height = 1 + max(getheight(root->left), getheight(root->right));
    }
    else{
        root->height = 0;
    }
}

int getbal(SymNode* root){
    
    if(root==NULL){
        return 0;
    }
    int bal = getheight(root->left) - getheight(root->right);
    
    return bal;
}
SymNode* SymNode::LeftLeftRotation(){
    SymNode* temp = this->right;
    this->right = temp->left;
    
    if(temp->left != NULL){
        temp->left->par = this;
    }

    temp->left = this;
    temp->par = this->par;
    if(this->par!=NULL){
    if(this->par->key > this->key){
        this->par->left = temp;
    }
    else{
        this->par->right = temp;
    }
    }
    this->par = temp;


    setheight(this);
    setheight(temp);

    return temp;
}

SymNode* SymNode::RightRightRotation(){
    SymNode* temp = this->left;
    this->left = temp->right;

    if(temp->right != NULL){
        temp->right->par = this;
    }

    temp->right = this;
    temp->par = this->par;
    if(this->par !=NULL){
        if(this->par->key > this->key){
            this->par->left = temp;
        }
        else{
            this->par->right = temp;
        }
    }
    this->par = temp;

    setheight(this);
    setheight(temp);

    return temp;
}

SymNode* SymNode::LeftRightRotation(){
    this->left = this->left->LeftLeftRotation();
    return RightRightRotation();
}

SymNode* SymNode::RightLeftRotation(){
    this->right = this->right->RightRightRotation();
    return LeftLeftRotation();
}

SymNode::~SymNode(){
    par = NULL;
    left = NULL;
    right= NULL;
}
