// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw 1;
	this->MemLen = ((len-1)/ (sizeof(TELEM)*8))+1;//Размер массива
	this->BitLen = len;
	this->pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++)
		this->pMem[i] = 0;
	
}

TBitField::TBitField(const TBitField &bf)
{
	this->MemLen = bf.MemLen;
	this->BitLen = bf.BitLen;
	this->pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++)
		this->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] this->pMem;
}

unsigned int TBitField::GetMemIndex(const int n) const
{
	if (n >= this->BitLen)
		throw 2;
	return (n/ (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const
{
	if (n >= this->BitLen)
		throw 2;
	TELEM mask = TELEM();
	mask |= 1 << n % (sizeof(TELEM) * 8);
	return mask;
}

int TBitField::GetLength(void) const
{
  return this->BitLen;
}

void TBitField::SetBit(const int n)
{
	if (n >= this->BitLen || n<0)
		throw 2;
	TELEM mask = GetMemMask(n);
	int index = GetMemIndex(n);
	this->pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) 
{
	if (n >= this->BitLen || n<0)
		throw 2;
	TELEM mask = GetMemMask(n);
	int index = GetMemIndex(n);
	this->pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const 
{
	if (n >= this->BitLen || n<0)
		throw 2;
	int index = GetMemIndex(n);
	TELEM mask = this->pMem[index] >> (n/ (sizeof(TELEM) * 8));

    return mask&1;
}

TBitField& TBitField::operator=(const TBitField &bf) 
{
	delete[] this->pMem;
	this->MemLen = bf.MemLen;
	this->BitLen = bf.BitLen;
	this->pMem = new TELEM[MemLen];
	for (size_t i = 0; i < MemLen; i++)
		this->pMem[i] = bf.pMem[i];
	
	return *this;
}

bool TBitField::operator==(const TBitField &bf) const
{
	if (this->BitLen != bf.BitLen)
		throw 2;
	bool check = true;
	for (int i = 0; i < MemLen; i++)
		if (this->pMem[i] != bf.pMem[i])
			check = false;
  return check;
}

bool TBitField::operator!=(const TBitField &bf) const 
{
	return !operator==(bf);
}

TBitField TBitField::operator|(const TBitField &bf)
{
	TBitField temp(this->BitLen);
	for (size_t i = 0; i <MemLen && i <bf.MemLen; i++)
	{
		temp.pMem[i] = this->pMem[i] | bf.pMem[i];
	}
	return temp;
}
TBitField& TBitField::operator|=(const TBitField &bf) 
{
	if (this->BitLen != bf.BitLen)
		throw 2;
	for (size_t i = 0; i < MemLen; i++)
	{
		this->pMem[i] = this->pMem[i] | bf.pMem[i];
	}
	return *this;
}

TBitField TBitField::operator&(const TBitField &bf) 
{
	TBitField temp(this->BitLen);
	for (size_t i = 0; i < MemLen && i <bf.MemLen; i++)
	{
		temp.pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return temp;
}

TBitField& TBitField::operator&=(const TBitField &bf) 
{
	if (this->BitLen != bf.BitLen)
		throw 2;
	for (size_t i = 0; i < MemLen; i++)
	{
		this->pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return *this;
}

TBitField TBitField::operator~(void) 
{
	TBitField temp(this->BitLen);
	TELEM temp_tel = temp.pMem[MemLen];
	for (size_t i = 0; i < MemLen; i++)
	{
		temp.pMem[i] = ~this->pMem[i];
	}
	
	return temp;
}

istream &operator >> (istream &istr, TBitField &bf) 
{
	if (bf.BitLen < istr.gcount())
		throw 2;
	char temp;
	int index = 0;
	while(istr.get(temp))
	{
		if (temp == '\n')
			break;
		if (temp == ' ')
			continue;
		if (temp != '1' && temp != '0')
			throw 3;
		if (temp == '1')
			bf.SetBit(index++);
		else
			bf.ClrBit(index++);
	} 
	
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (size_t i = 0; i < bf.MemLen; i++)
	{
		for (int z = (sizeof(TELEM) * 8) -1; z >=0 ; z--)
		{
			ostr << ((bf.pMem[i] & 1 << z) ? 1 : 0);			
		}
		ostr<< endl;
	}
	return ostr;
}
