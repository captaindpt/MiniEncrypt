#include<stdio.h>
#include<string.h>

int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}

void encrypt(char *string) {
    FILE *ptr;
    ptr = fopen(string, "r");
    char *cyphername = strtok(string, ".");
    FILE *cypher = fopen(strcat(cyphername, ".crp"), "w");
    while(1) {
        int c = fgetc(ptr);
        if(feof(ptr)) {
            break;
        }
        if(c == 9) {
            fprintf(cypher, "TT");
        } else if(c == 10) {
            fputs("\n", cypher);
        } else {
            int outChar = c - 16;
            if (outChar < 32)
                outChar = outChar - 32 + 144;
            fprintf(cypher, "%x", outChar);
        }

    }
    fclose(ptr);
    fclose(cypher);
}

void decrypt(char *string) {
    FILE *cypher = fopen(string, "r");
    char *filename = strtok(string, ".");
    FILE *text = fopen(strcat(filename, ".txt"), "w");
    while(1) {
        // int position = ftell(cypher);
        int first = getc(cypher);
        if(feof(cypher)) 
            break;
        if (first == 10) {
            fputc(10, text);
            continue;
        }
        char seq[3] = {first, getc(cypher), 0};
        if(feof(cypher)) 
            break;
        if (strcmp(seq, "TT") == 0) {
            fputc(9, text);
        } else {
            int outChar = hex2int(seq[0])*16 + hex2int(seq[1]);
            outChar += 16;
            if (outChar > 127) {
                outChar = outChar - 144 + 32;
            }
            fprintf(text, "%c", outChar);
        }
        
    }
    fclose(cypher);
    fclose(text);
}


int main(int argc, char *argv[]){
    int i = 0;
    for(i=1; i <argc; i++){
        if (strcmp(argv[i], "-D")==0){
            decrypt(argv[2]);
        }
         if (strcmp(argv[i], "-E")==0){
            encrypt(argv[2]);
            
        }
    }
    return 0;
}
