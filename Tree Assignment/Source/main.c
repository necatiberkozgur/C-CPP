#include "Util.h"

int main(int argc,char* argv[]) {
    FILE* input;			/*declaring and opening files to process*/
    FILE* output;
    input = fopen(argv[1], "r");
    output = fopen("output.txt", "w");
    
    int k,p;				/*counter variables to use later in loops*/
    
    char line[255];			/*String to store lines in input file*/
    char username[100];			/*Strings to store usernames and passwords*/
    char password[100];
    char str[1200];			/*String to store branches and use in list command*/
    char origstr[1200];
    
    Node root;
    Node* temp = (Node*) malloc(sizeof(Node));
    root.child_count = 0;
    root.data = '0';
    root.child = NULL;
    root.password = NULL;
    
    while(fscanf(input, "%s", line) != EOF) {
    
    /*PROCESSING EACH LINE IN INPUT FILE*/
    
    if(strcmp(line, "-a") == 0) {
        fscanf(input, "%s", line);
        strcpy(username, line);
        fscanf(input, "%s", line);
        strcpy(password, line);
        if(strcmp(search_tree(&root, username), "!") == 0 || strcmp(search_tree(&root, username), "!!") ==0){
            add_word(&root, username, password);
            fprintf(output, "\"%s\" was added\n", username);
        }
        else{
            fprintf(output, "\"%s\" reserved username\n", username);
        }
    }
    else if(strcmp(line, "-s") == 0){
        fscanf(input, "%s", line);
        strcpy(username, line);
        if(search(&root, username[0]) == -1){
            fprintf(output, "\"%s\" no record\n", username);
        }
        else if(strcmp(search_tree(&root, username), "!") == 0){
            fprintf(output, "\"%s\" incorrect username\n", username);
        }
        else if(strcmp(search_tree(&root, username), "!!") == 0){
            fprintf(output, "\"%s\" not enough username\n", username);
        }
        else{
            fprintf(output, "\"%s\" password \"%s\"\n", username, search_tree(&root, username));
        }

    }
    else if(strcmp(line, "-q") == 0){
        fscanf(input, "%s",line);
        strcpy(username, line);
        fscanf(input, "%s", line);
        strcpy(password, line);
        if(search(&root, username[0]) == -1){
            fprintf(output, "\"%s\" no record\n", username);
        }
        else if(strcmp(search_tree(&root, username), "!") == 0){
            fprintf(output, "\"%s\" incorrect username\n", username);
        }
        else if(strcmp(search_tree(&root, username), "!!") == 0){
            fprintf(output, "\"%s\" not enough username\n", username);
        }
        else{
            if(strcmp(password, search_tree(&root, username)) == 0 ){
                fprintf(output, "\"%s\" successful login\n", username);
            }
            else{
                fprintf(output, "\"%s\" incorrect password\n", username);
            }
        }
    }
    else if(strcmp(line, "-d") == 0){
        fscanf(input, "%s", line);
        strcpy(username, line);
        if(search(&root, username[0]) == -1){
            fprintf(output, "\"%s\" no record\n", username);
        }
        else if(strcmp(search_tree(&root, username), "!") == 0){
            fprintf(output, "\"%s\" incorrect username\n", username);
        }
        else if(strcmp(search_tree(&root, username), "!!") == 0){
            fprintf(output, "\"%s\" not enough username\n", username);
        }
        else{
            delete(&root, username);
            fprintf(output, "\"%s\" deletion is successful\n", username);
        }
    }
    else if(strcmp(line, "-l") == 0){
        for(k = 0; k<root.child_count; k++){
            temp=&root.child[k];
            memset(str, 0, strlen(str));
            memset(origstr, 0, strlen(origstr));
            extract_branch(&root.child[k], str);
            strcpy(origstr,str);
            fprintf(output, "-%s\n", str);
            while(temp->child_count<2){
                if(temp->child_count ==0){
                    break;
                }
                temp = &temp->child[0];
            }
            if(temp->child_count!=0){
                for(p = 0; p<temp->child_count; p++){
                    fprintf(output, "\t-%s",origstr);
                    print_preorder(&temp->child[p],str,output);
                }
            }
        }
    }
    }
    
    /*FREEING POINTERS AND FILES*/
    free_all(&root);
    fclose(input);
    fclose(output);
    return 0;

}
