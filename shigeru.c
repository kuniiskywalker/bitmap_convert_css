/*
 ============================================================================
 Name        : shigeru.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

struct fileinfo {
    int  width;
    int  height;
};

int getFileSize(FILE *fp);
void getFileInfo(FILE *fp, struct fileinfo *fi);

int main(int argc, char *argv[]) {
	
	unsigned char *filename = NULL;
	
	filename = argv[1];
	
	
	
    FILE *readFile;	 /*読み込みようファイル*/
    FILE *writeFile; /*書き込み用ファイル*/
    unsigned int fileSize = 0; /*ファイルサイズの格納用*/
    unsigned char *buffer = NULL; /*データの格納用*/
    unsigned char *tmpBuffer = 0; /*データの操作用*/
    int i,j;
    int paddingByte = 0; /*埋める分のバイト数を格納*/
    int r, g, b;
    struct fileinfo fi[1];

    /*バイナリ読み込みモードでオープン*/
    if ((readFile = fopen(filename, "rb")) == NULL) {
        printf("ERR:ReadFile\n");
        exit(1);
    }
    /*バイナリ書き込みモードでオープン*/
    /*
    if ((writeFile = fopen("output.bmp", "wb")) == NULL) {
        printf("ERR:WriteFile\n");
        fclose(readFile);
        exit(1);
    }
    */
    
    if ((writeFile = fopen("style.css", "w")) == NULL) {
        printf("ERR:WriteFile\n");
        fclose(readFile);
        exit(1);
    }
    
    
    /* ファイルサイズ取得 */
    fileSize = getFileSize(readFile);
    /* ファイル情報取得 */
    getFileInfo(readFile, fi);

//    printf("width=%d\n", fi->width);
//    printf("height=%d\n", fi->height);

    /*メモリ確保*/
    if ((buffer = (unsigned char *)malloc(sizeof(unsigned char) * fileSize)) == NULL) {
        printf("ERR:Memory\n");
        fclose(readFile);
        fclose(writeFile);
        exit(1);
    }
    fread(buffer, sizeof(unsigned char), fileSize, readFile);
	
	
	// 
	fprintf(writeFile,".draw {\n");
	fprintf(writeFile,"position: absolute;\n");
	fprintf(writeFile,"top: 30px;\n");
	fprintf(writeFile,"left: 50%;\n");
	fprintf(writeFile,"width: 0;\n");
	fprintf(writeFile,"height: 0;\n");
	fprintf(writeFile,"box-shadow:\n");
	
    paddingByte = (2 * 3) % 4;
    if (paddingByte != 0) {
        paddingByte = 4 - paddingByte;
    }
    
    int first_flag = '1';
    
    tmpBuffer = buffer + 54;
    for (i = fi->height; i > 0; i--) {
        for (j = 0; j < fi->width; j++) {
			
            b = *tmpBuffer;
            g = *(tmpBuffer + 1);
            r = *(tmpBuffer + 2);
			
			
			if (first_flag == '1') {
				first_flag = '0';
			} else {
				fprintf(writeFile,",");
			}
			
			
			fprintf(writeFile,"%dpx  %dpx 1px 2px #%02x%02x%02x", j, i, r, g, b);
			
			
			tmpBuffer += 3;
        }
        tmpBuffer += paddingByte; /*余分なデータを読み飛ばす*/
        // tmpBuffer += (3*fi->width);
    }
    
    fprintf(writeFile,";}\n");
    
    /* ファイルに書く */
    // fwrite(buffer, sizeof(unsigned char), fileSize, writeFile);
    fclose(writeFile);
    return EXIT_SUCCESS;
}

/*ファイルサイズ取得用関数*/
int getFileSize(FILE *fp){
    fpos_t tmpSeek = 0;
    fpos_t fileSize = 0;
    tmpSeek = ftell(fp);		/*位置保存*/
    fseek(fp, 0L, SEEK_SET);	/*先頭にセット*/
    fseek(fp, 0L, SEEK_END);	/*サイズ取得*/
    fgetpos(fp,&fileSize);
    fseek(fp, tmpSeek, SEEK_SET);		/*位置復元*/
    return (int)fileSize;
}

/*ファイル情報取得用関数*/
void getFileInfo(FILE *fp, struct fileinfo *fi){
    fpos_t tmpSeek = 0;
    unsigned char header[54];
    tmpSeek = ftell(fp);		/*位置保存*/
    fread(header,1,54,fp);
    fi->width = header[21] << 24 | header[20] << 16 | header[19] << 8 | header[18];
    fi->height = header[25] << 24 | header[24] << 16 | header[23] << 8 | header[22];
    fseek(fp, tmpSeek, SEEK_SET );
}
