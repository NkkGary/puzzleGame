//============================================================================
// Name        : puzzleGame.cpp
// Author      : nkkgary
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

//Dear professor, welcome to the code! Most* of the functions have stright forward names
//but if you are confused about what they do, please hover on the function name and it will show you.
/**
 * @brief Clear the console output screen (windows only.)
 *
 */
void clearConsole() {
    //clear the history so it wont be a wall of text
#ifdef _WIN32
    system("cls");
#elif __APPLE__
    system("clear");
#endif
}

/**
 * @brief wait for user to press enter to continue
 *
 */
void waitForEnter() {
#ifdef _WIN32
    system("pause");
#elif __APPLE__
    system("read"); //dont know if this works or not i dont have macos
#endif
}

class Timer {
public:
    int startTime; //i dont even want it to be private
    void start() {
        startTime = time(0); // current time in seconds
    }
    int stop() {
        int elapsed = time(0) - startTime;
        return elapsed;
    }
};

class PuzzlePiece {
public:
    PuzzlePiece(char n, int a, int b, int c, int d) {
        name = n;
        Side[0] = a;
        Side[1] = b;
        Side[2] = c;
        Side[3] = d;
        isEmpty = false;
    }
    PuzzlePiece() {
        name = ' ';
        Side[0] = -1;
        Side[1] = -1;
        Side[2] = -1;
        Side[3] = -1;
        isEmpty = true;
    }
    /**
     * @brief Set the side of the puzzle piece to number
     *
     * @param side {int} the side of the puzzle piece. 0, 1, 2, 3 for top, right, bottom, left
     * @param number {int} the number to set the side to
     */
    void setSide(int side, int number) {
        Side[side] = number;
    }
    /**
     * @brief Set the Name of the puzzle piece
     *
     * @param n {char} The name to set the puzzle piece to
     */
    void setName(char n) {
        name = n;
    }
    /**
     * @brief Set the isEmpty flag of the puzzle piece
     *
     * @param empty {bool} The value to set the isEmpty flag to. true if empty, false if not
     */
    void setEmptyness(bool empty) {
        isEmpty = empty;
    }
    /**
     * @brief Rotate the piece 90 degrees clockwise
     *
     */
    void RotateCCW() {
        int temp = Side[0];
        Side[0] = Side[1];
        Side[1] = Side[2];
        Side[2] = Side[3];
        Side[3] = temp;
    }
    /**
     * @brief Rotate the piece 90 degrees counter-clockwise
     *
     */
    void RotateCW() {
        int temp = Side[3];
        Side[3] = Side[2];
        Side[2] = Side[1];
        Side[1] = Side[0];
        Side[0] = temp;
    }
    /**
     * @brief Get the name of the puzzle piece
     *
     * @return {char} the name of the puzzle piece
     */
    char PrintName() {
        return name;
    }
    /**
     * @brief Get the top side of the puzzle piece
     *
     * @return {int} the top side of the puzzle piece
     */
    int PrintT() {
        return Side[0];
    }
    /**
     * @brief Get the right side of the puzzle piece
     *
     * @return {int} the right side of the puzzle piece
     */
    int PrintR() {
        return Side[1];
    }
    /**
     * @brief Get the bottom side of the puzzle piece
     *
     * @return {int} the bottom side of the puzzle piece
     */
    int PrintB() {
        return Side[2];
    }
    /**
     * @brief Get the left side of the puzzle piece
     *
     * @return {int} the left side of the puzzle piece
     */
    int PrintL() {
        return Side[3];
    }
    /**
     * @brief Get the emptyness of the puzzle piece
     *
     * @return {bool} true if the piece is empty, false if not
     */
    bool PrintEmptyness() {
        return isEmpty;
    }
private:
    char name;  // name of the piece
    bool isEmpty; // is the piece empty?
    int Side[4]; //Top0, Right1, Bottom2, Left3 / NESW
};

class GameBoard {
public:
    GameBoard(const int pPC, const int rNRL, const int rNRU, const int seed = time(NULL)) {
        for (int i = 0; i < 25; i++) {
            UsedChar[i] = { '\0' }; // initialize the array of used characters
        }
        puzzlePieceCount = pPC;
        randomNumberRangeLower = rNRL;
        randomNumberRangeUpper = rNRU;
        seedRNG = seed;
        //init offboard (should all be empty now)
        for (int i = 0; i < 25; i++) {
            OffBoard[i].setEmptyness(true);
        }
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                Board[i][j].setEmptyness(true);
            }
        }
    }
    /**
     * @brief Generates the PuzzlePieces and places them on the board.
     *
     */
    void GeneratePuzzlePieces() {
        //cout << "Generating puzzle pieces..." << endl;
        const int range = (randomNumberRangeUpper - randomNumberRangeLower);

        srand(seedRNG);
        //generate top left
        PuzzlePiece tempPuzzlePiece;
        tempPuzzlePiece.setName(GameBoard::generateRandomLetterNoRepeat());
        //do check on range to prevent div by 0 error
        if (range == 0) {
            tempPuzzlePiece.setSide(0, randomNumberRangeLower);
            tempPuzzlePiece.setSide(1, randomNumberRangeLower);
            tempPuzzlePiece.setSide(2, randomNumberRangeLower);
            tempPuzzlePiece.setSide(3, randomNumberRangeLower);
        }
        else {
            tempPuzzlePiece.setSide(0, (rand() % range + randomNumberRangeLower));
            tempPuzzlePiece.setSide(1, (rand() % range + randomNumberRangeLower));
            tempPuzzlePiece.setSide(2, (rand() % range + randomNumberRangeLower));
            tempPuzzlePiece.setSide(3, (rand() % range + randomNumberRangeLower));
        }
        tempPuzzlePiece.setEmptyness(false);
        Board[0][0] = tempPuzzlePiece;
        //

        //sweep left to right, top to bottom, and generate
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (i == 0 && j == 0) {
                    continue;
                }
                //generate Name, check if it has been used
                tempPuzzlePiece.setName(GameBoard::generateRandomLetterNoRepeat());
                //check for the top (and copy its' value), dont check if its out of range(i = -1)
                if (i >= 1) {
                    tempPuzzlePiece.setSide(0, Board[i - 1][j].PrintB());
                }
                else {
                    //short hand if makes the code harder to read, so below is explaination
                    //it makes the side always generate as the lower range if range is 0.
                    tempPuzzlePiece.setSide(0, (range == 0) ? randomNumberRangeLower : (rand() % range + randomNumberRangeLower));
                }
                //check for the left (and copy its' value), dont check if its out of range(j = -1)
                if (j >= 1) {
                    tempPuzzlePiece.setSide(3, Board[i][j - 1].PrintR());
                }
                else {
                    tempPuzzlePiece.setSide(3, (range == 0) ? randomNumberRangeLower : (rand() % range + randomNumberRangeLower));
                }
                tempPuzzlePiece.setSide(1, (range == 0) ? randomNumberRangeLower : (rand() % range + randomNumberRangeLower));
                tempPuzzlePiece.setSide(2, (range == 0) ? randomNumberRangeLower : (rand() % range + randomNumberRangeLower));
                Board[i][j] = tempPuzzlePiece;
            }
        }
    }
    /**
     * @brief Check The puzzle pieces to see if it fits into the board (sides should be matching)
     *
     * @param indexY {int} the y index of where the puzzle piece should be place to
     * @param indexX {int} the x index of where the puzzle piece should be place to
     * @param PuzzlePieceToPush {PuzzlePiece} the puzzle piece to be placed
     * @return true if the puzzle piece fits, false if it does not
     */
    bool checkSides(int targetCoordY, int targetCoordX, int pieceIndexX) {
        bool check = true;
        if (targetCoordY != 4) { //if not the at bottom (y=4) check the bottom of the piece
            if (Board[targetCoordY + 1][targetCoordX].PrintEmptyness() == false) {
                if (Board[targetCoordY + 1][targetCoordX].PrintT() != OffBoard[pieceIndexX].PrintB()) {
                    check = false;
                }
            }
        }
        if (targetCoordX != 4) { //if not the at right (x=4) check the right of the piece
            if (Board[targetCoordY][targetCoordX + 1].PrintEmptyness() == false) {
                if (Board[targetCoordY][targetCoordX + 1].PrintL() != OffBoard[pieceIndexX].PrintR()) {
                    check = false;
                }
            }
        }
        if (targetCoordY != 0) { //if not the at top (y=0) check the top of the piece
            if (Board[targetCoordY - 1][targetCoordX].PrintEmptyness() == false) {
                if (Board[targetCoordY - 1][targetCoordX].PrintB() != OffBoard[pieceIndexX].PrintT()) {
                    check = false;
                }
            }
        }
        if (targetCoordX != 0) { //if not the at left (x=0) check the left of the piece
            if (Board[targetCoordY][targetCoordX - 1].PrintEmptyness() == false) {
                if (Board[targetCoordY][targetCoordX - 1].PrintR() != OffBoard[pieceIndexX].PrintL()) {
                    check = false;
                }
            }
        }
        return check;
    }
    /**
     * @brief Put a puzzle piece from the offboard to the board
     *
     * @param indexY {int} the y index of where the puzzle piece should be place to
     * @param indexX {int} the x index of where the puzzle piece should be place to
     * @param PuzzlePieceToPush {PuzzlePiece} the puzzle piece to be placed
     *
     * @return {int} 0 if the puzzle piece placed successfully, 1 if it  is occupied, 2 if placing an empty piece, 3 if the sides dont match
     */
    int pushToBoard(int targetCoordY, int targetCoordX, int pieceIndexX) {
        //check if the piece we are placing is ok (the edges are correct)
        if (checkSides(targetCoordY, targetCoordX, pieceIndexX)) {
            //check if the piece we are placing is empty
            if (!OffBoard[pieceIndexX].PrintEmptyness()) {
                //check if the piece we are placing is already on the board
                if (Board[targetCoordY][targetCoordX].PrintEmptyness()) {
                    //place the piece on the board
                    Board[targetCoordY][targetCoordX] = OffBoard[pieceIndexX];
                    //remove the piece from the offboard
                    OffBoard[pieceIndexX].setEmptyness(true);
                }
                else {
                    cout << "The coordinate you are trying to place at is occupied." << endl;
                    return 1;
                }
            }
            else {
                cout << "The puzzle piece you are trying to place is empty. (This is an error. Should not happen!)" << endl;
                return 2;
            }
        }
        else {
            cout << "The sides of the puzzle does not match!" << endl;
            return 3;
        }
        return 0;
    }
    /**
     * @brief Remove a puzzle piece from the board to the offboard
     *
     * @param indexY {int} the y index of the puzzle piece to remove
     * @param indexX {int} the x index of the puzzle piece to remove
     */
    void pullToOffBoard(int indexY, int indexX) {
        for (int i = 0; i < 25; i++) { // will replace the first instance of OffBoard that is empty
            if (OffBoard[i].PrintEmptyness() == true) {
                OffBoard[i] = Board[indexY][indexX];
                //this should not fail right? in NO WAY we will have more than 25 piece that is in OffBoard at once else this code is a mess
                (Board[indexY][indexX]).setEmptyness(true);
                return;
            }
        }
        cout << "Error: pullToOffBoard() failed to find an empty space in OffBoard" << endl;
    }
    /**
     * @brief Displays the Board.
     *
     */
    void displayBoard() {

        cout << "\n****Main Board View****" << endl;
        cout << setw(5) << "A" << setw(3) << "B" << setw(3) << "C" << setw(3) << "D" << setw(4) << "E\n";
        for (int i = 0; i < 17; i++) {
            switch (i) {
            case 2:
                cout << "1 ";
                break;
            case 5:
                cout << "2 ";
                break;
            case 8:
                cout << "3 ";
                break;
            case 11:
                cout << "4 ";
                break;
            case 14:
                cout << "5 ";
                break;
            default:
                cout << std::string(2, ' ');
            }
            for (int j = 0; j < 17; j++) {
                //print the edges
                if ((i == 0 || i == 16) && (j == 0 || j == 16)) {
                    cout << '+';
                }
                else if (i == 0 || i == 16) {
                    cout << '-';
                }
                else if (j == 0 || j == 16) {
                    cout << '|';
                }
                // print puzzlepieces on board yes
                else if (i % 3 == 1) { //top row of a piece
                    if (j % 3 == 1) { //top left of a piece
                        cout << ' ';
                    }
                    if (j % 3 == 2) { //top middle of a piece
                        if (Board[i / 3][j / 3].PrintEmptyness() == false) {
                            cout << Board[i / 3][j / 3].PrintT();
                        }
                        else cout << ' ';
                    }
                    if (j % 3 == 0) { //top right of a piece
                        cout << ' ';
                    }
                }
                else if (i % 3 == 2) { //middle row of a piece
                    if (j % 3 == 1) { //middle left of a piece
                        if (Board[i / 3][j / 3].PrintEmptyness() == false) {
                            cout << Board[i / 3][j / 3].PrintL();
                        }
                        else cout << ' ';
                    }
                    if (j % 3 == 2) { //center of a piece
                        if (Board[i / 3][j / 3].PrintEmptyness() == false) {
                            cout << Board[i / 3][j / 3].PrintName();
                        }
                        else cout << ' ';
                    }
                    if (j % 3 == 0) { //middle right of a piece
                        if (Board[i / 3][(j / 3) - 1].PrintEmptyness() == false) {
                            cout << Board[i / 3][(j / 3) - 1].PrintR();
                        }
                        else cout << ' ';
                    }
                }
                else if (i % 3 == 0) { //bottom row of a piece
                    if (j % 3 == 1) { //bottom left of a piece
                        cout << ' ';
                    }
                    if (j % 3 == 2) { //bottom middle of a piece
                        if (Board[(i / 3) - 1][j / 3].PrintEmptyness() == false) {
                            cout << Board[(i / 3) - 1][j / 3].PrintB();
                        }
                        else cout << ' ';
                    }
                    if (j % 3 == 0) { //bottom right of a piece
                        cout << ' ';
                    }
                }
            }
            cout << endl;
        }
    }
    /**
     * @brief Displays the OffBoard.
     *
     * @param AltAppearance should alternate appearance be used which have paddings around
     */
    void displayOffBoard(bool AltAppearance) {
        cout << "\n****Off Board Pieces****" << endl;
        int x, y, PieceWH;
        if (AltAppearance) { // alternate apperance
            x = 7; //puzzle pieces in a row (max)
            y = 4; //puzzle pieces in a column (max)
            PieceWH = 4; //Width and Height of the piece, expected to be a square.
        }
        else { //normal
            x = 10;
            y = 3;
            PieceWH = 3;
        }
        int count = 0;
        for (int i = 0; i < y * PieceWH + 2; i++) {
            if (count >= puzzlePieceCount) { //dont print offboard[25] please
                if (i != y * PieceWH + 1) // if not at the last line, skip until last line
                    continue; // this is code for adaptive y size of offboard, it will expend the offboard size to fit the puzzle piece count.
            }
            for (int j = 0; j < x * PieceWH + 2; j++) {
                //print the edges
                if ((i == 0 || i == y * PieceWH + 1) && (j == 0 || j == x * PieceWH + 1)) {
                    cout << '+';
                }
                else if (i == 0 || i == y * PieceWH + 1) {
                    cout << '-';
                }
                else if (j == 0 || j == x * PieceWH + 1) {
                    cout << '|';
                }
                else if (count >= puzzlePieceCount) { //dont print offboard[25] please
                    cout << ' ';
                }
                // print puzzlepieces on board yes
                else if (AltAppearance) { //Alternate Appearence
                    if (i % 4 == 1) { //top row of a piece
                        if (j % 4 == 1) { //top left of a piece
                            cout << ' ';
                        }
                        if (j % 4 == 2) { //top middle of a piece
                            if (OffBoard[(i / 4) * x + (j / 4)].PrintEmptyness() == false) {
                                cout << OffBoard[(i / 4) * x + (j / 4)].PrintT();
                            }
                            else cout << ' ';
                        }
                        if (j % 4 == 3) { //top right of a piece
                            cout << ' ';
                        }
                        if (j % 4 == 0) { //padding
                            cout << ' ';
                        }
                    }
                    else if (i % 4 == 2) { //middle row of a piece
                        if (j % 4 == 1) { //middle left of a piece
                            if (OffBoard[(i / 4) * x + (j / 4)].PrintEmptyness() == false) {
                                cout << OffBoard[(i / 4) * x + (j / 4)].PrintL();
                            }
                            else cout << ' ';
                        }
                        if (j % 4 == 2) { //center of a piece
                            if (OffBoard[(i / 4) * x + (j / 4)].PrintEmptyness() == false) {
                                cout << OffBoard[(i / 4) * x + (j / 4)].PrintName();
                            }
                            else cout << ' ';
                        }
                        if (j % 4 == 3) { //middle right of a piece
                            if (OffBoard[(i / 4) * x + (j / 4)].PrintEmptyness() == false) {
                                cout << OffBoard[(i / 4) * x + (j / 4)].PrintR();
                            }
                            else cout << ' ';
                        }
                        if (j % 4 == 0) { //padding
                            cout << ' ';
                        }
                    }
                    else if (i % 4 == 3) { //bottom row of a piece
                        if (j % 4 == 1) { //bottom left of a piece
                            cout << ' ';
                        }
                        if (j % 4 == 2) { //bottom middle of a piece
                            if (OffBoard[((i / 4)) * x + (j / 4)].PrintEmptyness() == false) {
                                cout << OffBoard[((i / 4)) * x + (j / 4)].PrintB();
                            }
                            else cout << ' ';
                        }
                        if (j % 4 == 3) { //bottom right of a piece
                            cout << ' ';
                            count++;
                        }
                        if (j % 4 == 0) { //padding
                            cout << ' ';
                        }
                    }
                    else if (i % 4 == 0) { //padding
                        cout << ' ';
                    }
                }
                else if (!AltAppearance) { //Normal
                    if (i % 3 == 1) { //top row of a piece
                        if (j % 3 == 1) { //top left of a piece
                            cout << ' ';
                        }
                        if (j % 3 == 2) { //top middle of a piece
                            if (OffBoard[(i / 3) * x + (j / 3)].PrintEmptyness() == false) {
                                cout << OffBoard[(i / 3) * x + (j / 3)].PrintT();
                            }
                            else cout << ' ';
                        }
                        if (j % 3 == 0) { //top right of a piece
                            cout << ' ';
                        }
                    }
                    else if (i % 3 == 2) { //middle row of a piece
                        if (j % 3 == 1) { //middle left of a piece
                            if (OffBoard[(i / 3) * x + (j / 3)].PrintEmptyness() == false) {
                                cout << OffBoard[(i / 3) * x + (j / 3)].PrintL();
                            }
                            else cout << ' ';
                        }
                        if (j % 3 == 2) { //center of a piece
                            if (OffBoard[(i / 3) * x + (j / 3)].PrintEmptyness() == false) {
                                cout << OffBoard[(i / 3) * x + (j / 3)].PrintName();
                            }
                            else cout << ' ';
                        }
                        if (j % 3 == 0) { //middle right of a piece
                            if (OffBoard[(i / 3) * x + (j / 3 - 1)].PrintEmptyness() == false) {
                                cout << OffBoard[(i / 3) * x + (j / 3) - 1].PrintR();
                            }
                            else cout << ' ';
                        }
                    }
                    else if (i % 3 == 0) { //bottom row of a piece
                        if (j % 3 == 1) { //bottom left of a piece
                            cout << ' ';
                        }
                        if (j % 3 == 2) { //bottom middle of a piece
                            if (OffBoard[((i / 3) - 1) * x + (j / 3)].PrintEmptyness() == false) {
                                cout << OffBoard[((i / 3) - 1) * x + (j / 3)].PrintB();
                            }
                            else cout << ' ';
                        }
                        if (j % 3 == 0) { //bottom right of a piece
                            cout << ' ';
                            count++;
                        }
                    }
                }
            }
            cout << endl;
        }
    }
    /**
     * @brief looks up the piece in the board and offboard. P-B-R means Pass-By-Reference
     *
     * @param lookupValue {char} - the name of the puzzlepiece to look for
     * @param inBoard {bool} - P-B-R is the found piece in the board or not.
     * @param yIndex {int} - P-B-R the y index of the piece in the board
     * @param xIndex {int} - P-B-R the x index of the piece in the board/offboard
     * @return {bool} -returns true if the piece is found  -returns false if the piece is not found
     */
    bool pieceLookup(char lookupValue, bool& inBoard, int& yIndex, int& xIndex) {
        //god this is awful code
        //check if in board first
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (Board[i][j].PrintEmptyness() == false) {
                    if (Board[i][j].PrintName() == lookupValue) {
                        cout << "\nFound puzzle piece \"" << Board[i][j].PrintName() << "\" in main Board\n";
                        inBoard = true;
                        yIndex = i;
                        xIndex = j;
                        return true;
                    }
                }
            }
        }
        //check if in offboard
        for (int i = 0; i < 25; i++) {
            if (OffBoard[i].PrintEmptyness() == false) {
                if (OffBoard[i].PrintName() == lookupValue) {
                    cout << "\nFound puzzle piece \"" << OffBoard[i].PrintName() << "\" in off Board\n";
                    inBoard = false;
                    yIndex = -1;
                    xIndex = i;
                    return true;
                }
            }
        }
        return false; //we cant find it. (or the code is broken)
    }
    /**
     * @brief displays the piece at the given index.
     *
     * @param inBoard {bool} -if inBoard is true, it is in the board, if false, it is in the offboard
     * @param indexY {int} -the y index of the piece (if inBoard is true ONLY)
     * @param indexX {int} -the x index of the piece
     */
    void displayPiece(bool inBoard, int indexY, int indexX) {
        if (inBoard) {

            cout << ' ' << Board[indexY][indexX].PrintT() << ' ' << endl;
            cout << Board[indexY][indexX].PrintL() << Board[indexY][indexX].PrintName() << Board[indexY][indexX].PrintR() << endl;
            cout << ' ' << Board[indexY][indexX].PrintB() << ' ' << endl;
        }
        else {
            cout << ' ' << OffBoard[indexX].PrintT() << ' ' << endl;
            cout << OffBoard[indexX].PrintL() << OffBoard[indexX].PrintName() << OffBoard[indexX].PrintR() << endl;
            cout << ' ' << OffBoard[indexX].PrintB() << ' ' << endl;
        }
    }
    /**
     * @brief returns the piece emptyness at the given index.
     *
     * @param inBoard {bool} -if inBoard is true, it is in the board, if false, it is in the offboard
     * @param indexY {int} -the y index of the piece (if inBoard is true ONLY)
     * @param indexX {int} -the x index of the piece
     * @return {bool} -returns true if the piece is empty, false if the piece is not empty
     */
    bool PrintPieceEmptyness(bool inBoard, int indexY, int indexX) {
        if (inBoard) {
            return Board[indexY][indexX].PrintEmptyness();
        }
        else {
            return OffBoard[indexX].PrintEmptyness();
        }
    }
    /**
     * @brief Rotates the piece at the given index 90 degree clockwise.
     *
     * @param index {int} Array Number of the piece to rotate in offboard
     */
    void rotatePieceCW(int index) {
        if (OffBoard[index].PrintEmptyness() == false) { //added failsafe, if the piece is empty, dont rotate it (should not happen normally)
            OffBoard[index].RotateCW();
        }
    }
    /**
    * @brief Rotates the piece at the given index 90 degree counter-clockwise.
    *
    * @param index {int} Array Number of the piece to rotate in offboard
    */
    void rotatePieceCCW(int index) {
        if (OffBoard[index].PrintEmptyness() == false) { //added failsafe, if the piece is empty, dont rotate it (should not happen normally)
            OffBoard[index].RotateCCW();
        }
    }
    /**
     * @brief Check if the Offboard is empty
     *
     * @return {bool} -returns true if the offboard is empty, false if it is not
     */
    bool isOffBoardEmpty() {
        for (int i = 0; i < 25; i++) {
            if (OffBoard[i].PrintEmptyness() == false) {
                return false;
            }
        }
        return true;
    }
    /**
     * @brief Move all pieces from the board and to the offboard.
     *
     */
    void removeAllPieceOnBoard() {  //NOT TESTED // should be used when the board is initialized (no idea if we can let user call it or not)
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (Board[i][j].PrintEmptyness() == false) {
                    GameBoard::pullToOffBoard(i, j);
                }
            }
        }
    }
    /**
     * @brief Randomly delete a piece from the board. (Set it to empty)
     *
     * @param piecesToRemove {int} Amount of pieces to be removed.
     */
    void randomRemovePuzzlePiece(const int piecesToRemove) {
        srand(seedRNG + 100);
        int piecesRemoved = 0;
        int randomY;
        int randomX;
        while (piecesRemoved < piecesToRemove) {
            randomY = rand() % 5;
            randomX = rand() % 5;
            if (Board[randomY][randomX].PrintEmptyness() == false) {
                Board[randomY][randomX].setEmptyness(true);
                piecesRemoved++;
            }
        }
    }
    /**
     * @brief Randomly rotates all pieces in the offboard.
     *
     */
    void randomRotateAllOffBoardPiece() { //NOT TESTED // rotate all piece randomly
        srand(seedRNG + 1000);
        int randomRotate;
        for (int i = 0; i < 25; i++) {
            randomRotate = rand() % 4;
            for (int j = 0; j < randomRotate; j++) {
                OffBoard[i].RotateCW();
            }
        }
    }
    /**
     * @brief Generates a random char from A to Z (without Q)
     *
     * @return {char} - a random char from A to Z. Returns \0 if all letters is exhausted.
     */
    char generateRandomLetterNoRepeat() {
        bool repeat = false;
        char generatedChar;
        //prevent inf loop? will return \0 when the usedchar is full
        for (int i = 0, count = 1; i < 25; i++, count++) {
            if (UsedChar[i] == '\0') {
                break;
            }
            if (count == 25) {
                return '\0';
            }
        }
        do {
            repeat = false;
            generatedChar = 'A' + rand() % 26;
            //check if the generated char is Q or not
            if (generatedChar == 'Q') {
                repeat = true;
            }
            //for loop check used char
            for (int i = 0; i < 25; i++) {
                if (generatedChar == UsedChar[i])
                    repeat = true;
            }
        } while (repeat);
        //below set usedchar to the generatedchar, preventing repeat
        for (int i = 0; i < 25; i++) {
            if (UsedChar[i] == '\0') {
                UsedChar[i] = generatedChar;
                break;
            }
        }
        return generatedChar;
    }
private:
    PuzzlePiece OffBoard[25];
    PuzzlePiece Board[5][5];
    char UsedChar[25];
    int puzzlePieceCount, randomNumberRangeLower, randomNumberRangeUpper, seedRNG;
};

void ChangePuzzlePieceCount(int& puzzlePieceCount) {
    int input = 0;
    cout << "\nCurrent puzzle piece count: " << puzzlePieceCount << endl;
    cout << "Please enter new puzzle piece count (1-25) : ";
    do {
        //catch input errors, to improve user experience.
        if (!(cin >> input)) {
            cout << "Invalid input. Please enter new puzzle piece count (1-25) : ";
            cin.clear();
            cin.ignore(10000, '\n');
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }
        //check if input is a number between 1 and 25
        else if (input >= 1 && input <= 25) {
            cout << "Enter successfully!" << endl;
            puzzlePieceCount = input;
        }
        else {
            cout << "Invalid input. Please enter new puzzle piece count (1-25) : ";
        }
    } while (input < 1 || input > 25);
    //Note: the loop will continue to run until the user enters a valid input.
    cout << "Puzzle piece count has been updated to " << puzzlePieceCount << "." << endl;
    waitForEnter();
}

void ChangeRandomNumberRange(int& randomNumberRangeLower, int& randomNumberRangeUpper) {
    int input = 0, lower = 0;
    cout << "\nCurrent random number range: " << randomNumberRangeLower << " to " << randomNumberRangeUpper << endl;
    cout << "A valid random number range is from 0 to 9. " << endl;
    cout << "Please enter new lower bound of random number range: ";

    do {
        //catch input errors, to improve user experience.
        if (!(cin >> input)) {
            cout << "Invalid input. \nPlease enter new lower bound of random number range(0-9):  ";
            cin.clear();
            cin.ignore(10000, '\n');
            input = -1;
            //assign -1 to input to keep the loop running
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }
        //check if input is a number between 0 and 9
        else if (input >= 0 && input <= 9) {
            randomNumberRangeLower = input;
        }
        else {
            cout << "Invalid input. \nPlease enter new lower bound of random number range(0-9): ";
        }
    } while (input < 0 || input > 9);     //check if input is a number between 0 and 9
    //Note: the loop will continue to run until the user enters a valid input.
    lower = randomNumberRangeLower;
    //assigning this value to a variable wastes memory, but it help prevent me doing something wrong.
    cout << "Please enter new upper bound of random number range: ";
    //check if input is a number between randomNumberRangeLower and 9
    do {
        //catch input errors, to improve user experience.
        if (!(cin >> input)) {
            cout << "Invalid input. \nPlease enter new upper bound of random number range(" << lower << "-9) : ";
            cin.clear();
            cin.ignore(10000, '\n');
            input = -1;
            //assign -1 to input to keep the loop running
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }
        //check if input is a number between 0 and 9
        if (input >= lower && input <= 9) {
            randomNumberRangeUpper = input;
        }
        else {
            cout << "Invalid input. \nPlease enter new upper bound of random number range(" << lower << "-9) : ";
        }
    } while (input < lower || input > 9);
    //Note: the loop will continue to run until the user enters a valid input.
    cout << "Random number range has been updated to " << randomNumberRangeLower << " to " << randomNumberRangeUpper << "." << endl;
    waitForEnter();
}

void ChangeSeed(int& seed) {
    int input = 0;
    cout << "\nCurrent puzzle seed: " << seed << endl;
    cout << "The puzzle seed determines the randomness of the puzzle. " << endl;
    cout << "The same puzzle seed will always generate the same puzzle. " << endl;
    cout << "Default seed is based on time. " << endl;
    cout << "Type \"-1\" to automatically change seed." << endl;
    cout << "Please enter new puzzle seed (integer) : ";
    do {
        //catch input errors, to improve user experience.
        if (!(cin >> input)) {
            cout << "Invalid input. \nPlease enter new puzzle seed (integer) : ";
            cin.clear();
            cin.ignore(10000, '\n');
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }

        else if (input >= 0) {        //check if input is a integer > 0
            seed = input;
            break;
        }
        else if (input < 0) {
            seed = (int)time(0);
            break;
        }
        else {
            cout << "Invalid input. \nPlease enter new puzzle seed (integer) : "; //dont see how you can trigger this but lets keep it
        }
    } while (true);
    //Note: the loop will continue to run until the user enters a valid input.
    cout << "\nPuzzle seed has been updated to " << seed << "." << endl;
    waitForEnter();
}

void ToggleTimer(bool& timer) {
    int input = 0;
    cout << "\nTimer is currently" << (timer ? " enabled." : " disabled.") << endl;
    cout << "\nEnabling timer will display the time it took to solve the puzzle when the game ends." << endl;
    cout << "1. Enable timer" << endl;
    cout << "2. Disable timer" << endl;
    cout << "Please choose (1 - 2) to change timer: ";
    do {
        //catch input errors, to improve user experience.
        if (!(cin >> input)) {
            cout << "Invalid input.";
            cout << "1. Enable timer" << endl;
            cout << "2. Disable timer" << endl;
            cout << "Please choose: ";
            cin.clear();
            cin.ignore(10000, '\n');
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }
        //check if input is a integer > 0
        else if (input >= 1) {
            if (input == 1) {
                timer = true;
            }
            else if (input == 2) {
                timer = false;
            }
            else {
                cout << "Invalid input. Please enter new option (1 - 2) :";
            }
        }
        else {
            cout << "Invalid input. Please enter new option (1 - 2) :";
        }

    } while (input != 1 && input != 2);
    //Note: the loop will continue to run until the user enters a valid input.
    cout << "Timer is now" << (timer ? " enabled." : " disabled.") << endl;
    waitForEnter();
}

void ToggleAltOffBoard(bool& altOffBoard) {
    int input = 0;
    cout << "\nYou are currently using" << (altOffBoard ? " alternate offboard." : " normal offboard.") << endl;
    cout << "Alternate offboard have paddings around the pieces to help you see." << endl;
    cout << "1. Use normal offboard" << endl;
    cout << "2. Use alternate offboard" << endl;
    cout << "Please choose (1 - 2): ";
    do {
        //catch input errors, to improve user experience.
        if (!(cin >> input)) {
            cout << "Invalid input.";
            cout << "1. Use normal offboard" << endl;
            cout << "2. Use alternate offboard" << endl;
            cout << "Please choose: ";
            cin.clear();
            cin.ignore(10000, '\n');
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }
        //check if input is a integer > 0
        else if (input >= 1) {
            if (input == 1) {
                altOffBoard = false;
            }
            else if (input == 2) {
                altOffBoard = true;
            }
            else {
                cout << "Invalid input. Please enter new option (1 - 2) :";
            }
        }
        else {
            cout << "Invalid input. Please enter new option (1 - 2) :";
        }

    } while (input != 1 && input != 2);
    //Note: the loop will continue to run until the user enters a valid input.
    cout << "You are now using" << (altOffBoard ? " alternate offboard." : " normal offboard.") << endl;
    waitForEnter();
}

void SettingMenu(int& puzzlePieceCount, int& randomNumberRangeLower, int& randomNumberRangeUpper) {
    //Enter the Setting Option
    int SettingOption;
    do {
        clearConsole();
        cout << "\n*** Settings Menu ***\n";
        cout << "[1] Number of puzzle pieces\n";
        cout << "[2] Range of random numbers\n";
        cout << "[3] Return to Main Menu\n";
        cout << "*********************\n";
        cout << "Option (1 - 3) : ";

        if (!(cin >> SettingOption)) {
            cout << "Invalid input. Please enter new Setting Option (1 - 3) \n";
            waitForEnter();
            cin.clear();
            cin.ignore(10000, '\n');
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }
        switch (SettingOption) {
        case 1: ChangePuzzlePieceCount(puzzlePieceCount);
            continue;
            //Change the Number of puzzle pieces
        case 2: ChangeRandomNumberRange(randomNumberRangeLower, randomNumberRangeUpper);
            continue;
            //Change the Range of random numbers
        case 3:
            clearConsole();
            return;
            //Return to Main Menu
        default:
            cout << "Invalid input. Please enter new Setting Option (1 - 3) \n";
            waitForEnter();
            continue;
        }
    } while (true);
}

void UsefulFeatureMenu(int& seed, bool& timer, bool& altOffBoard) {
    //Enter the Setting Option
    int SettingOption;
    do {
        clearConsole();
        //catch input errors, to improve user experience.
        cout << "\n*** Extras Menu ***\n";
        cout << "[1] Change Puzzle Seed\n";
        cout << "[2] Toggle Timer\n";
        cout << "[3] Alternate Offboard Appearence\n";
        cout << "[4] Return to Main Menu\n";
        cout << "*********************\n";
        cout << "Option (1 - 4) : ";

        if (!(cin >> SettingOption)) {
            cout << "Invalid input. Please enter new option (1 - 4) \n";
            waitForEnter();
            cin.clear();
            cin.ignore(10000, '\n');
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }

        switch (SettingOption) {
        case 1: ChangeSeed(seed);
            continue;
        case 2:
            ToggleTimer(timer);
            continue;
        case 3:
            ToggleAltOffBoard(altOffBoard);
            continue;
        case 4:
            clearConsole();
            return;
            //Return to Main Menu
        default:
            cout << "Invalid input. Please enter new option (1 - 4) \n";
            waitForEnter();
            continue;
        }
    } while (true);
}

void showInfo() {
    // Fill in your personal particulars below
    cout << "*** Credits ***" << endl;
    cout << "\nStudent 1:\n";
    cout << "Name          : Ng Ka Ki\n";
    cout << "Student ID    : 21106575A\n";
    cout << "Class         : SEHH2042\n";
    cout << "Tutorial Group: 203C\n\n";

    cout << "Student 2:\n";
    cout << "Name          : Yeung Cheuk Hin\n";
    cout << "Student ID    : 21088854A\n";
    cout << "Class         : SEHH2042\n";
    cout << "Tutorial Group: 203D\n\n";

    cout << "Student 3:\n";
    cout << "Name          : Siu Yeung Yeung\n";
    cout << "Student ID    : 21040219A\n";
    cout << "Class         : SEHH2042\n";
    cout << "Tutorial Group: 203A\n\n";

    cout << "Student 4:\n";
    cout << "Name          : Yang Han Jun\n";
    cout << "Student ID    : 21131314A\n";
    cout << "Class         : SEHH2042\n";
    cout << "Tutorial Group: 203C\n\n";

    cout << "Student 5:\n";
    cout << "Name          : Li Wing Sum\n";
    cout << "Student ID    : 21117516A\n";
    cout << "Class         : SEHH2042\n";
    cout << "Tutorial Group: 203B\n\n";
    waitForEnter();
    clearConsole();

}

void StartGame(int puzzlePieceCount, int randomNumberRangeLower, int randomNumberRangeUpper, int seed, bool timer, bool altOffBoard) {
    //init game.
    clearConsole();
    Timer timecount;
    timecount.start();
    GameBoard gameBoard(puzzlePieceCount, randomNumberRangeLower, randomNumberRangeUpper, seed);
    GameBoard Solution(puzzlePieceCount, randomNumberRangeLower, randomNumberRangeUpper, seed);
    gameBoard.GeneratePuzzlePieces();
    //gameBoard.displayBoard();
    gameBoard.randomRemovePuzzlePiece(25 - puzzlePieceCount);
    //gameBoard.displayBoard();
    Solution = gameBoard; //save a copy of the gameboard as solution.
    gameBoard.removeAllPieceOnBoard(); //pull the pieces that we generated to OffBoard
    //gameBoard.displayBoard();
    //gameBoard.displayOffBoard();
    //all instances of diaplayboards are for debugging purposes.
    gameBoard.randomRotateAllOffBoardPiece();
    //end init. the board has been generated and randomized.
    //gameplay loop below.
    bool isGameOver = false;
    bool isGameWon = false;
    char input, ConfirmedExit;
    string coordinates, charinput; //bad naming on my part, charinput should be input. and viceversa, but i typed too many codes to switch now.
    int intinput, yindex, xindex;
    int targetY, targetX;
    bool inBoard;
    PuzzlePiece selectedPiece; //may not be used
    cout << "Welcome to the Puzzle Game!\n";
    cout << "You have " << puzzlePieceCount << " puzzle pieces.\n";
    cout << "The goal is to place all the puzzle pieces to the board.\n";
    cout << "You win when all the puzzle pieces are placed on the board.\n";
    cout << "Quit at any time by pressing \"Q\" on the Puzzle Piece selection prompt.\n";
    waitForEnter();
    while (isGameOver == false) {
        gameBoard.displayBoard();
        gameBoard.displayOffBoard(altOffBoard);
        cout << "Choose a Puzzle Piece that is in the game.\n";
        cout << "Type the charcter in the center of the piece to choose.\n";
        cout << "To quit the game, press \"Q\"\n";
        cout << "Selection :";
        cin >> charinput;
        cout << endl;
        if (charinput.length() > 1) {
            cout << "Invalid input. Please enter a single character.\n";
            waitForEnter();
            clearConsole();
            continue;
        }
        input = charinput[0]; //map charinput {string} to input {char} so that the following comparison can be done.
        if (input >= 'a' && input <= 'z') {//check if the input is a uppercase letter. if not, convert it to uppercase.
            input = (char)toupper(input);
        }
        if (input == 'Q') {
            cout << "If you leave the game, you will lose!\nAre you sure you want to leave the game? (Y/N) : ";// promt Msg
            cin >> charinput;
            if (charinput.length() > 1) {
                cout << "Invalid input. Please enter a single character.\n";
                waitForEnter();
                continue;
            }
            ConfirmedExit = charinput[0];
            if (ConfirmedExit == 'N' || ConfirmedExit == 'n') {
                cout << "Back to game... " << endl;
                waitForEnter();
                clearConsole();
                continue;
            }
            else if (ConfirmedExit == 'Y' || ConfirmedExit == 'y') { // note that the loop continues if the input is not Y/y, same effect as N/n
                isGameOver = true;
                cout << endl;
                clearConsole();
                break;
            }
        }
        else if (gameBoard.pieceLookup(input, inBoard, yindex, xindex) == true) {
            if (inBoard == true) {
                //show the piece that is selected.
                clearConsole();
                cout << "Selected Puzzle Piece: " << endl;
                gameBoard.displayPiece(inBoard, yindex, xindex);
                cout << "Remove this piece from board? (Y/N): ";
                cin >> charinput; // only accept 1st char input.
                if (charinput.length() > 1) {
                    cout << "Invalid input. Please enter a single character.\n";
                    waitForEnter();
                    continue;
                }
                input = charinput[0];
                if (input == 'Y' || input == 'y') {
                    gameBoard.pullToOffBoard(yindex, xindex);
                    clearConsole();
                    cout << "Puzzle Piece taken out.\n";
                }
                else if (input == 'N' || input == 'n') {
                    cout << "Puzzle Piece not removed.\n";
                    waitForEnter();
                }
                else {
                    cout << "Invalid input. Puzzle Piece not removed.\n";
                    waitForEnter();
                }
            }
            else {
                //show the piece that is selected.
                do {
                    cout << "\nSelected Puzzle Piece: " << endl;
                    gameBoard.displayPiece(inBoard, yindex, xindex);
                    cout << "Choose action:\n";
                    cout << "1. Place Puzzle Piece on board\n";
                    cout << "2. Rotate Puzzle Piece clockwise\n";
                    cout << "3. Rotate Puzzle Piece counter-clockwise\n";
                    cout << "4. Cancel\n";
                    cout << "Enter your choice: ";
                    if (!(cin >> intinput)) {
                        cout << "Invalid input. Please enter action (1 - 4) \n";
                        waitForEnter();
                        clearConsole();

                        cin.clear();
                        cin.ignore(10000, '\n');
                        continue; //continue will skip the rest of the code in the loop and ask the user for input again
                    }
                    switch (intinput) {
                    case 1:
                        do {
                            gameBoard.displayBoard(); //show the board again (in case the player rotated the piece many times and the board is offscreen)
                            gameBoard.displayPiece(inBoard, yindex, xindex);
                            cout << "Choose a location to place the Puzzle Piece on the board.\n" << "(e.g. 1A, 2B)\n" << "Type 'C' ONLY to cancel.\n";
                            cout << "Location :";
                            if (!(cin >> coordinates)) {
                                cout << "Invalid input. Please enter coordinates (e.g. 1A, 2B) \n";
                                cin.clear();
                                cin.ignore(10000, '\n');
                                waitForEnter();
                                clearConsole();
                                continue; //continue will skip the rest of the code in the loop and ask the user for input again
                            }
                            if (coordinates.at(0) == 'c' || coordinates.at(0) == 'C') {
                                clearConsole();
                                cout << "Canceled piece placement!\n";
                                break;
                            }
                            if (coordinates.length() != 2) { //check if the input is 2 characters long.
                                cout << "Invalid input. Please enter coordinates (e.g. 1A, 2B) \n";
                                cin.clear();
                                cin.ignore(10000, '\n');
                                waitForEnter();
                                clearConsole();
                                continue; //continue will skip the rest of the code in the loop and ask the user for input again
                            }
                            targetY = coordinates.at(0) - '1'; //decode coordinates
                            targetX = (char)toupper(coordinates.at(1)) - 'A';
                            if (targetX < 0 || targetX > 4 || targetY < 0 || targetY > 4) { //check if the coordinates are valid
                                cout << "Invalid coordinates.\n";
                                waitForEnter();
                                clearConsole();
                                continue;
                            }
                            else {
                                if (gameBoard.pushToBoard(targetY, targetX, xindex) == 0) {
                                    clearConsole();
                                    cout << "Piece placed";
                                    break;
                                }
                                else {
                                    waitForEnter(); //Let user see why placment failed. DO NOT DELETE!
                                    continue;
                                }
                            }
                            break;
                        } while (true);
                        break;
                    case 2:
                        gameBoard.rotatePieceCW(xindex);
                        clearConsole();
                        gameBoard.displayBoard();
                        continue;
                    case 3:
                        gameBoard.rotatePieceCCW(xindex);
                        clearConsole();
                        gameBoard.displayBoard();
                        continue;
                    case 4:
                        clearConsole();
                        break;
                    default:
                        cout << "Input out of range.Please enter (1 - 4)" << endl;
                        waitForEnter();
                        clearConsole();
                        continue;
                    }
                    break;
                } while (true);
            }

        }
        else {
            cout << "Invalid input. Please enter a valid Puzzle Piece. \n";
            waitForEnter();
            clearConsole();
        }
        if (gameBoard.isOffBoardEmpty() == true) { //check if the game is won.
            isGameWon = true;
            isGameOver = true;
        }
    }
    if (isGameWon == true) {
        gameBoard.displayBoard();
        gameBoard.displayOffBoard(altOffBoard);
        cout << "You won!\n";

        if (timer) {
            cout << "Time taken: " << timecount.stop() << " seconds.\n";
        }
        waitForEnter();
        clearConsole();
    }
    else {
        gameBoard.displayBoard();
        gameBoard.displayOffBoard(altOffBoard);
        cout << "You lost!\n";
        if (timer) {
            cout << "Time taken: " << timecount.stop() << " seconds.\n";
        }
        waitForEnter();
        clearConsole();
        cout << "+----------+\n| Solution |\n+----------+\n";
        Solution.displayBoard();
        cout << "Returning to main menu...\n";
        waitForEnter();
        clearConsole();
    }
}

void AskForExit(char& ExitOption) {
    cout << "\nComfirm to Exit? (Y/N) :"; // promt Msg
    do {
        cin >> ExitOption;  // input Y/N
        if (ExitOption == 'Y' || ExitOption == 'y' || ExitOption == 'N' || ExitOption == 'n')
            return;
        else {
            cout << "Invalid input. Please enter Y or N: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    } while (true);
}

void WarnMultiplePlays(bool& doNotRemind, int& seed) {

    int input;
    do {
        cout << "Starting another game in the same session will result in the same game due to using the same seed." << endl;
        cout << "Do you wish to change the seed of the puzzle?" << endl;
        cout << "\n*********************\n";
        cout << "[1] Change Puzzle Seed manually\n";
        cout << "[2] Change Puzzle Seed automatically\n";
        cout << "[3] Play the same puzzle\n";
        cout << "[4] Do not remind again in this session" << (doNotRemind ? " ENABLED!" : " ") << "\n";
        cout << "*********************\n";
        cout << "Option (1 - 4) : ";

        if (!(cin >> input)) {
            cout << "Invalid input. Please enter new option (1 - 4) \n";
            waitForEnter();
            cin.clear();
            cin.ignore(10000, '\n');
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }

        switch (input) {
        case 1:
            ChangeSeed(seed);
            return;
        case 2:
            seed = (int)time(0);
            return;
        case 3:
            //do nothing LOL
            return;
        case 4:
            clearConsole();
            doNotRemind = true;
            continue;
            //Return to Main Menu
        default:
            cout << "Invalid input. Please enter new option (1 - 4) \n";
            waitForEnter();
            continue;
        }
    } while (true);
}

int main() {
    int i;
    int puzzlePieceCount = 15;
    int randomNumberRangeLower = 0;
    int randomNumberRangeUpper = 5;
    int seed = (int)time(0);
    int playCount = 0;
    bool timer = false;
    bool altOffBoard = false;
    bool doNotRemindSeed = false;
    char ConfirmedExit = 0;
    cout << "\n[Welcome to the puzzle game!!]\n";
    do {
        cout << "\n*** Main Menu ***" << endl;
        cout << "[1] Start Game" << endl;
        cout << "[2] Settings" << endl;
        cout << "[3] Useful feature(s) added" << endl;
        cout << "[4] Credits" << endl;
        cout << "[5] Exit" << endl;
        cout << "*******************" << endl;
        cout << "Option (1 - 5):";
        //catch input errors, to improve user experience.
        if (!(cin >> i)) {
            cout << "Invalid input. Please enter Option (1 - 5) " << endl;
            waitForEnter();
            clearConsole();
            cin.clear();
            cin.ignore(10000, '\n');
            //program will still crash when the user enters a string of size 10001 or more
            continue;
            //continue will skip the rest of the code in the loop and ask the user for input again
        }
        switch (i) {
        case 1:
            if (playCount > 0 && !(doNotRemindSeed)) {
                clearConsole();
                WarnMultiplePlays(doNotRemindSeed, seed);
            }
            StartGame(puzzlePieceCount, randomNumberRangeLower, randomNumberRangeUpper, seed, timer, altOffBoard);
            playCount++;
            continue;
        case 2:
            SettingMenu(puzzlePieceCount, randomNumberRangeLower, randomNumberRangeUpper);
            continue;
        case 3:
            clearConsole();
            UsefulFeatureMenu(seed, timer, altOffBoard);
            continue;
        case 4:
            clearConsole();
            showInfo();
            continue;
        case 5:
            AskForExit(ConfirmedExit);

            if (ConfirmedExit == 'N' || ConfirmedExit == 'n') {
                cout << "Return to main menu...\n";
                waitForEnter();
                clearConsole();
                continue;
            }
            else if (ConfirmedExit == 'Y' || ConfirmedExit == 'y') {
                cout << "Thank you for playing!\n"; //change if you want
                cout << "Exiting ... ...";
                break;// exit switch
            }
            break;

        default:
            cout << "Invalid input. Please enter Option (1 - 5) " << endl;
            waitForEnter();
            clearConsole();
            continue;
        }
        break; //this should only triggers when the user enters 5 --> Y
    } while (true); //this check is useless, could make it true instead
    return 0;
}
