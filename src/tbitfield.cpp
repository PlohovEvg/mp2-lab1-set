// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
	{
		throw - 2;
	}

	BitLen = len;

	if (BitLen % (sizeof(int)*8) == 0)
	{
		MemLen = BitLen / (sizeof(int)*8);
	}
	else
	{
		MemLen = (BitLen / (sizeof(int)*8)) + 1;
	}
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n >= BitLen)||(n < 0))
	{
		throw - 2;
	}
	int k;

	k = n / (sizeof(int) * 8);
	
	return k;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n >= BitLen)||(n < 0))
	{
	throw - 2;
	}
	
	TELEM a = 0;

	a = a | (1 << n);

	return a;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n >= BitLen) || (n < 0))
	{
		throw - 2;
	}

	int k = this->GetMemIndex(n);
	TELEM m = this->GetMemMask(n % (sizeof(int) * 8));

	pMem[k] = pMem[k] | m;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n >= BitLen) || (n < 0))
	{
		throw - 2;
	}
	int k = this->GetMemIndex(n);
	TELEM m = this->GetMemMask(n % (sizeof(int) * 8));

	pMem[k] = pMem[k] & ~m;
}

int TBitField::GetBit(const int n) const // получить значение бита 
{
	if ((n >= BitLen) || (n < 0))
	{
		throw - 2;
	}

	int k = this->GetMemIndex(n);
  TELEM m = this->GetMemMask(n % (sizeof(int) * 8));

  if ((pMem[k] & m) == 0)
  {
	 return 0;
  }
  return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
	{
		return *this;
	}

	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int k = 0;

	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] == bf.pMem[i])
		{
			k++;
		}
	}

	if (k == MemLen)
	{
		return 1;
	}
  return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int k = 0;

	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] == bf.pMem[i])
		{
			k++;
		}
	}

	if (k == MemLen)
	{
		return 0;
	}
	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (this == &bf)
	{
		return *this;
	}

	int maxLen;

	if (BitLen > bf.BitLen)
	{
		maxLen = BitLen;
	}
	else
	{
		maxLen = bf.BitLen;
	}

	TBitField res(maxLen);
	
	for (int i = 0; i < res.MemLen; i++)
	{
		res.pMem[i] = pMem[i] | bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (this == &bf)
	{
		return *this;
	}
	int maxLen;

	if (BitLen > bf.BitLen)
	{
		maxLen = BitLen;
	}
	else
	{
		maxLen = bf.BitLen;
	}

	TBitField res(maxLen);

	for (int i = 0; i < res.MemLen; i++)
	{
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(*this);
	int i;
	for (int i = 0; i < (MemLen - 1); i++)
	{
		res.pMem[i] = ~(res.pMem[i]);
	}

	i = (MemLen - 1)*(sizeof(int)* 8);

	while (i < res.BitLen)
	{
		if (res.GetBit(i) == 0)
		{
			res.SetBit(i);
		}
		else
		{
			res.ClrBit(i);
		}
		i++;
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int k,a;
	cout << "Введите количество единичных битов" << endl;
	istr >> k;
	for (int i = 0; i < k; i++)
	{
		cout << "Введите номер бита" << endl;
		istr >> a;
		if ((a > bf.BitLen) || (a < 0))
		{
			throw - 2;
		}
		else
		{
			bf.SetBit(a);
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.MemLen; i++)
	{
		for (TELEM j = i*(sizeof(TELEM) * 8); j < (i + 1)*(sizeof(TELEM) * 8); j++)
		{
			if (bf.GetBit(j) == 1)
			{
				ostr << "Номер единичного бита:" << j << endl;
			}
		}
	}
	return ostr;
}
