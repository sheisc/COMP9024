#include <stdio.h>
#include <stdlib.h>
#include "Huffman.h"
#include "BiTree.h"


int main(int argc, char **argv, char **env) {
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    char *text = "Data Structures";        
    printf("Original text: %s\n", text);

    struct HuffmanInfo *pHuffmanInfo = CreateHuffmanInfoFromText(text);
    char *encodedText = HuffmanEncode(pHuffmanInfo, text);
    printf("Encoded: %s\n", encodedText);
    
    char *decodedText = HuffmanDecode(pHuffmanInfo, encodedText);
    printf("Decoded: %s\n", decodedText);

    free(encodedText);
    free(decodedText);
    ReleaseHuffmanInfo(pHuffmanInfo);
    return 0;
}



