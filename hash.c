


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Books{
    int bookId;
    char  authorName[30];
    char  bookName[30];
};
struct Index{
    int id;
};

const int NUMBER_OF_BOOKS_PER_SHELF = 5;

FILE * indexPtr;
FILE * booksPtr;

int hashing(int bookId){
    return bookId%30+1;
}
void createIndexFile(){
    indexPtr = fopen("index.dat","wb");
    //Index dosyasını oluşturduk her id ye -1 atadık.
    struct Index * create = malloc(sizeof(struct Index));
    create->id=-1;
    for(int i=0; i<150; i++)
        fwrite(create, sizeof(struct Index),1,indexPtr);
    free(create);
    fclose(indexPtr);
}
void createBooksFile(){
    booksPtr = fopen("books.dat","wb");
    struct Books * create = malloc(sizeof(struct Books));
    strcpy(create->authorName,"none");
    strcpy(create->bookName,"none");
    create->bookId=-1;
    for(int i=0; i<150; i++)
        fwrite(create, sizeof(struct Books),1,booksPtr);
    free(create);
    fclose(booksPtr);
}
void searchWithId(){
    int bookId;
    printf("Enter book id:");
    scanf("%d",&bookId);
    indexPtr = fopen("index.dat","rb");
    int shelf_for_book = hashing(bookId);
    fseek(indexPtr, (shelf_for_book-1)*5*sizeof(struct Index),SEEK_SET);
    int searchCount =0, control=0;
    struct Index tempIndex;
    // Id le index dosyasında kitabı arıyoruz.
    for(int i=0; i<=NUMBER_OF_BOOKS_PER_SHELF; i++){ // Kitap id si indexte yoksa dosyayı kapatıp çıkıyoruz
        if(control==NUMBER_OF_BOOKS_PER_SHELF){
            printf("Search count for book : %d\n",searchCount);
            printf("There is no a book with your input id.\n");
            fclose(indexPtr);
            return;
        }
        fread(&tempIndex, sizeof(struct Index),1,indexPtr);
        searchCount++;
        if(tempIndex.id == bookId){
            booksPtr = fopen("books.dat","rb");
            fseek(booksPtr,(shelf_for_book-1)*5*sizeof(struct Books),SEEK_SET);
            struct Books bookTemp;
            int tombstones =0;
            // Books dosyasında kitabı aramaya geçtik.
            for(int j=0; j<NUMBER_OF_BOOKS_PER_SHELF; j++){
                fread(&bookTemp,sizeof(struct Books),1,booksPtr);
                searchCount++;
                if(strcmp(bookTemp.bookName,"///")==0 && strcmp(bookTemp.authorName,"///")==0)
                    tombstones++;
                if(bookTemp.bookId==bookId){
                    printf("------------------------------\n");
                    printf("Book id : %d\nBook Name : %s\nAuthor Name : %s\n",bookId,bookTemp.bookName,bookTemp.authorName);
                    printf("Search count for book : %d\n",searchCount);
                    printf("Trespassed tombstones count : %d\n",tombstones);
                    printf("------------------------------\n");
                    fclose(indexPtr);
                    fclose(booksPtr);
                    return;
                }
            }
        }
        control++;
    }
}
void searchWithBookName(){
    char bookName[50];
    printf("Enter book name:");
    scanf("%s",bookName);

    int searchCount=0, tombstones=0;
    booksPtr = fopen("books.dat","rb");
    struct Books tempBook;
    for(int i=0; i<=150; i++){
        if(searchCount==150){ // Kitap dosyada yoksa dosyayı kapatıp çıkıyoruz.
            printf("Search count for book : %d\n",searchCount);
            printf("There is no a book with your input book name.\n");
            fclose(booksPtr);
            return;
        }
        fread(&tempBook, sizeof(struct Books),1,booksPtr);
        searchCount++;
        if(strcmp(tempBook.bookName,"///")==0 && strcmp(tempBook.authorName,"///")==0)
            tombstones++;
        if(strcmp(tempBook.bookName,bookName)==0){
            printf("------------------------------\n");
            printf("Book id : %d\nBook Name : %s\nAuthor Name : %s\n",tempBook.bookId,tempBook.bookName,tempBook.authorName);
            printf("Search count for book : %d\n",searchCount);
            printf("Trespassed tombstones count : %d\n",tombstones);
            printf("------------------------------\n");
            fclose(booksPtr);
            return;
        }
    }
}
void searchWithAuthorName(){
    char authorName[50];
    printf("Enter author name:");
    scanf("%s",authorName);

    int searchCount=0, tombstones=0, control=0;
    booksPtr = fopen("books.dat","rb");
    struct Books tempBook;
    // Book dosyasında arama yapıyoruz girilen author name ile.
    printf("------------------------------\n");

    for(int i=0; i<150; i++){
        fread(&tempBook, sizeof(struct Books),1,booksPtr);
        searchCount++;
        if(strcmp(tempBook.authorName,authorName)==0) {
            printf("Book id : %d\nBook Name : %s\nAuthor Name : %s\n-----\n", tempBook.bookId, tempBook.bookName,
                   tempBook.authorName);
            continue;
        }
        if(strcmp(tempBook.authorName,"///")==0){
            tombstones++; control++;
        }
        if(strcmp(tempBook.authorName,"none")==0)
            control++;



    }
    printf("Search count for book : %d\n",searchCount);
    if(control==150){ //Kitap dosyada yoksa dosyayı kapatıp çıkıyoruz.
        printf("There is no a book with your input author name.\n");
        fclose(booksPtr);
        return;
    }

    printf("Trespassed tombstones count : %d\n",tombstones);
    printf("------------------------------\n");
    fclose(booksPtr);


}
void deleteWithId() {
    int bookId;
    printf("Enter book id:");
    scanf("%d", &bookId);
    indexPtr = fopen("index.dat", "rb+");
    int shelf_for_book = hashing(bookId);
    fseek(indexPtr, (shelf_for_book - 1) * 5 * sizeof(struct Index), SEEK_SET);
    int control = 0;
    struct Index tempIndex;
    // Id le index dosyasında kitabı arıyoruz.
    for (int i = 0; i <= NUMBER_OF_BOOKS_PER_SHELF; i++) {
        fread(&tempIndex, sizeof(struct Index), 1, indexPtr);
        control++;
        if (control == NUMBER_OF_BOOKS_PER_SHELF) { // Kitap id si yoksa dosyayı kapatıp çıkıyoruz.
            printf("There is no a book with your input id.\n");
            fclose(indexPtr);
            return;
        }
        if (tempIndex.id == bookId) {
            // Index dosyasında id yi siliyoruz.
            struct Index * tempI= &tempIndex;
            tempI->id=-1;
            int position = ftell(indexPtr)- sizeof(struct Index);
            fseek(indexPtr,position,SEEK_SET);
            fwrite(&tempIndex, sizeof(struct Index),1,indexPtr);
            fclose(indexPtr);


            booksPtr = fopen("books.dat", "rb+");
            fseek(booksPtr, (shelf_for_book - 1) * 5 * sizeof(struct Books), SEEK_SET);
            struct Books bookTemp;
            // Books dosyasında kitabı aramaya geçtik.
            for (int j = 0; j < NUMBER_OF_BOOKS_PER_SHELF; j++) {
                fread(&bookTemp, sizeof(struct Books), 1, booksPtr);
                if (bookTemp.bookId == bookId) {
                    struct Books *temp = &bookTemp;
                    temp->bookId = -1;
                    strcpy(temp->bookName, "///");
                    strcpy(temp->authorName, "///");
                    int positionB = ftell(booksPtr) - sizeof(bookTemp);
                    fseek(booksPtr, positionB, SEEK_SET);
                    fwrite(&bookTemp, sizeof(struct Books), 1, booksPtr);
                    fclose(booksPtr);
                    return;
                }
            }
        }

    }
}
void deleteWithBookName(){
    char bookName[50];
    printf("Enter book name:");
    scanf("%s",bookName);

    booksPtr = fopen("books.dat","rb+");
    struct Books tempBook;

    for(int i=0; i<=150; i++){
        if(i==150){ // Kitap dosyada yoksa kapatıp çıkıyoruz.
            printf("Search count for book : %d\n",i);
            printf("There is no a book with your input book name.\n");
            fclose(booksPtr);
            return;
        }
        fread(&tempBook, sizeof(struct Books),1,booksPtr);
        if(strcmp(tempBook.bookName,bookName)==0){
            indexPtr = fopen("index.dat","rb+");
            int shelf_for_book = hashing(tempBook.bookId);
            fseek(indexPtr,(shelf_for_book-1)*5* sizeof(struct Index),SEEK_SET);
            struct Index tempIndex;
            for(int j=0; j<NUMBER_OF_BOOKS_PER_SHELF; j++){
                fread(&tempIndex, sizeof(struct Index),1,indexPtr);
                if(tempIndex.id==tempBook.bookId){
                    struct Index *tempI = &tempIndex;
                    tempI->id=-1;
                    int position = ftell(indexPtr)- sizeof(struct Index);
                    fseek(indexPtr,position,SEEK_SET);
                    fwrite(&tempIndex, sizeof(tempIndex),1,indexPtr);
                    fclose(indexPtr);

                    struct Books *temp = &tempBook;
                    temp->bookId=-1;
                    strcpy(temp->bookName,"///");
                    strcpy(temp->authorName,"///");
                    int positionB = ftell(booksPtr)- sizeof(tempBook);
                    fseek(booksPtr, positionB,SEEK_SET);
                    fwrite(&tempBook, sizeof(struct Books),1,booksPtr);
                    fclose(booksPtr);
                    return;
                }
            }
        }
    }
}
void deleteWithAuthorName(){
    char authorName[50];
    printf("Enter author name:");
    scanf("%s",authorName);


    booksPtr = fopen("books.dat","rb+");
    struct Books tempBook;

    for(int i=0; i<=150; i++){
        if(i==150){ // Kitap dosyada yoksa dosyayı kapatıp çıkıyoruz.
            printf("Search count for book : %d\n",i);
            printf("There is no a book with your input book name.\n");
            fclose(booksPtr);
            return;
        }
        fread(&tempBook, sizeof(struct Books),1,booksPtr);
        if(strcmp(tempBook.authorName,authorName)==0){
            // İlk önce index dosyasını güncelliyoruz.
            // Kitap bulunduğu için kesinlikle index dosyasında id si var.
            indexPtr = fopen("index.dat","rb+");
            int shelf_for_book = hashing(tempBook.bookId);
            fseek(indexPtr,(shelf_for_book-1)*5* sizeof(struct Index),SEEK_SET);
            struct Index tempIndex;
            for(int j=0; j<NUMBER_OF_BOOKS_PER_SHELF; j++){
                fread(&tempIndex, sizeof(struct Index),1,indexPtr);
                if(tempIndex.id==tempBook.bookId){
                    struct Index *tempI = &tempIndex;
                    tempI->id=-1;
                    int position = ftell(indexPtr)- sizeof(struct Index);
                    fseek(indexPtr,position,SEEK_SET);
                    fwrite(&tempIndex, sizeof(tempIndex),1,indexPtr);
                    fclose(indexPtr);

                    // İndex dosyasını kapattık.
                    struct Books *temp = &tempBook;
                    temp->bookId=-1;
                    strcpy(temp->bookName,"///");
                    strcpy(temp->authorName,"///");
                    int positionB = ftell(booksPtr)- sizeof(tempBook);
                    fseek(booksPtr, positionB,SEEK_SET);
                    fwrite(&tempBook, sizeof(struct Books),1,booksPtr);
                    fclose(booksPtr);

                    return;
                }
            }
        }
    }
}
void insertBook(){
    int bookId;
    char bookName[50];
    char authorName[50];
    printf("Enter a book id:");
    scanf("%d",&bookId);
    printf("Enter a book name:");
    scanf("%s",bookName);
    printf("Enter a author name:");
    scanf("%s",authorName);


    int shelf_for_book = hashing(bookId);

    // Raf numarasına göre index dosyasında nereye geleceğini bulup yerleştiriyoruz.
    indexPtr = fopen("index.dat","rb+");
    fseek(indexPtr,(shelf_for_book-1)*5*sizeof(struct Index),SEEK_SET);

    int control=-1;
    struct Index tempIndex;
    for(int i=0; i<=NUMBER_OF_BOOKS_PER_SHELF; i++){
        fread(&tempIndex, sizeof(struct Index),1,indexPtr);
        control++;
        if(control==NUMBER_OF_BOOKS_PER_SHELF){
            printf("------------------------------\n");
            printf("There is no place in shelf!\n");
            printf("------------------------------\n");
            fclose(indexPtr);
            return;
        }
        if(tempIndex.id==-1){
            struct Index *temp = &tempIndex;
            temp->id = bookId;
            int position = ftell(indexPtr)- sizeof(tempIndex);
            fseek(indexPtr, position,SEEK_SET);
            fwrite(&tempIndex, sizeof(struct Index),1,indexPtr);
            fclose(indexPtr);
            // Kitabı  indexe göre dosyaya yerleştiriyoruz.
            booksPtr = fopen("books.dat","rb+");
            // Dosayada nereye geleceğini bulduk.
            fseek(booksPtr,(shelf_for_book-1)*5*sizeof(struct Books),SEEK_SET);
            struct Books tempBook;
            for(int j=0; j<NUMBER_OF_BOOKS_PER_SHELF; j++){
                fread(&tempBook, sizeof(struct Books),1,booksPtr);
                if(strcmp(tempBook.bookName,"none")==0 && strcmp(tempBook.authorName,"none")==0){
                    struct Books * tempB = &tempBook;
                    tempB->bookId = bookId;
                    strcpy(tempB->bookName,bookName);
                    strcpy(tempB->authorName,authorName);
                    int positionB = ftell(booksPtr)- sizeof(tempBook);
                    fseek(booksPtr, positionB,SEEK_SET);
                    fwrite(&tempBook, sizeof(tempBook),1,booksPtr);
                    fclose(booksPtr);
                    return;
                }
                if(strcmp(tempBook.bookName,"///")==0 && strcmp(tempBook.authorName,"///")==0){
                    printf("------------------------------\n");
                    printf("This slot was a tombstone!\n");
                    printf("------------------------------\n");
                    struct Books *tempB = &tempBook;
                    tempB->bookId = bookId;
                    strcpy(tempB->authorName,authorName);
                    strcpy(tempB->bookName,bookName);
                    int positionB = ftell(booksPtr)- sizeof(tempBook);
                    fseek(booksPtr, positionB,SEEK_SET);
                    fwrite(&tempBook,sizeof(tempBook),1,booksPtr);
                    fclose(booksPtr);
                    return;
                }
            }

        }

    }
}
void searchBook(){
    int selection;
    printf("1-> Search With Book Id\n");
    printf("2-> Search With Book Name\n");
    printf("3-> Search With Author Name\n");
    printf("4-> Back to Main Menu\n");
    scanf("%d",&selection);
    switch(selection){
        case 1:
            searchWithId();
            break;
        case 2:
            searchWithBookName();
            break;
        case 3:
            searchWithAuthorName();
            break;
        case 4:
            return;
    }
}
void deleteBook(){
    int selection;
    printf("1-> Delete With Book Id\n");
    printf("2-> Delete With Book Name\n");
    printf("3-> Delete With Author Name\n");
    printf("4-> Back to Main Menu\n");
    scanf("%d",&selection);
    switch(selection){
        case 1:
            deleteWithId();
            break;
        case 2:
            deleteWithBookName();
            break;
        case 3:
            deleteWithAuthorName();
            break;
        case 4:
            return;
    }
}
void listAllBook(){
    printf("%-15s%-15s%-15s%-15s%-15s%-15s\n","Shelf Number","B1","B2","B3","B4","B5");
    for(int i=0; i<90; i++)
        printf("-");
    printf("\n");
    booksPtr = fopen("books.dat","rb");
    struct Books temp;
    for(int shelf_number=1; shelf_number<=30; shelf_number++){
        printf("%-15d",shelf_number);
        for(int j=0; j<NUMBER_OF_BOOKS_PER_SHELF; j++) {
            fread(&temp, sizeof(struct Books), 1, booksPtr);
            if(strcmp(temp.bookName,"none")==0)
                printf("%-15s","NULL");
            else if(strcmp(temp.bookName,"///")==0)
                printf("%-15s","///");
            else
                printf("%-15s",temp.bookName);
        }
        printf("\n");

    }
    for(int i=0; i<90; i++)
        printf("-");
    printf("\n");
    fclose(booksPtr);
}



int main() {
    indexPtr = fopen("index.dat", "rb");
    booksPtr = fopen("books.dat", "rb");

    if (!indexPtr) {
        createIndexFile();
        // Index dosyasını oluşturduk her id ye -1 atadık.
        /*struct Index * temp = malloc(sizeof(struct Index));
        indexPtr=fopen("index.dat","rb");
        for(int i=0; i<150; i++){
            fread(temp, sizeof(struct Index),1,indexPtr);
            printf("%d\n",temp->id);
        }
        fclose(indexPtr);*/
    }
    if (!booksPtr) {
        createBooksFile();
        // Books dosyasını oluşturduk her bir değişkene "none" atadık.
        /*struct Books * temp = malloc(sizeof(struct Books));
        booksPtr = fopen("books.dat","rb");
        for(int i=0; i<150; i++) {
            fread(temp, sizeof(struct Books), 1, booksPtr);
            printf("%s %s\n", temp->bookName, temp->authorName);
        }
        fclose(booksPtr);*/
    }
    fclose(booksPtr);
    fclose(indexPtr);

    while(true){

        printf("1-> Insert A Book\n");
        printf("2-> Search A Book\n");
        printf("3-> Delete A Book\n");
        printf("4-> List All Book\n");
        printf("5-> Exit The Program\n");

        int selection;
        char * a= malloc(sizeof(char *));
        scanf("%d",&selection);
        switch (selection){
            case 1:
                insertBook();
                printf("Return the main menu you must press c or C key!\n");
                scanf("%s",a);

                break;
            case 2:
                searchBook();
                printf("Return the main menu you must press c or C key!\n");
                scanf("%s",a);
                break;
            case 3:
                deleteBook();
                break;
            case 4:
                listAllBook();
                printf("Return the main menu you must press c or C key!\n");
                scanf("%s",a);
                break;
            case 5:
                exit(EXIT_SUCCESS);
            default:
                printf("There is no selection with your input!\n");
                break;
        }
    }


}
