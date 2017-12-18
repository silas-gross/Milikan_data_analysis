// Millikan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include "C:\root_v5.34.36\include\TH1.h"
#include "C:\root_v5.34.36\include\TLatex.h"
#include "C:\root_v5.34.36\include\TPad.h"
#include "C:\root_v5.34.36\include\TCanvas.h"
#include "C:\root_v5.34.36\include\TLegend.h"
#include "C:\root_v5.34.36\include\TString.h"
#include "C:\root_v5.34.36\include\TFile.h"
#include <math.h>
#include <string>
#include <sstream>
#define PI 3.14159265358979324
TFile* F;
void analysis(std::string fname) {
	const char* name = fname.c_str();
	int Volts = 0;
	TH1F* h = new TH1F(name, name, 200, 0, 200);
	std::cout << "How many Volts?";
	std::cin >> Volts;
	std::cout << std::endl;
	std::string fullfname = fname + ".txt";
	const char* cf = fullfname.c_str();
	ofstream errorstream;
	errorstream.open("stackcrash.txt");
	fstream tf;
	tf.open(cf);
	if (tf.is_open() == true)std::cout << "open" << std::endl;
	std::string full = "", time = "";
	float t[100000] = { 0 };
	//int label[50];
	//std::string slabel[50];
	std::string brokentime = "";
	/*for (int j = 0; j < 50; j++)
	{
		label[j] = j + 1;
		slabel[j]=;
	}*/
	int i = 0;
	while (tf.eof() == false) {
		//i++;
		//std::cout << i << std::endl;
		while(std::getline(tf, full)){
		std::stringstream tbreak;
		tbreak.str(full);
		int l = 0;
		std::string milsec = "";
		while (std::getline(tbreak, time, ' '))
		{
			//	std::cout << k << std::endl;
			
			if(time==0x20) continue;
			if (l % 3 == 0 || l % 3 == 1) {
				l++;
				continue;
			}
			std::cout << time << std::endl;
			std::stringstream hrminsec;
			hrminsec.str(time);
			int m = 0;
			while (std::getline(hrminsec, brokentime, ':'))
			{
				//std::cout << brokentime << std::endl;

				if (m % 2 == 0)
				{
					m++;
					continue;
				}
				//if (m % 2 == 1) time = brokentime;
				//if (m % 3 == 2) milsec =brokentime;
				if (brokentime == "time" || brokentime == "times")
				{
					m++;
					continue;
				}
				//std::cout << brokentime << std::endl;
				std::stringstream further;
				int n = 0;
				further.str(brokentime);
				std::string temp;
				while (std::getline(further, temp, '.'))
				{
					//std::cout << temp << std::endl;
					if (n % 2 == 0) time = temp;
					//time.erase(0, time.find_first_not_of('0'));
					//std::cout << time << std::endl;
					if (n % 2 == 1)milsec = temp;
					//std::cout << milsec << std::endl;
					n++;
				}



				if (i % 2 == 1 && i != 0) {
					errorstream << "Time: \"" << time << "\", milsec: \"" << milsec << "\"" << std::endl;
					float temp1 = std::stof(time);
					//	std::cout << temp1 << std::endl;
					float temp2 = std::stof(milsec);
					temp1 = temp1 + 0.01*temp2;
					int j = i;
					//if (j % 2 == 1) continue;
					t[j] = temp1;
					//	std::cout << j << std::endl;
				}
				m++;
				l++;
			}
			i++;
		}
		}





	double v[100], a[100], k[100], ne[100];
	float eta = 1.82*powf(10, -5);
	float rho_dif = 0.83035 - 0.01205;
	float ialpha = 4 * PI*6.02*powf(10, 23)*83.305 / 1582;
	float alpha = 1 / ialpha;
	//std::cout << alpha << std::endl;
	std::string outfname = fname + ".csv";
	const char* cof = outfname.c_str();
	ofstream outf;
	outf.open(cof);
	outf << "time , velocity , radius , ne , drag coefficent" << std::endl;
	for (int j = 0; j < 100; j++)
	{
		v[j] = 1 / t[j];
		if (t[j] == 0) continue;
		if (Volts == 0) {
			a[j] = sqrtf(0.09*v[j] * eta / (2 * 9.81*(0.83035 - 0.01205)));
			//std::cout << a[j] << std::endl;
			k[j] = 18 * PI*sqrtf(powf(eta, 3)*v[j] / (2 * 9.81*(0.83035 - 0.01205)));
			ne[j] = 0;
			outf << t[j] << ',' << v[j] << ',' << a[j] << ',' << ne[j] << ',' << k[j] << std::endl;
		}
		if (Volts == 600 && fname != "-600Vd")
		{
			ne[j] = sqrt(alpha* pow(((18 * PI*eta*v[j] * alpha*4.97*0.001) / (-4 * PI*9.81*alpha*4.97*0.001 + 3 * Volts)), 3));
			a[j] = pow(ne[j] * alpha, 1 / 3);
			outf << t[j] << ',' << v[j] << ',' << a[j] << ',' << ne[j] << ',' << k[j] << std::endl;
		}
		if(Volts!=0 ||(Volts!=600 && fname =="-600Vd"))
		{
			ne[j] = sqrt(alpha* powf(((18*PI*eta*v[j]*alpha*4.97*0.001) / (4*PI*9.81*alpha*4.97*0.001 - 3*Volts)), 3));
			a[j] = pow(ne[j] * alpha, 1 / 3);
			k[j] = 6 * PI*eta*a[j];
			outf << t[j] << ',' << v[j] << ',' << a[j] << ',' << ne[j] << ',' << k[j] << std::endl;
		}
		
		h->SetBinContent(j, ne[j]);
	}
	h->Write();

}}
int main()
{
	F = new TFile("millikan.root", "RECREATE");
	std::string mf="";
	std::cout << "More files?";
	std::cin >> mf;
	std::cout << std::endl;
	while (mf == "y") {
		std::string fname="";
		std::cout << "Input File name: ";
		std::cin >> fname;
		//std::cout << "what is up?"<<std::endl;
		analysis(fname);
		std::cout << "More files?";
		std::cin >> mf;
	}
	std::cout << "end of analysis" << std::endl;
	F->Close();
	return 0;
}

