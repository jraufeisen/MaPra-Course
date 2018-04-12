/***********************************************************
*  Name       : meina1.cpp                                 *
*  Verwendung : Loesung der ersten Praktikumsaufgabe,      *
*               Nullstellen eines quadratischen Polynoms   *
*  Autor      :                                            *
*  Datum      :                                            *
*  Sprache    : C++                                        *
***********************************************************/

// Einbinden der Praktikums-Umgebung. Falls Sie die Ein-/Ausgabe zuerst
// nicht ueber die Praktikumsumgebung machen wollen, sollten Sie auch noch
// #include <iostream> einbinden.

#include "unit.h"
#include <iostream>

using namespace std;

// ===== Hauptprogramm =====

// represents a Polynomial f(x) = ax^2 + bx + c with a, b, c in R
class Polynomial {
	public:
		Polynomial(double a, double b, double c);
		bool isConstant();
		bool isLinear();	
		bool isZeroPolynomial();
		double a, b, c;
};

Polynomial::Polynomial(double a, double b, double c){
	this -> a = a;
	this -> b = b;
	this -> c = c;
}

bool Polynomial::isConstant(){
	return a == 0 && b == 0;
}

bool Polynomial::isLinear(){
	return a == 0;
}

bool Polynomial::isZeroPolynomial(){
	return isConstant() && c == 0;
}



void calculateZeros(Polynomial &poly){
	if(poly.isConstant()){
		if(poly.isZeroPolynomial()) cout << "Infinite many zeros." << endl;
		else cout << "no zeros." << endl;
        Ergebnis(0);
	}
	else if(poly.isLinear()){
		// there is only one zero
		double zero = -poly.c / poly.b;
		cout << "one zero: " << zero << endl;
        Ergebnis(1,false,zero)
	}
	else { 
		// .. it should be quadratic
		double p = poly.b / poly.a;
		double q = poly.c / poly.a;
		double pHalf = p / 2;
		
		
		double pqRoot;
		// if p big number, factorise abs(p) to avoid overflow
		// TODO: maybe something greater than 1?
		if(abs(p) <= 1){
			pqRoot = sqrt((pHalf * pHalf) - q);
		}
		else {
			pqRoot = abs(p) * sqrt((1/4) - (q/p)/p);
		}
			

		double zero1, zero2;
		
		// use Vieta to avoid cancellation
		// TODO: there is a problem if there is a "zero zero"
		if(p > 0){
			zero2 = -pHalf - pqRoot;
			zero1 = q / zero2;
		}
		else {
			zero1 = -pHalf + pqRoot;
			zero2 = q / zero1;
		}
		cout << "two zeros. zero1: " << zero1 << ", zero2: " << zero2;
	}
}


int main(){
/*
	int x = 5;
	double a, b, c;
	Start(1, a, b, c);
	cout << a << ", " << b << ", " << c << endl; */

    
	Polynomial p(4, 0, 0);
	calculateZeros(p);
	return 0;
	
	// TODO: call unit.Ergebnis() 
}
