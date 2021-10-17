#include "lab32.h"
#include <QREct>
#include <QHBoxLayout>
#include <QFrame>
#include <qpushbutton.h>
#include <QMessageBox>
#include <qstring.h>
#include <QVariant>
#include <QAbstractButton>
#include <fstream>
#include <QInputDialog>
#include <string>
#include <iostream>


lab32::lab32(QWidget* parent)
    : QWidget(parent), ui(new Ui::lab32Class)
{
    // ui->setupUi(this);

      //Button* btn[400];
    Grid* grid = new Grid(this);

    Button* field = new Button();
    field->setText("add field");
    grid->addWidget(field, 20, 0);
    Button* element = new Button();
    element->setText("add element");
    grid->addWidget(element, 20, 1);
    connect(field, SIGNAL(clicked()), grid, SLOT(inputField()));
    connect(element, SIGNAL(clicked()), grid, SLOT(inputElement()));
    //кнопка посчитать с функцией CreateBest
    Button* calculate = new Button();
    calculate->setText("Calculate");
    grid->addWidget(calculate, 20, 2);
    connect(calculate, SIGNAL(clicked()), grid, SLOT(GetBest()));

    Button* showElems = new Button();
    showElems->setText("Show Elements");
    grid->addWidget(showElems, 20, 3);
    connect(showElems, SIGNAL(clicked()), grid, SLOT(ShowElements()));

    setLayout(grid);
}


Button::Button()
    :QPushButton()
{ 
    used = 0;
}

void Button::changeColor()
{
    setStyleSheet("background-color: rgb(255,0,0)");
}

//------------------------------------------Window--------------------------------------------------------------------------
Window::Window(int num, Element** elems, QWidget* parent )
    :QWidget(parent), ui(new Ui::lab32Class)
{
    QGridLayout* grid = new QGridLayout(this);
    QTextEdit** text = new QTextEdit*[num];
    QString** s = new QString*[num];
    QString value;
    for (int k = 0; k < num; k++)
    {
        text[k] = new QTextEdit();
        s[k] = new QString();
        for (int i = elems[k]->top; i <= elems[k]->bot; i++)
        {
            for (int j = elems[k]->left; j <= elems[k]->right; j++)
            {
                if (elems[k]->size[i][j] == 2)
                {
                    value.setNum(elems[k]->value);
                    s[k]->append(value);
                    s[k]->append(" ");
                }
                else
                {
                    s[k]->append("0");
                    s[k]->append(" ");
                }
            }
            s[k]->append("\n");
        }
        text[k]->setText(*s[k]);
        grid->addWidget(text[k], k, 0);
    }
    

    setLayout(grid);
}


//-------------------------------------------------------------------GRID-----------------------------------------------------------------------

Grid::Grid()
    :QGridLayout()
{
    for (int k = 0; k < 400; k++)
    {
        elems[k] = new Element();
    }
    area = new Field();
    num = 0;
}

Grid::Grid(QWidget* parent)
    :QGridLayout(parent)
{
    for (int k = 0; k < 400; k++)
    {
        elems[k] = new Element();
    }
    area = new Field();
    //Field::numer = 0;
    list = new List();
    best = new Field();
    for (int i = 0; i < 400; i++)
    { 
        btns[i] = new Button();
        btns[i]->setStyleSheet("background-color: rgb(0,150,150)");
        connect(btns[i], SIGNAL(clicked()), btns[i], SLOT(changeColor()));
        addWidget(btns[i], i/20, i%20);
    }
    num = 0;
    totalLast = 0;
    text = new QTextEdit();
    text->setText("Elems1:");
    text->setText("Elems2:");

    neww = nullptr;
}

void Grid::inputField() 
{
    int ll = 21;
    int rr = -1;
    int tt = 21;
    int bb = -1;
    if (list->head == nullptr) {
        for (int i = 0; i < 400; i++)
        {
            if (btns[i]->styleSheet() == "background-color: rgb(255,0,0)")
            {
                area->field[i / 20][i % 20] = -1;
                if (ll > (i % 20)) ll = i % 20;
                if (rr < (i % 20)) rr = i % 20;
                if (tt > (i / 20)) tt = i / 20;
                if (bb < (i / 20)) bb = i / 20;
                btns[i]->setStyleSheet("background-color: rgb(155,155,0)");
                btns[i]->used = 1;
            }
        }
        
        for (int k = 0; k < 400; k++)
        {
            for (int i = 0; i < 20; i++)
            {
                for (int j = 0; j < 20; j++)
                {
                    elems[k]->size[i][j] = area->field[i][j];
                }
            }
        }
        
        list->head = area;
        area->l = ll;
        area->r = rr;
        area->t = tt;
        area->b = bb;
        area->highest = area->b;
    }

}


void Grid::inputElement()
{
    int l = 21;
    int r = -1;
    int t = 21;
    int b = -1;
    for (int i = 0; i < 400; i++)
    {
        if (btns[i]->styleSheet() == "background-color: rgb(255,0,0)")
        {
            elems[num]->size[i / 20][i % 20] = 2;
            if (btns[i]->used == 1) btns[i]->setStyleSheet("background-color: rgb(155,155,0)");
            else btns[i]->setStyleSheet("background-color: rgb(0,155,155)");
            if (l > (i % 20)) l = i % 20;
            if (r < (i % 20)) r = i % 20;
            if (t > (i / 20)) t = i / 20;
            if (b < (i / 20)) b = i / 20;
        }
    }
    elems[num]->value = QInputDialog::getInt(0, "Input", "Value:", QLineEdit::Normal);
    elems[num]->Output();
    elems[num]->top = t;
    elems[num]->left = l;
    elems[num]->right = r;
    elems[num]->bot = b;
    num++;
    //Field::numer = num;
}

bool Grid::Comp(int i, int j, int y, int x, Field* f, Element* elem, int last)
{
   
    if (j > f->r) return false;
    if (j < f->l) return false;
    if (i > f->b) return false;
    if (y < f->t) return false;

    if (f->checked[i][j] == 1) return true;

    if (elem->size[y][x] == 2)
        if (f->field[i][j] != -1) return false;
    

    f->checked[i][j] = 1;
    if ((y-1 >= elem->top))  if (Comp(i - 1, j, y - 1, x, f, elem, 1) == false) return false; ; //1
    if ( (y+1 <= elem->bot) )   if (Comp(i + 1, j, y + 1, x, f, elem, 2) == false) return false; //2
    if ( (x-1>= elem->left))  if (Comp(i, j-1,y, x-1, f, elem, 3) == false) return false; //3
    if ( (x+1 <= elem->right) )  if (Comp(i, j+1, y, x+1, f, elem, 4) == false) return false; //4

    
    return true;
}

void Grid::Fill(int i, int j, int y, int x, Field* f, Element* elem)
{
    if (j > f->r) return ;
    if (j < f->l) return ;
    if (i > f->b) return ;
    if (y < f->t) return ;

    if (f->checked[i][j] == 1) return ;
    
    if (elem->size[y][x] == 2)
    {
        if (i <= f->highest) f->highest = i - 1;
        if (f->highest < 0) f->highest = 0;
        f->field[i][j] = elem->value;
    }
    
    f->checked[i][j] = 1;
    if ((y - 1 >= elem->top))  Fill(i - 1, j, y - 1, x, f, elem);
    if ((y + 1 <= elem->bot))   Fill(i + 1, j, y + 1, x, f, elem );
    if ((x - 1 >= elem->left)) Fill(i, j - 1, y, x - 1, f, elem);
    if ((x + 1 <= elem->right)) Fill(i, j + 1, y, x + 1, f, elem);

    return;
}

bool Grid::CheckAbility(int i, int j, Element* elem, Field* f)
{
    int bl = elem->left;
    while (elem->size[elem->bot][bl] == 0) bl++;
    if (Comp(i, j, elem->bot, bl, f, elem, 0) == false)
    {
        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                f->checked[y][x] = 0;
            }
        }
        return false;
    }
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            f->checked[y][x] = 0;
        }
    }
    return true;
    /*
    int dx = elem->right - elem->left + 1;
    int dy = elem->bot - elem->top + 1;
    
    for (int y = 0; y < dy; y++)
    {       
        for (int x = 0; x < dx; x++)
        {        
            if (elem->size[elem->bot - y][elem->left + x] == 2)
            {
                if (f->field[i - y][j + x] != -1) return false;
            }
        }
    }
    return true;
    */
}

void Grid::FillInField(int i, int j, Element* elem, Field* f, int k)
{
    int bl = elem->left;
    while (elem->size[elem->bot][bl] == 0) bl++;
    Fill(i, j, elem->bot, bl, f, elem);
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            f->checked[y][x] = 0;
        }
    }
    f->cost += elem->value;
    f->used[k] = 1;
    if (f->cost > best->cost) best = f;
    /*
    if (!elem->size) return;
    int dx = elem->right - elem->left + 1;
    int dy = elem->bot - elem->top + 1;
   
    for (int y = 0; y < dy; y++)
    {
       
        for (int x = 0; x < dx; x++)
        {        
            if (elem->size[elem->bot - y][elem->left + x] == 2)
            {
                f->field[i - y ][j + x ] = elem->value;
                if (i - y  <= f->highest) if (i- y - 1 >= 0) f->highest = i - y - 1; //
                else f->highest = f->t;
            }
        }
    }


    f->cost += elem->value;   
    f->used[k] = 1;    
    */
}


void Grid::CreateBest(Field* f)
{ 
    
    Field* ptr = f;
    if (!ptr) return;
    int cnt = 0;
    int flag = 0;
    for (int k = 0; k < num; k++) //кол-во дочерних элементов 
    {
        flag = 0;
        for (int i = ptr->b; i >= ptr->highest; i--)
        {
            for (int j = ptr->l; j <= ptr->r; j++)
            {
                if (ptr->field[i][j] == -1)
                {
                    
                    if ((CheckAbility(i, j, elems[k], ptr) == true) && (ptr->CheckElementUsed(k) == false))
                    {
                        cnt++; //кол-вл детей
                        flag = 1;
                    }
                }
                if (flag == 1) break;
            }
            if (flag == 1) break;
        }

    } 
    flag = 0;
    ptr->count = cnt;
    if (cnt == 0)
    {
        f->status = 1;
        return;
    }
    
    ptr->next = new Field*[cnt] ;
    for (int i = 0; i < cnt; i++)
    {
        ptr->next[i] = new Field(ptr);
        ptr->next[i]->used = new int[num];
        for (int j = 0; j < num; j++)
        {
            ptr->next[i]->used[j] = ptr->used[j];
        }
    } 
   
    int number = 0; //кол-во дочерних
    for (int k = 0; k < num; k++)
    {
        flag = 0;
        for (int i = ptr->b; i >= ptr->highest; i--)// вставляем элемент всеми возможными способами
        {
            for (int j = ptr->l; j <= ptr->r; j++)
            {
                if (ptr->field[i][j] == -1)
                {
                    if ((CheckAbility(i, j, elems[k], ptr) == true) && (ptr->CheckElementUsed(k) == false))
                    {
                        if (number < cnt) FillInField(i, j, elems[k], ptr->next[number], k);
                        if (number < cnt) CreateBest(ptr->next[number]);
                        number++; // переходим к следующему 
                        flag = 1;
                    }                    
                }
                if (flag == 1) break;
            }
            if (flag == 1) break;
        }
    }
}

void Grid::FindBest(Field* f)
{
    
    if (f->status == 1)
    {
        if (f->cost > best->cost) best = f;
        return;
    }
    else
    for (int i = 0; i < f->count; i++)
    {
        FindBest(f->next[i]);
    }
}

void Grid::GetBest()
{
    area->used = new int[num];
    for (int i = 0; i < num; i++)
    {
        area->used[i] = 0;
    }
    CreateBest(list->head);
    //FindBest(list->head);
    //best->Output();
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (best->field[i][j] > 0)
            {
                btns[20 * i + j]->setStyleSheet("background-color: rgb(255,0,255)");
                QString s = QString::number(best->field[i][j]);
                btns[20 * i + j]->setText(s);
            }
        }
    }
    neww = new Window(num, elems);
} 

void Grid::ShowElements()
{
    neww->show();
}

//---------------------------------------------------------ELEMENT------------------------------------------------------------------------------------------------------

Element::Element()
{
    value = 0;
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            size[i][j] = 0;
        }
    }
    left = 21;
    right = -1;
    top = 21;
    bot = -1;
    space = 0;
}

Element::~Element()
{}


void Element::Output()
{
    std::ofstream outf("Output.txt");
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            outf << size[i][j];    
        }
        outf << "\n";
    }
    outf << value;
    return;
}


//---------------------------------------------------------FIELD------------------------------------------------------------

Field::Field()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            field[i][j] = 0;
            checked[i][j] = 0;
        }
    }
    
    count = 0;
    cost = 0;
    current = 0;
    used = nullptr;
    status = 0;
    highest = 19;
    l = 21;
    r = -1;
    t = 21;
    b = -1;
    next = nullptr;
}

Field::Field(Field* f)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            field[i][j] = f->field[i][j];
            checked[i][j] = 0;
        }
    }
    
    count = 0;
    cost = f->cost;
    current = f->current;
    used = nullptr;
    status = f->status;
    highest = f->highest;
    t = f->t;
    b = f->b;
    l = f->l;
    r = f->r;
    next = nullptr;
}

void Field::SetUsed(int k)
{
    used = new int[k];
}


List::List()
{
    head = nullptr;
    tails = nullptr;
}


bool Field::CheckElementUsed(int k)
{
    
    if (used[k] > 0) return true;
    return false;
}

void Field::Output()
{
    std::ofstream outf("Check.txt");
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            outf << field[i][j] << "  ";
        }
        outf << "\n";
    }
    outf << "\n";
    outf.close();
}

