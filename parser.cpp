/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "parser.h"

//Write your code below this line

Parser::Parser(){
    symtable = new SymbolTable();
    last_deleted = -3;
}

ExprTreeNode* parsehelp(vector<string> exp, long unsigned int i, SymbolTable* symtable){
    ExprTreeNode* pres = new ExprTreeNode();
    vector<ExprTreeNode*> stk;

    while(i <exp.size()){

        if(exp[i]=="("){
            stk.push_back(pres);
            pres->left = new ExprTreeNode();
            pres = pres->left;
        }
        else if (exp[i]== "+"){
            pres->type = "ADD";
            stk.push_back(pres);
            pres->right = new ExprTreeNode();
            pres = pres->right;
        }
        else if (exp[i]== "-"){
            pres->type = "SUB";
            stk.push_back(pres);
            pres->right = new ExprTreeNode();
            pres = pres->right;
        }
        else if (exp[i]== "*"){
            pres->type = "MUL";
            stk.push_back(pres);
            pres->right = new ExprTreeNode();
            pres = pres->right;
        }
        else if (exp[i]== "/"){
            pres->type = "DIV";
            stk.push_back(pres);
            pres->right = new ExprTreeNode();
            pres = pres->right;
        }
        else if (exp[i]== ")"){
            if(!stk.empty()){
                pres = stk.back();
                stk.pop_back();
            }
        }
        else if(isdigit(exp[i][0])){
            pres->type = "VAL";
            pres->num = std::stoi(exp[i]);

            if(!stk.empty()){
                pres = stk.back();
                stk.pop_back();
            }
        }
        else if(exp[i][0]=='-' && isdigit(exp[i][1])){
            pres->type = "VAL";
            pres->num = -std::stoi(exp[i].substr(1));

            if(!stk.empty()){
                pres = stk.back();
                stk.pop_back();
            }
        }
        else{
            pres->type = "VAR";
            pres->id = exp[i];
            if(!stk.empty()){
                pres = stk.back();
                stk.pop_back();
            }
        }

        i++;
    }

    return pres;
}

void Parser::parse(vector<string> expression){
    ExprTreeNode* root = new ExprTreeNode(":=", 0);
    expr_trees.push_back(root);
    if(expression[0]=="del"){
        root->left = new ExprTreeNode("DEL", 0);
        root->right = new ExprTreeNode("VAR", 0);
        root->right->id = expression[2];
        last_deleted = symtable->search(expression[2]);
        symtable->remove(expression[2]);
    }

    else if(expression[0]=="ret"){
        root->left = new ExprTreeNode("RET", 0);
        root->right = parsehelp(expression,2,symtable);
    }
    else{
        root->left = new ExprTreeNode("VAR", 0);
        root->left->id = expression[0];
        root->right = parsehelp(expression,2,symtable);
        symtable->insert(root->left->id);

    }
}

Parser::~Parser(){

}
