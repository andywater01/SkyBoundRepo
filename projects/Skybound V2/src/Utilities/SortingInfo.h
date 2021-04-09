#pragma once

class SortingInfo
{
public:

	void SetQuantity(int num) 
	{ 
		quantity = num;
	}

	int GetQuantity()
	{
		return quantity;
	}

	void SetAlphabet(char alpha)
	{
		alphabet = alpha;
	}

	char GetAlphabet()
	{
		return alphabet;
	}

	void SetColor(int col)
	{
		color = col;
	}

	int GetColor()
	{
		return color;
	}


private:
	int quantity;
	char alphabet;
	int color;

};
