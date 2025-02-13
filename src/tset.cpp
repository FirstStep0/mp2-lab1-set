// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (s.MaxPower != MaxPower) {
        return false;
    }
    return BitField.operator==(s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !operator==(s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(BitField.operator|(s.BitField));
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet b(BitField);
    b.InsElem(Elem);
    return b;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet b(BitField);
    b.DelElem(Elem);
    return b;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(BitField.operator&(s.BitField));
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(BitField.operator~());
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    TSet temp(s.GetMaxPower());
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        temp.InsElem(x);
    }
    s = temp;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    int size = s.GetMaxPower();
    for (int i = 0; i < size; i++) {
        if (s.IsMember(i))cout << i << " ";
    }
    return ostr;
}
