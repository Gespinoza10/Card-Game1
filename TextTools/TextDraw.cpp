#include "TextDraw.h"

TextDraw::TextDraw(int r, int c, bool hollow = false)
{
    xPivot = yPivot = 0;
    rows = r;
    columns = c;
    symbol = 219;
    currentColor = 15;
    currentBackgroundColor = 0;
    drawBorderFlag = false;
    PalletForegroundColor_y = rows + 4;
    PalletForegroundColor_x = 50;
    PalletBackgroundColor_y = rows + 4;
    PalletBackgroundColor_x = 65;
    fillFlag = false;
    symbolX = symbolY = 0;

    // initialize array with border
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            board[i][j].color = currentColor;
            board[i][j].bgcolor = currentBackgroundColor;
            if (!hollow)
            {
                board[i][j].symbol = 219;
                board[i][j].color = 15;
            }
            else
            {
                if ((i == 0) || (i == rows - 1) || (j == 0) || (j == columns - 1))
                {
                    board[i][j].symbol = 219;
                    board[i][j].color = 15;
                }
                else
                    board[i][j].symbol = 32;
            }
        }
    }
    gotoxy(PalletForegroundColor_x, PalletForegroundColor_y - 1);
    setForeGroundAndBackGroundColor(15, 0);
    cout << "F-Color: ";
    setForeGroundAndBackGroundColor(currentColor, 0);
    cout << char(219);
    gotoxy(PalletBackgroundColor_x, PalletBackgroundColor_y - 1);
    setForeGroundAndBackGroundColor(15, 0);
    cout << "B-Color: ";
    setForeGroundAndBackGroundColor(currentBackgroundColor, currentBackgroundColor);
    cout << char(219);
    setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
}


void TextDraw::DisplaySymbols()
{
    int rowcount = 0, colcount = 0;
    int symbolsperrow = 0;

    setForeGroundAndBackGroundColor(15, 0);
    gotoxy(0, PalletForegroundColor_y);
    // we will display only displayable symbols. 1-6 then 16 and up
    for (int i = 0; i < 255; i++)
    {
        if ((i >= 1 && i <= 6) || ((i >= 28) && (i != 127)))
        {
            cout << setw(3);
            cout << char(i);
            symbols[rowcount][colcount++] = i;
            symbolsperrow++;
            if (symbolsperrow % 15 == 0)
            {
                cout << endl;
                rowcount++;
                colcount = 0;
            }
        }
        else if (i == 7) // we want the symbols 1 to 6 to be on a separate row (for calculations later).
        {
            cout << endl;
            rowcount++;
            colcount = 0;
            symbolsperrow = 0;
        }
    }
    // display forground color pallet
    int colorcount = 0;
    int xloc = 0;
    int r, c;
    r = c = 0;
    int thecolor = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(PalletForegroundColor_x + j, PalletForegroundColor_y + i);
            setForeGroundAndBackGroundColor(thecolor, 0);
            cout << char(219);
            colorPallet[i][j] = thecolor;
            thecolor++;
        }
    }

    // display background color pallet
    colorcount = 0;
    xloc = 0;
    r = c = 0;
    thecolor = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(PalletBackgroundColor_x + j, PalletBackgroundColor_y + i);
            setForeGroundAndBackGroundColor(thecolor, 0);
            cout << char(219);
            colorPallet_Background[i][j] = thecolor;
            thecolor++;
        }
    }

    // set color to default
    setForeGroundAndBackGroundColor(15, 0);
}

void TextDraw::UpdateBoardElement(int r, int c, int value, int clr, int bgclr)
{
    board[r][c].symbol = value;
    board[r][c].color = clr;
    board[r][c].bgcolor = bgclr;
}

void TextDraw::DisplayFrameTypes()
{
    setForeGroundAndBackGroundColor(15, 0);
    gotoxy(PalletForegroundColor_x, PalletForegroundColor_y + 6);
    cout << "Borders";
    gotoxy(PalletForegroundColor_x, PalletForegroundColor_y + 7);
    cout << char(218) << char(196) << char(191);
    gotoxy(PalletForegroundColor_x, PalletForegroundColor_y + 8);
    cout << char(192) << char(196) << char(217);

    gotoxy(PalletForegroundColor_x+3, PalletForegroundColor_y + 7);
    cout << char(201) << char(205) << char(187);
    gotoxy(PalletForegroundColor_x+3, PalletForegroundColor_y + 8);
    cout << char(200) << char(205) << char(188);

    gotoxy(PalletForegroundColor_x, PalletForegroundColor_y + 9);
    cout << "*WILD*";
    setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
}

void TextDraw::DisplayInterface()
{
    gotoxy(0, rows + 3);
    PrintCommandOptions("[Save]  [Load]  [Fill]  [Reset]  [Paste]");
    DisplaySymbols();
    DisplayFrameTypes();
    DrawShape();
}

void TextDraw::DrawShape()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            M.gotoxy(j+xPivot, i+yPivot);
            setForeGroundAndBackGroundColor(board[i][j].color, board[i][j].bgcolor);
            cout << char(board[i][j].symbol);
        }
    }
}

void TextDraw::StartDrawing()
{
    while (1)
    {

        M.ReadMouseInput();

        switch (M.InputRecord.EventType) {
        case KEY_EVENT: // keyboard input 


            switch (M.InputRecord.Event.KeyEvent.wVirtualKeyCode)
            {
            case 'l':
            case 'L': // load all files. 
                    struct dirent* d;
                    DIR* dr;

                    dr = opendir("files/");
                    if (dr != NULL)
                    {
                        for (d = readdir(dr); d != NULL; d = readdir(dr))
                        {
                            string path = "files/";
                            path = path + d->d_name;
                            LoadFile(path);
                            xPivot = xPivot + columns + 5;
                        }
                        closedir(dr);
                    }
                    xPivot = yPivot = 0;
                    for (int i=0; i < rows; i++)
                        for (int j = 0; j < columns; j++)
                        {
                            board[i][j].bgcolor = 0;
                            board[i][j].color = 15;
                            board[i][j].symbol = ' ';
                        }
                break;
            case 27:

                return;
                break;
            }//switch

            //---------------------------------------------------------------------------------
            break;

        case MOUSE_EVENT: // mouse input 
            if (M.InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                M.coord.X = M.InputRecord.Event.MouseEvent.dwMousePosition.X;
                M.coord.Y = M.InputRecord.Event.MouseEvent.dwMousePosition.Y;
                gotoxy(PalletBackgroundColor_x, 26);
                cout << M.coord.X << ":" << M.coord.Y;
                if ((M.coord.X < PalletForegroundColor_x) && (M.coord.Y >= PalletForegroundColor_y) && (M.coord.Y <= PalletForegroundColor_y+17)) // he clicked on symbol
                {
                    // calculate row
                    int yloc = M.coord.Y - (PalletForegroundColor_y);
                    int xloc = M.coord.X / 3;
                    // this is done for accuracy. User MUST click on letter, not next to letter
                    // to check accuracy, I look at the remainder, if remainder is 2, that means he clicked exactly on the letter
                    // otherwise, he's off, so I ignore the click
                    if (M.coord.X % 3 == 2)
                    {
                        // clear background of previous selected letter
                        if (symbolY != 0) // was there a previous selection? If yes, clear it
                        {
                            int t1 = symbolX / 3;
                            int t2 = symbolY - (PalletForegroundColor_y);
                            gotoxy(symbolX, symbolY);
                            setForeGroundAndBackGroundColor(15, 0);
                            cout << char(symbols[t2][t1]);
                        }
                        symbolX = M.coord.X;
                        symbolY = M.coord.Y;
                        symbol = symbols[yloc][xloc];
                        setForeGroundAndBackGroundColor(10, 0);
                        gotoxy(symbolX, symbolY);
                        cout << char(symbol);
                        setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
                        // if a symbol is selected and drawBorderFlag is true, that means he wants to use that symbol to draw border
                        if (drawBorderFlag)
                        {
                            for (int i = 0; i < columns; i++)
                            {
                                board[0][i].symbol = board[rows - 1][i].symbol = symbol;
                                board[0][i].color = currentColor;
                                board[0][i].bgcolor = currentBackgroundColor;
                                board[rows - 1][i].color = currentColor;
                                board[rows - 1][i].bgcolor = currentBackgroundColor;
                            }
                            for (int i = 0; i < rows; i++)
                            {
                                board[i][0].symbol = board[i][columns - 1].symbol = symbol;
                                board[i][0].color = currentColor;
                                board[i][0].bgcolor = currentBackgroundColor;
                                board[i][columns - 1].color = currentColor;
                                board[i][columns - 1].bgcolor = currentBackgroundColor;
                            }
                        }
                        DrawShape();
                    }
                }
                else if (M.coord.Y == rows + 3) // clicked on save/load
                {
                    if (M.coord.X <= 5) // save
                    {
                        gotoxy(0, PalletForegroundColor_y-1);
                        PrintCommandOptions("-WAIT-  [Load]  [Fill]  [Reset]");
                        ofstream output("files/output.txt");
                        output << rows << " " << columns << endl;
                        for (int i = 0; i < rows; i++)
                        {
                            for (int j = 0; j < columns; j++)
                                output << board[i][j].symbol << " " << board[i][j].color << " " << board[i][j].bgcolor << " ";
                            output << endl;
                        }
                        output.close();
                        Sleep(1000);
                        gotoxy(0, PalletForegroundColor_y - 1);
                        PrintCommandOptions("[Save]  [Load]  [Fill]  [Reset]  [Paste]");
                    }
                    else if (M.coord.X <= 15) // clicked LOAD
                    {
                        LoadFile("files/output.txt");
                        xPivot = yPivot = 0;
                    }
                    else  if (M.coord.X <= 20) // clicked FILL
                    {
                        fillFlag = true;
                        gotoxy(0, PalletForegroundColor_y - 1);
                        PrintCommandOptions("[Save]  [Load]  [-ON-]  [Reset]");
                    }
                    else  if ((M.coord.X >= 24) && (M.coord.X <= 29)) // clicked Reset
                    {
                        for (int i = 0; i < rows; i++)
                            for (int j = 0; j < columns; j++)
                                UpdateBoardElement(i, j, 32, 0, 0);
                        DrawShape();
                    }
                    else  if ((M.coord.X >= 32) && (M.coord.X <= 39)) // clicked Paste
                    {
                        PasteClipboard();
                        //DrawShape();
                    }
                }
                else if ((M.coord.Y >= PalletForegroundColor_y + 7) && ((M.coord.Y <= PalletForegroundColor_y + 9)))  // clicked on border
                {
                    if (M.coord.Y == PalletForegroundColor_y + 9)  //clicked on WILD
                    {
                        if ((M.coord.X >= PalletForegroundColor_x) && (M.coord.X < PalletForegroundColor_x + 6))
                        {
                            if (!drawBorderFlag)
                            {
                                gotoxy(PalletForegroundColor_x, PalletForegroundColor_y + 9);
                                setForeGroundAndBackGroundColor(15, 0);
                                cout << "Select Symbol";
                                setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
                                drawBorderFlag = true;
                            }
                            else
                            {
                                gotoxy(PalletForegroundColor_x, PalletForegroundColor_y + 9);
                                setForeGroundAndBackGroundColor(15, 0);
                                cout << "*WILD*       ";
                                setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
                                drawBorderFlag = false;
                            }
                        }
                    }
                    else if ((M.coord.X >= PalletForegroundColor_x) && (M.coord.X < PalletForegroundColor_x + 3)) // thin border
                    {
                        UpdateBoardElement(0, 0, 218, currentColor, currentBackgroundColor);
                        UpdateBoardElement(0, columns - 1, 191, currentColor, currentBackgroundColor);
                        UpdateBoardElement(rows - 1, 0, 192, currentColor, currentBackgroundColor);
                        UpdateBoardElement(rows - 1,columns - 1, 217, currentColor, currentBackgroundColor);
                        for (int i = 1; i < columns - 1; i++)
                        {
                            UpdateBoardElement(0, i, 196, currentColor, currentBackgroundColor);
                            UpdateBoardElement(rows - 1, i, 196, currentColor, currentBackgroundColor);
                        }
                        for (int i = 1; i < rows - 1; i++)
                        {
                            UpdateBoardElement(i, 0, 179, currentColor, currentBackgroundColor);
                            UpdateBoardElement(i, columns - 1, 179, currentColor, currentBackgroundColor);
                        }
                        DrawShape();
                    }
                    else if ((M.coord.X >= PalletForegroundColor_x+3) && (M.coord.X < PalletForegroundColor_x + 6)) // double border
                    {       
                        UpdateBoardElement(0, 0, 201, currentColor, currentBackgroundColor);
                        UpdateBoardElement(0, columns - 1, 187, currentColor, currentBackgroundColor);
                        UpdateBoardElement(rows - 1, 0, 200, currentColor, currentBackgroundColor);
                        UpdateBoardElement(rows - 1, columns - 1, 188, currentColor, currentBackgroundColor);
                        for (int i = 1; i < columns - 1; i++)
                        {
                            UpdateBoardElement(0, i, 205, currentColor, currentBackgroundColor);
                            UpdateBoardElement(rows - 1, i, 205, currentColor, currentBackgroundColor);
                        }
                        for (int i = 1; i < rows - 1; i++)
                        {
                            UpdateBoardElement(i, 0, 186, currentColor, currentBackgroundColor);
                            UpdateBoardElement(i, columns - 1, 186, currentColor, currentBackgroundColor);
                        }
                        DrawShape();
                    }
                }
                else if ((M.coord.Y >= PalletForegroundColor_y) && ((M.coord.Y < PalletForegroundColor_y + 4))) // he clicked on a symbol.
                {
                    // did he click on color?
                    if ((M.coord.X >= PalletForegroundColor_x) && (M.coord.X < PalletForegroundColor_x+4) ) // clicked on forecolor pallet
                    {
                        int yloc = M.coord.Y - (PalletForegroundColor_y);
                        int xloc = M.coord.X - PalletForegroundColor_x;
                        int c = colorPallet[yloc][xloc];
                        currentColor = c;
                        gotoxy(PalletForegroundColor_x, PalletForegroundColor_y -1);
                        setForeGroundAndBackGroundColor(15, 0);
                        cout << "F-Color: ";
                        setForeGroundAndBackGroundColor(currentColor, 0);
                        cout << char(219);
                        setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
                    }
                    else if ((M.coord.X >= PalletBackgroundColor_x) && (M.coord.X < PalletBackgroundColor_x + 4)) // clicked on backcolor pallet
                    {
                        int yloc = M.coord.Y - (PalletBackgroundColor_y);
                        int xloc = M.coord.X - PalletBackgroundColor_x;
                        int c = colorPallet_Background[yloc][xloc];
                        currentBackgroundColor = c;
                        gotoxy(PalletBackgroundColor_x, PalletBackgroundColor_y - 1);
                        setForeGroundAndBackGroundColor(15, 0);
                        cout << "B-Color: ";
                        setForeGroundAndBackGroundColor(currentBackgroundColor, currentBackgroundColor);
                        cout << char(219);
                        setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
                    }
                }
                else
                {
                    // if clicked outside drawing area, ignore it.
                    if (M.coord.Y < rows && M.coord.X < columns)
                    {
                        // check if fill Flag is not true, if true, he wants to replace color (fill)
                        if (!fillFlag)
                        {
                            board[M.coord.Y][M.coord.X].symbol = symbol;
                            board[M.coord.Y][M.coord.X].color = currentColor;
                            board[M.coord.Y][M.coord.X].bgcolor = currentBackgroundColor;
                            M.gotoxy(M.coord.X, M.coord.Y);
                            setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
                            cout << char(symbol);
                        }
                        else
                        {
                            // get color of area clicked
                            int originalColor = board[M.coord.Y][M.coord.X].color;
                            int originalSymbol = board[M.coord.Y][M.coord.X].symbol;
                            // now replace original color with "currentColor".
                            for (int i = 0; i < rows; i++)
                                for (int j = 0; j < columns; j++)
                                    if (board[i][j].symbol == originalSymbol)
                                    {
                                        board[i][j].color = currentColor;
                                        board[i][j].bgcolor = currentBackgroundColor;
                                        board[i][j].symbol = symbol;    
                                    }
                            fillFlag = false;
                            gotoxy(0, PalletForegroundColor_y - 1);
                            PrintCommandOptions("[Save]  [Load]  [Fill]  [Reset]  [Paste]");
                            // redraw shape
                            DrawShape();
                        }
                    }
                }
                setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);
            }
            else if (M.InputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
            {
                M.coord.X = M.InputRecord.Event.MouseEvent.dwMousePosition.X;
                M.coord.Y = M.InputRecord.Event.MouseEvent.dwMousePosition.Y;
                // if clicked outside drawing area, ignore it.
                if (M.coord.Y <= rows && M.coord.X <= columns)
                {
                    board[M.coord.Y][M.coord.X].color = currentColor;
                    board[M.coord.Y][M.coord.X].bgcolor = currentBackgroundColor;
                    board[M.coord.Y][M.coord.X].symbol = 32;
                    M.gotoxy(M.coord.X, M.coord.Y);
                    cout << " ";
                }
            }// mouse 

            break;

        case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
            ;
            break;

        case FOCUS_EVENT:  // disregard focus events 

        case MENU_EVENT:   // disregard menu events 

            break;
        }
    }
}

void TextDraw::LoadFile(string filename)
{
    ifstream input(filename);
    if (!input)
        return;
    gotoxy(0, PalletForegroundColor_y - 1);
    PrintCommandOptions("[Save]  -WAIT-  [Fill]  [Reset]");
    input >> rows >> columns;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            input >> board[i][j].symbol >> board[i][j].color >> board[i][j].bgcolor;
    }
    input.close();
    gotoxy(0, PalletForegroundColor_y - 1);
    PrintCommandOptions("[Save]  [Load]  [Fill]  [Reset]  [Paste]");
    DrawShape();
    setForeGroundAndBackGroundColor(currentColor, currentBackgroundColor);   
}

void TextDraw::PrintCommandOptions(string menu)
{
    setForeGroundAndBackGroundColor(15, 0);
    cout << menu << endl;
    setForeGroundAndBackGroundColor(currentColor,currentBackgroundColor);

}

void TextDraw::PasteClipboard()
{
    gotoxy(0, 0);
    // Open the clipboard for reading
    if (!OpenClipboard(NULL)) {
        // Failed to open clipboard
        // Handle error here
    }

    // Get the clipboard data in CF_TEXT format
    HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
    if (hGlobal == NULL) {
        // Failed to get clipboard data
        // Handle error here
    }

    // Lock the memory handle to get a pointer to the data
    char* pData = (char*)GlobalLock(hGlobal);
    if (pData == NULL) {
        // Failed to lock memory handle
        // Handle error here
    }

    // Output the clipboard contents to the console
    //std::cout << pData << std::endl;

    // copy data into array 
    char* t = pData;
    int r, c;
    r = c = 0;
    while (*t != '\0')
    {
        if (*t == '\n')
        {
            r++;
            c = 0;
        }
        else
        {
            board[r][c].symbol = *t;
            board[r][c].color = currentColor;
            board[r][c].bgcolor = currentBackgroundColor;
        }
        t++;
        c++;
    }

    // Unlock the memory handle and close the clipboard
    GlobalUnlock(hGlobal);
    CloseClipboard();

    DrawShape();
}
