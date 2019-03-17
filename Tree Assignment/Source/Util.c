#include "Util.h"

/*NECESSARY INFO ABOUT FUNCTION DECLARATONS IS PROVIDED IN HEADER FILE*/

void add_node(Node* parent, Node new_child){
    if(parent->child_count == 0){
        parent->child = (Node*) malloc(sizeof(Node));
    }
    else{
        parent->child = (Node*) realloc(parent->child, (parent->child_count+1) * sizeof(Node));
    }
    parent->child[parent->child_count].data = new_child.data;
    parent->child[parent->child_count].child_count = 0;
    parent->child[parent->child_count].password = NULL;
    parent->child[parent->child_count].child = NULL;
    parent->child_count +=1;
}
void add_word(Node* n, char* word, char* password){
    int cnt;
    Node* current = n;
    for(cnt = 0; cnt < strlen(word); cnt++) {
        Node newnode;
        newnode.child_count = 0;
        newnode.data = word[cnt];
        newnode.child = NULL;
        newnode.password = NULL;
        if (search(current, word[cnt]) == -1) { /*If node with given data is not present at current node*/
            add_node(current, newnode);
            current = &(current->child[current->child_count - 1]);
        } else {                                 /*If node is present, skip and make next node current*/
            current = &(current->child[search(current, word[cnt])]);
        }
        if (cnt == strlen(word) - 1) {
            assign_password(current, password);
        }
    }
}

void assign_password(Node* n, char* p){
    n->password = (char*) malloc((strlen(p)+1) * sizeof(char));
    strcpy(n->password, p);
}

int search(Node* n, char key){
    int index = 0;
    for(index; index < n->child_count; index++){
        if(n->child[index].data == key){
            return index;
        }
    }
    return -1;
}

void append_char(char* dest, char src){
    dest[strlen(dest)] = src;
    dest[strlen(dest)+1] = '\0';

}

Node* extract_branch(Node* root, char* str){
    int cnt1 = 0;

    if((root->child_count >1 && root->data != '0')){
        append_char(str, root->data);
        return root;
    }
        append_char(str, root->data);

    for(cnt1; cnt1<root->child_count; cnt1++){
        extract_branch(&root->child[cnt1], str);
    }

}
void print_preorder(Node* root, char* str1, FILE* of){
    Node* temp = root;
    int c;
    if(temp->child_count==0){
        fprintf(of, "%c", temp->data);
        if(temp->password != NULL){
            fprintf(of, "\n");
        }
        return;
    }
    for(c = 0; c<temp->child_count; c++){
        fprintf(of, "%c",temp->data);
        append_char(str1, temp->data);
        if(temp->password != NULL){
            fprintf(of, ",%s", str1);
        }
        print_preorder(&temp->child[c],str1, of
        );
    }

}
char* search_tree(Node* root, char* key){
    int cnt = 0;
    int ind = 0;
    Node* temp = root;
    for(cnt; cnt<strlen(key); cnt++){
        ind = search(temp, key[cnt]);
        if(ind == -1){
            return "!";
        }
        else{
            temp = &(temp->child[ind]);
        }
    }
    if(temp->password != NULL){
        return temp->password;
    } else{
        return "!!";
    }
}

void remove_node(Node* parent, char deleted){
    int ind = search(parent, deleted);

    for(ind; ind<parent->child_count-1; ind++){
        if(parent->child[ind+1].child_count > 0){
            (parent->child[ind].child) = (parent->child[ind+1].child);
        }
        else{
            parent->child[ind].child = NULL;
        }
        parent->child[ind].child_count = parent->child[ind+1].child_count;
        parent->child[ind].data = parent->child[ind+1].data;
        if(parent->child[ind+1].password != NULL){
            assign_password(&(parent->child[ind]), parent->child[ind+1].password);
        }
        else{
            free(parent->child[ind].password);
            parent->child[ind].password = NULL;
        }

    }
    if(parent->child_count >1){
        parent->child =(Node*) realloc(parent->child, (parent->child_count-1) * sizeof(Node));

    }
    parent->child_count--;
}

void delete(Node* root, char* username){
    Node** nodes = (Node**) malloc(sizeof(Node*)*strlen(username));
    int cnt = 0;
    Node* temp;
    temp = root;
    for(cnt; cnt<strlen(username); cnt++){
        temp = &temp->child[search(temp, username[cnt])];
        nodes[cnt] = temp;
    }
    cnt--;
    if(nodes[cnt]->child_count>1){
        free(nodes[cnt]->password);
        nodes[cnt]->password = NULL;
        return;
    }
    else{
        free(temp->password);
        temp->password = NULL;
        if(nodes[cnt-1]->child_count < 2 ){
            nodes[cnt-1]->child = NULL;
            nodes[cnt-1]->child_count = 0;
        }
        else{
            remove_node(nodes[cnt-1], nodes[cnt]->data);
        }
        temp = nodes[--cnt];
        while(temp->password == NULL && temp->child_count<2 && cnt>0){
            free(temp->password);
            temp->password = NULL;
            if(nodes[cnt-1]->child_count < 2){
                nodes[cnt-1]->child = NULL;
                nodes[cnt-1]->child_count = 0;
            }
            temp = nodes[--cnt];
        }
        if(temp->password != NULL){
            if(temp->child_count>1){
                remove_node(temp, nodes[cnt+1]->data);
            }
            else{
                temp->child_count--;
            }
        }
        else{
            if(temp->child_count>1){
                remove_node(temp, nodes[cnt+1]->data);
            }
            else{
                temp->child_count = 0;
                temp->child = NULL;
            }
        }
        if(cnt == 0){
            remove_node(root, temp->data);
        }
    }
    free(nodes);
}

void free_all(Node* root){
    if(root->child_count == 0){
        return;
    }
    else{
        int a;
        for(a = 0; a<root->child_count;a++){
            free_all(&root->child[a]);
            if(root->child[a].password != NULL){
                free(root->child[a].password);
            }
        }
        if(root->child_count > 0){
            free(root->child);
        }
    }
}
