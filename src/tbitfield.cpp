// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen = 0;
	MemLen = 0;
	pMem = nullptr;

	if (len < 0)throw exception("len < 0");

	int size = (len + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8); //округление вверх
	TELEM* temp = new TELEM[size];

	pMem = temp;
	BitLen = len;
	MemLen = size;

	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = 0;
	MemLen = 0;
	pMem = nullptr;

	TELEM* temp = new TELEM[bf.MemLen];

	pMem = temp;
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;

	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	BitLen = 0;
	MemLen = 0;
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen))throw exception("invalid value");
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (bf != *this) {
		if (bf.MemLen != MemLen) {
			TELEM* temp = new TELEM[bf.MemLen];
			delete[] pMem;
			pMem = temp;
			MemLen = bf.MemLen;
		}
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
		BitLen = bf.BitLen;
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	int size = max(MemLen, bf.MemLen);
	bool flag = true;
	for (int i = 0; i < size; i++) {
		if ((i < MemLen ? pMem[i] : 0) != (i < bf.MemLen ? bf.pMem[i] : 0)) {
			flag = false;
			break;
		}
	}
	return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(this->operator==(bf));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(max(BitLen, bf.BitLen));
	for (int i = 0; i < res.MemLen; i++) {
		res.pMem[i] = ((i < MemLen) ? pMem[i] : 0) | ((i < bf.MemLen) ? bf.pMem[i] : 0);
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(max(BitLen, bf.BitLen));
	for (int i = 0; i < res.MemLen; i++) { //можно идти только до min(MemLen, bf.MemLen)
		res.pMem[i] = ((i < MemLen) ? pMem[i] : 0) & ((i < bf.MemLen) ? bf.pMem[i] : 0);
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = ~(this->pMem[i]);
	}
	int x = sizeof(TELEM) * 8 - (BitLen % (sizeof(TELEM) * 8));
	//возвращение в ноль инвертированных битов
	res.pMem[MemLen - 1] <<= x;
	res.pMem[MemLen - 1] >>= x;
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int size = bf.GetLength();
	char x;
	for (int i = 0; i < size; i++) {
		istr >> x;
		if (x > 0) bf.SetBit(i);
		else bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int size = bf.GetLength();
	for (int i = 0; i < size;i++) {
		ostr << bf.GetBit(i);
	}
	return ostr;
}
