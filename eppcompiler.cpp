
/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "eppcompiler.h"

//Write your code below this line

EPPCompiler::EPPCompiler(){
    
}

EPPCompiler::EPPCompiler(string out_file,int mem_limit){
    output_file = out_file;
    memory_size = mem_limit;
    for(int i =0; i<memory_size; i++){
        mem_loc.push_back(i);
    }
}

void helper_generate_targ_commands(ExprTreeNode* node, vector<string>& v,Parser targ){
    if(node == NULL){
        return;
    }
    if(node->right!=NULL){
        helper_generate_targ_commands(node->right, v,targ);
    }
    if(node->left!=NULL){
        helper_generate_targ_commands(node->left, v,targ);
    }
    if(node->left==NULL && node->right == NULL){
        
        if(node->type == "VAR"){
            int mem_idx = targ.symtable->search(node->id);
            if(mem_idx != -2){
                v.push_back("PUSH mem[" + to_string(mem_idx) + "]");
                return;
            }
            else{
                v.push_back("PUSH mem[" + to_string(targ.last_deleted) + "]");
                return;
            }
        }
        else if(node->type == "VAL"){
            v.push_back("PUSH "+ to_string(node->num));
            return;
        }
    }
    if(node->type == "ADD"){
        v.push_back("ADD");
        return;
    }
    else if(node->type == "SUB"){
        v.push_back("SUB");
        return;
    }
    else if(node->type == "MUL"){
        v.push_back("MUL");
        return;
    }
    else if(node->type == "DIV"){
        v.push_back("DIV");
        return;
    }
}


void EPPCompiler::compile(vector<vector<string>> code){
    
    vector<string> v;
    
    for(long unsigned int i =0; i<code.size(); i++){
        targ.parse(code[i]);
        if(targ.expr_trees[i]->left->type == "VAR"){
            int add;
            string s = code[i][0];
            int x = targ.symtable->search(s);
            if((x==-1)||(x==-2)){ 
                add = mem_loc.back();
                mem_loc.pop_back();
                targ.symtable->assign_address(s,add);
            }
            
        }
        else if(targ.expr_trees[i]->left->type == "RET"){
            break;
        }
    }

    v = generate_targ_commands();
    
    for(long unsigned int i = 0; i<code.size(); i++){
        if(targ.expr_trees[i]->left->type=="DEL"){
            mem_loc.push_back(targ.last_deleted);
            targ.last_deleted = mem_loc.back();
        }
    }
    
    write_to_file(v);
}

vector<string> EPPCompiler::generate_targ_commands(){

    vector<string> v;
    
    for(long unsigned int i =0; i<targ.expr_trees.size(); i++){

        ExprTreeNode* root = targ.expr_trees[i];

        if(root->left->type == "DEL"){
            v.push_back("DEL = mem[" + to_string(targ.last_deleted) + "]");
        }
        else if(root->left->type == "VAR"){
            vector<string> t;
            helper_generate_targ_commands(root->right, t, targ);
            int mem_idx = targ.symtable->search(root->left->id);
            v.insert(v.end(),t.begin(),t.end());
            t.clear();
            if(mem_idx != -2){
                v.push_back("mem[" + to_string(mem_idx) + "] = POP");
            }
            else{
                v.push_back("mem[" + to_string(targ.last_deleted) + "] = POP");
            }
        }
        else if(root->left->type == "RET"){
            helper_generate_targ_commands(root->right,v,targ);
            v.push_back("RET = POP");
        }
    }
    return v;
}

void EPPCompiler::write_to_file(vector<string> commands){
    ofstream out(output_file, ios::app);
    if(out.is_open()){
        for(const string& s : commands){
            out << s <<endl;
        }
        out.close();
    }
}

EPPCompiler::~EPPCompiler(){
    
    delete targ.symtable;
}
