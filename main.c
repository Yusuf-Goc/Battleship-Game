#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EASY 10
#define MEDIUM 15
#define HARD 20
#define EMPTY '.'   //sabitler
#define SHIP 'F'
#define MISS 'O'
#define HIT 'X'

// fonksiyonlarin bildirimleri
void showMenu();
void showBoard(char board[][HARD], int size);
void placeShips(char board[][HARD], int size);
void playGame(char board1[][HARD], char board2[][HARD], int size);
int shoot(char board[][HARD], int size, int row, int col);
int isGameOver(char board[][HARD], int size);

int main()
{
    //rastgele sayi uretmek icin
    srand(time(NULL));

    int option, size;
    char board1[HARD][HARD], board2[HARD][HARD];
    int i, j;

    showMenu();

    printf("Please enter the difficulty level:  ");
    scanf("%d", &option);

    switch(option)
    {
        case 1:
            size = EASY;
            break;
        case 2:
            size = MEDIUM;
            break;
        case 3:
            size = HARD;
            break;
        default:
            printf("Please enter a number between 1-3: \n");
            return 0;
    }

    //tahtalari bos karelerle doldur
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            board1[i][j] = EMPTY;
            board2[i][j] = EMPTY;
        }
    }

    //iki oyuncunun da gemileri yerlestirilir
    placeShips(board1, size);
    placeShips(board2, size);

    //oyun baslar
    playGame(board1, board2, size);

    printf("Player 1's board:\n");
    showBoard(board1, size);
    printf("Player 2's board:\n");
    showBoard(board2, size);

    return 0;
}

//menuyu gosterir
void showMenu()
{
    printf("     WELCOME TO BATTLESHIP GAME!!!\n\n");
    printf("\t1. Easy difficulty: (%dx%d)\n\n", EASY, EASY);
    printf("\t2. Medium difficulty: (%dx%d)\n\n", MEDIUM, MEDIUM);
    printf("\t3. Hard difficulty: (%dx%d)\n\n", HARD, HARD);
}

//Oyun tahtasini gosteren fonksiyon
void showBoard(char board[][HARD], int size)
{
    int i, j;

    //sutun baslari
    printf("  ");
    for(i = 0; i < size; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    //satir basliklari
    for(i = 0; i < size; i++)
    {
        printf("%c ", 'A' + i);
        for(j = 0; j < size; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

//Gemileri rastgele yerlestirme
void placeShips(char board[][HARD], int size)
{
    int shipSizes[5] = {0, 2, 3, 4, 5}, shipCounts[5] = {0, 4, 3, 2, 1};
    int shipType, shipSize, shipCount, direction, row, col;
    int i, j, valid;


    //gemi turleri sirayla döngüye alinir
    for(shipType = 1; shipType <= 4; shipType++)
    {
        shipSize = shipSizes[shipType];
        shipCount = shipCounts[shipType];

        //tum gemiler yerlestirilene kadar dongu devam eder
        for(i = 0; i < shipCount; i++)
        {
            do
            {
                //rastgele yon secme
                direction = rand() % 2;

                //gemi yonune gore baslangýc noktasý secme
                if(direction == 0) //yatay
                {
                    row = rand() % size;
                    col = rand() % (size - shipSize + 1);
                }
                else //dikey
                {
                    row = rand() % (size - shipSize + 1);
                    col = rand() % size;
                }

                valid = 1;

                //gemi yerlestirme kontrol edilir
                for(j = 0; j < shipSize; j++)
                {
                    //kare bos degilse gecersiz olur
                    if(board[row][col] != EMPTY)
                    {
                        valid = 0;
                    }

                    //gemi yonune gore yerlestirilir
                    if(direction == 0)
                    {
                        col++;
                    }
                    else
                    {
                        row++;
                    }
                }

            } while(valid == 0); //gecerli bir yerlestirme araniyor

            //gemi yonune gore baslangic noktasi tekrar aranir
            if(direction == 0)
            {
                col = col - shipSize;
            }
            else
            {
                row = row - shipSize;
            }

            //gemi yonune gore tahtaya yerlestirilir
            for(j = 0; j < shipSize; j++)
            {
                board[row][col] = SHIP;
                if(direction == 0)
                {
                    col++;
                }
                else
                {
                    row++;
                }
            }
        }
    }
}

//Oyunu oynatan fonksiyon
void playGame(char board1[][HARD], char board2[][HARD], int size)
{
    int turn, row, col, result, over;
    char letter;

    //her tahta için boþ bir dizi oluþtur
    char emptyBoard1[HARD][HARD];
    char emptyBoard2[HARD][HARD];

    //boþ dizileri EMPTY karakteri ile doldur
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            emptyBoard1[i][j] = EMPTY;
            emptyBoard2[i][j] = EMPTY;
        }
    }

    turn = rand() % 2;

    //oyun bitene kadar dongu devam eder
    do
    {
        //tahtayý göster
        if(turn == 0)     //1. oyuncu
        {
            printf("Player 2's board:\n");
            showBoard(emptyBoard2, size); //boþ tahtayý göster
        }
        else     //2. oyuncu
        {
            printf("Player 1's board:\n");
            showBoard(emptyBoard1, size); //boþ tahtayý göster
        }

        //hamle yap
        printf("WHICH LETTER DO YOU WANT TO HIT: ");
        scanf(" %c", &letter);
        printf("WHICH NUMBER DO YOU WANT TO HIT: ");
        scanf("%d", &col);

        //harfi buyuk haline ceviren fonksiyon
        letter = toupper(letter);

        row = letter - 'A';

        //siraya gore atis yapilarak sonuc alinir
        if(turn == 0) //1. oyuncu
        {
            result = shoot(board2, size, row, col);
            //boþ tahtayý güncelle
            if(result == 0) //iska
            {
                emptyBoard2[row][col] = MISS;
            }
            else if(result == 1) //vurma
            {
                emptyBoard2[row][col] = HIT;
            }
        }
        else //2. oyuncu
        {
            result = shoot(board1, size, row, col);
            //boþ tahtayý güncelle
            if(result == 0) //iska
            {
                emptyBoard1[row][col] = MISS;
            }
            else if(result == 1) //vurma
            {
                emptyBoard1[row][col] = HIT;
            }
        }

        //atistan sonra
        if(result == 0)
        {
            printf("MISS!\n");
        }
        else if(result == 1) //vurma
        {
            printf("HIT!\n");
        }
        else if(result == 2) //batirma
        {
            printf("SUNK!\n");
        }
        else
        {
            printf("INVALID SHOT!\n");
        }

        //oyunun bitip bitmedigini kontrol eder
        if(turn == 0)
        {
            over = isGameOver(board2, size);
        }
        else
        {
            over = isGameOver(board1, size);
        }

        if(over == 0)
        {
            turn = 1 - turn;
        }

    } while(over == 0);

    //bitis ekrani
    if(turn == 0)
    {
        printf("CONGRATULATIONS! PLAYER 1 WON!\n");
    }
    else
    {
        printf("CONGRATULATIONS! PLAYER 2 WON!\n");
    }
}


//atis sonucunu degerlendiren fonksiyon
int shoot(char board[][HARD], int size, int row, int col)
{
    int result;
    //girilen kare sinirlar icinde mi
    if(row < 0 || row >= size || col < 0 || col >= size)
    {
        result = -1;
    }
    else
    {
        //atis yapilan kareyi kontrol
        if(board[row][col] == EMPTY)//iska
        {
            result = 0;
            board[row][col] = MISS;
        }
        else if(board[row][col] == SHIP) //vurma
        {
            result = 1;
            board[row][col] = HIT;
        }
        else//gecersiz
        {
            result = -1;
        }
    }
    return result;
}


//oyun bitti mi
int isGameOver(char board[][HARD], int size)
{

    int i, j, over = 1;

    //tahtayi inceler
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            //gemi parcasi kalmissa
            if(board[i][j] == SHIP)
            {
                over = 0;
            }
        }
    }
    return over;
}
