#include "MATH.h"

double dResult[3][3];

double MATH::fpuAdd(const double dAddNumOne, const double dAddNumTwo)
{
	double dAddResult = 0;
	_asm {
		FLD dAddNumOne
		FADD dAddNumTwo
		FSTP dAddResult
	}
	return dAddResult;
}

double MATH::fpuSub(const double dSubNumOne, const double dSubNumTwo)
{
	double dSubResult = 0;
	_asm {
		FLD dSubNumOne
		FSUB dSubNumTwo
		FSTP dSubResult
	}
	return dSubResult;
}

double MATH::fpuDiv(const double dDivNumOne, const double dDivNumTwo)
{
	double dDivResult = 0;
	_asm {
		FLD dDivNumOne
		FDIV dDivNumTwo
		FSTP dDivResult
	}
	return dDivResult;
}

double MATH::fpuMul(const double dMulNumOne, const double dMulNumTwo)
{
	double dMulResult = 0;
	_asm {
		FLD dMulNumOne
		FMUL dMulNumTwo
		FSTP dMulResult
	}
	return dMulResult;
}

double MATH::fpuSquareRoot(const double dSqRt)
{
	double dSquareRootResult = 0;
	_asm {
		FLD dSqRt
		FSQRT
		FSTP dSquareRootResult
	}
	return dSquareRootResult;
}

double MATH::fpuAbsVal(const double dAbs)
{
	double dAbsValResult = 0;
	_asm {
		FLD dAbs
		FABS
		FSTP dAbsValResult
	}
	return dAbsValResult;
}

double MATH::fpuModulus(const double dDivide, const double dDivideBy)
{
	double dModulusResult = 0;
	_asm {
		FLD dDivideBy
		FLD dDivide
		FPREM
		FSTP dModulusResult
	}
	return dModulusResult;
}

void MATH::matrix3x3Add(const double dMat1R1C1, const double dMat1R1C2, const double dMat1R1C3, const double dMat1R2C1, const double dMat1R2C2, const double dMat1R2C3, const double dMat1R3C1, const double dMat1R3C2, const double dMat1R3C3, const double dMat2R1C1, const double dMat2R1C2, const double dMat2R1C3, const double dMat2R2C1, const double dMat2R2C2, const double dMat2R2C3, const double dMat2R3C1, const double dMat2R3C2, const double dMat2R3C3)
{
	dResult[0][0] = fpuAdd(dMat1R1C1, dMat2R1C1);
	dResult[0][1] = fpuAdd(dMat1R1C2, dMat2R1C2);
	dResult[0][2] = fpuAdd(dMat1R1C3, dMat2R1C3);

	dResult[1][0] = fpuAdd(dMat1R2C1, dMat2R2C1);
	dResult[1][1] = fpuAdd(dMat1R2C2, dMat2R2C2);
	dResult[1][2] = fpuAdd(dMat1R2C3, dMat2R2C3);

	dResult[2][0] = fpuAdd(dMat1R3C1, dMat2R3C1);
	dResult[2][1] = fpuAdd(dMat1R3C2, dMat2R3C2);
	dResult[2][2] = fpuAdd(dMat1R3C3, dMat2R3C3);
}

void MATH::matrix3x3Sub(const double dMat1R1C1, const double dMat1R1C2, const double dMat1R1C3, const double dMat1R2C1, const double dMat1R2C2, const double dMat1R2C3, const double dMat1R3C1, const double dMat1R3C2, const double dMat1R3C3, const double dMat2R1C1, const double dMat2R1C2, const double dMat2R1C3, const double dMat2R2C1, const double dMat2R2C2, const double dMat2R2C3, const double dMat2R3C1, const double dMat2R3C2, const double dMat2R3C3)
{
	dResult[0][0] = fpuSub(dMat1R1C1, dMat2R1C1);
	dResult[0][1] = fpuSub(dMat1R1C2, dMat2R1C2);
	dResult[0][2] = fpuSub(dMat1R1C3, dMat2R1C3);

	dResult[1][0] = fpuSub(dMat1R2C1, dMat2R2C1);
	dResult[1][1] = fpuSub(dMat1R2C2, dMat2R2C2);
	dResult[1][2] = fpuSub(dMat1R2C3, dMat2R2C3);

	dResult[2][0] = fpuSub(dMat1R3C1, dMat2R3C1);
	dResult[2][1] = fpuSub(dMat1R3C2, dMat2R3C2);
	dResult[2][2] = fpuSub(dMat1R3C3, dMat2R3C3);
}

void MATH::matrix3x3Mul(const double dMat1R1C1, const double dMat1R1C2, const double dMat1R1C3, const double dMat1R2C1, const double dMat1R2C2, const double dMat1R2C3, const double dMat1R3C1, const double dMat1R3C2, const double dMat1R3C3, const double dMat2R1C1, const double dMat2R1C2, const double dMat2R1C3, const double dMat2R2C1, const double dMat2R2C2, const double dMat2R2C3, const double dMat2R3C1, const double dMat2R3C2, const double dMat2R3C3)
{
	double tempResult[4];

	tempResult[0] = fpuMul(dMat1R1C1, dMat2R1C1);
	tempResult[1] = fpuMul(dMat1R1C2, dMat2R2C1);
	tempResult[2] = fpuMul(dMat1R1C3, dMat2R3C1);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[0][0] = fpuAdd(tempResult[2], tempResult[3]);

	tempResult[0] = fpuMul(dMat1R1C1, dMat2R1C2);
	tempResult[1] = fpuMul(dMat1R1C2, dMat2R2C2);
	tempResult[2] = fpuMul(dMat1R1C3, dMat2R3C2);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[0][1] = fpuAdd(tempResult[2], tempResult[3]);

	tempResult[0] = fpuMul(dMat1R1C1, dMat2R1C3);
	tempResult[1] = fpuMul(dMat1R1C2, dMat2R2C3);
	tempResult[2] = fpuMul(dMat1R1C3, dMat2R3C3);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[0][2] = fpuAdd(tempResult[2], tempResult[3]);

	tempResult[0] = fpuMul(dMat1R2C1, dMat2R1C1);
	tempResult[1] = fpuMul(dMat1R2C2, dMat2R2C1);
	tempResult[2] = fpuMul(dMat1R2C3, dMat2R3C1);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[1][0] = fpuAdd(tempResult[2], tempResult[3]);

	tempResult[0] = fpuMul(dMat1R2C1, dMat2R1C2);
	tempResult[1] = fpuMul(dMat1R2C2, dMat2R2C2);
	tempResult[2] = fpuMul(dMat1R2C3, dMat2R3C2);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[1][1] = fpuAdd(tempResult[2], tempResult[3]);

	tempResult[0] = fpuMul(dMat1R2C1, dMat2R1C3);
	tempResult[1] = fpuMul(dMat1R2C2, dMat2R2C3);
	tempResult[2] = fpuMul(dMat1R2C3, dMat2R3C3);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[1][2] = fpuAdd(tempResult[2], tempResult[3]);

	tempResult[0] = fpuMul(dMat1R3C1, dMat2R1C1);
	tempResult[1] = fpuMul(dMat1R3C2, dMat2R2C1);
	tempResult[2] = fpuMul(dMat1R3C3, dMat2R3C1);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[2][0] = fpuAdd(tempResult[2], tempResult[3]);

	tempResult[0] = fpuMul(dMat1R3C1, dMat2R1C2);
	tempResult[1] = fpuMul(dMat1R3C2, dMat2R2C2);
	tempResult[2] = fpuMul(dMat1R3C3, dMat2R3C2);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[2][1] = fpuAdd(tempResult[2], tempResult[3]);

	tempResult[0] = fpuMul(dMat1R3C1, dMat2R1C3);
	tempResult[1] = fpuMul(dMat1R3C2, dMat2R2C3);
	tempResult[2] = fpuMul(dMat1R3C3, dMat2R3C3);
	tempResult[3] = fpuAdd(tempResult[0], tempResult[1]);
	dResult[2][2] = fpuAdd(tempResult[2], tempResult[3]);
}

double MATH::HexToDec(const double dHex)
{
	double dHexToDecResult = 0;

	return dHexToDecResult;
}
