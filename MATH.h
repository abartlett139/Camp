#ifndef MATH_H
#define MATH_H

#define PI ((float)3.141592654f)
#define PI2 ((float)6.283185307f)
#define PI_DIV_2 ((float)1.570796327f)
#define PI_DIV_4 ((float)0.785398163)
#define PI_INV ((float)0.318309886f)

#define FIXP16_SHIFT 16
#define FIXP16_MAG 65536
#define PIXP16_DP_MASK 0x0000ffff
#define FIXP16_WP_MASK 0Xffff000
#define FIXP16_ROUND_UP 0x00008000

#define EPSILON_E4 (float)(1E-4)
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

#define PARM_LINE_NO_INTERSECT 0
#define PARM_LINE_INTERSECT_IN_SEGMENT 1
#define PARM_LINE_INTERSECT_OUT_SEGMENT 2
#define PARM_LINE_INTERSECT_EVERYWHERE 3


namespace MATH {


	/***********************************************************************
	fpuAdd used to add two real numbers together(doubles in this case)
	requires two doubles be passed to it, inline function
	returns a double data type, and can be used as a parameter for
	another function that requires the double data type
	***********************************************************************/
	double fpuAdd(const double dAddNumOne, const double dAddNumTwo);


	/***********************************************************************
	fpuSub used to subtract two real numbers together(doubles in this case)
	requires two doubles be passed to it, inline function
	returns a double data type, and can be used as a parameter for
	another function that requires the double data type
	***********************************************************************/
	double fpuSub(const double dSubNumOne, const double dSubNumTwo);


	/***********************************************************************
	fpuDiv used to divide two real numbers together(doubles in this case)
	requires two doubles be passed to it, inline function
	returns a double data type, and can be used as a parameter for
	another function that requires the double data type
	***********************************************************************/
	double fpuDiv(const double dDivNumOne, const double dDivNumTwo);


	/***********************************************************************
	fpuMul used to multiply two real numbers together(doubles in this case)
	requires two doubles be passed to it, inline function
	returns a double data type, and can be used as a parameter for
	another function that requires the double data type
	***********************************************************************/
	double fpuMul(const double dMulNumOne, const double dMulNumTwo);


	/***********************************************************************
	fpuSquareRoot used to find the square root of a real number(double)
	requires one double be passed to it, inline function
	returns a double data type, and can be used as a parameter for
	another function that requires the double data type
	***********************************************************************/
	double fpuSquareRoot(const double dSqRt);


	/***********************************************************************
	fpuAbsVal used to find the absolute value of a real number(double)
	requires one double be passed to it, inline function
	returns a double data type, and can be used as a parameter for
	another function that requires the double data type
	***********************************************************************/
	double fpuAbsVal(const double dAbs);


	/***********************************************************************
	fpuModulus used to find the remainder of a real number(double)
	requires two numerical values be passed to it, inline function
	returns a double data type, and can be used as a parameter for
	another function that requires the double data type
	***********************************************************************/
	double fpuModulus(const double dDivide, const double dDivideBy);


	/***********************************************************************
	matrix3x3Add used to add two 3x3 matrices together
	requires eightteen doubles be passed to it
	***********************************************************************/
	void matrix3x3Add(const double dMat1R1C1, const double dMat1R1C2, const double dMat1R1C3,
		const double dMat1R2C1, const double dMat1R2C2, const double dMat1R2C3,
		const double dMat1R3C1, const double dMat1R3C2, const double dMat1R3C3,
		const double dMat2R1C1, const double dMat2R1C2, const double dMat2R1C3,
		const double dMat2R2C1, const double dMat2R2C2, const double dMat2R2C3,
		const double dMat2R3C1, const double dMat2R3C2, const double dMat2R3C3);


	/***********************************************************************
	matrix3x3Sub used to subtract two 3x3 matrices from each other
	requires eightteen doubles be passed to it
	***********************************************************************/
	void matrix3x3Sub(const double dMat1R1C1, const double dMat1R1C2, const double dMat1R1C3,
		const double dMat1R2C1, const double dMat1R2C2, const double dMat1R2C3,
		const double dMat1R3C1, const double dMat1R3C2, const double dMat1R3C3,
		const double dMat2R1C1, const double dMat2R1C2, const double dMat2R1C3,
		const double dMat2R2C1, const double dMat2R2C2, const double dMat2R2C3,
		const double dMat2R3C1, const double dMat2R3C2, const double dMat2R3C3);


	/***********************************************************************
	matrix3x3Mul used to multiply two 3x3 matrices together
	requires eightteen doubles be passed to it
	***********************************************************************/
	void matrix3x3Mul(const double dMat1R1C1, const double dMat1R1C2, const double dMat1R1C3,
		const double dMat1R2C1, const double dMat1R2C2, const double dMat1R2C3,
		const double dMat1R3C1, const double dMat1R3C2, const double dMat1R3C3,
		const double dMat2R1C1, const double dMat2R1C2, const double dMat2R1C3,
		const double dMat2R2C1, const double dMat2R2C2, const double dMat2R2C3,
		const double dMat2R3C1, const double dMat2R3C2, const double dMat2R3C3);


	/***********************************************************************
	HexToDec used to calculate Hexadecimal to Decimal
	requires one double be passed to it, returns a double data type,
	***********************************************************************/
	double HexToDec(const double dHex);



}

#endif