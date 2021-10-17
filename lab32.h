#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_lab32.h"
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <qpushbutton.h>
#include <qgridlayout.h>
#include <QTextEdit>

class lab32 : public QWidget
{
    Q_OBJECT

public:
    lab32(QWidget *parent = Q_NULLPTR);
   
private:
    Ui::lab32Class* ui;

};





class Button : public QPushButton
{
    Q_OBJECT
public:
    Button();
    int used;
public slots:
    void changeColor();
};


class Element 
{
public:
    int size[20][20];
    int value;
    Element();
    ~Element();
    void Output();
    int left;
    int right;
    int top;
    int bot;
    int space;
    void GetLRTBS();

};

class Field  : public Element
{
public:
   // static int numer;
    int field[20][20];
    Field** next;
    Field();
    Field(Field* f);
    int current;
    int count;
    int cost;
    int* used;
    int checked[20][20];
    int status;
    int highest;
    int l;
    int r;
    int t;
    int b;
    //Field* best;
   // Field* FillInField(int i, int j, Element* elem, int k);
    void SetUsed(int num);
    bool CheckElementUsed(int k);
    void Output();
};

class List : public Field
{
public:
    Field* head;
    Field** tails;
    List();
    ~List();
};


class Window : public QWidget
{
    Q_OBJECT

public:
    Window(int num, Element** elems, QWidget* parent = Q_NULLPTR);

private:
    Ui::lab32Class* ui;
};

class Grid : public QGridLayout
{
    Q_OBJECT
    
public:
    Window* neww;
    Button* btns[400];
    QTextEdit* text;
    Grid();
    Grid(QWidget* parent);
    Field* area;
    Element* elems[400];
    //void SortElems();
    int num;
    List* list;
    Field* best;
    //void WriteElems();
    bool Comp(int i, int j, int y, int x, Field*f, Element* elem, int last);
    void Fill(int i, int j, int y, int x, Field* f, Element* elem);
    void CreateBest(Field* f); //память
    void FindBest(Field* f);
    bool CheckAbility(int i, int j, Element* elem, Field* f);
    int totalLast;
    void FillInField(int i, int j, Element* elem, Field* f, int k);
public slots:
    void inputField();
    void inputElement();
    void GetBest();
    void ShowElements();
};

