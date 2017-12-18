// Millikan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <cstdio>
/*#include "C:\root_v5.34.36\include\TH1.h"
#include "C:\root_v5.34.36\include\TLatex.h"
#include "C:\root_v5.34.36\include\TPad.h"
#include "C:\root_v5.34.36\include\TCanvas.h"
#include "C:\root_v5.34.36\include\TLegend.h"
#include "C:\root_v5.34.36\include\TString.h"
#include "C:\root_v5.34.36\include\TFile.h"
*/
#include <math.h>
#include <string>
#include <sstream>
#define PI 3.14159265358979324
//TFile* F;
void analysis(std::string fname) {
	const char* name = fname.c_str();
	std::string vs = fname + "_velocity";
	std::string as = fname + "_radius";
	std::string ks = fname + "_drag_coefficent";
	const char* vname = vs.c_str();
	const char* aname = as.c_str();
	const char* kname = ks.c_str();
	int Volts = 0;
	/*TH1F* h = new TH1F(name, name, 200, 0, 200);
	TH1F* h1 = new TH1F(vname, vname, 20, 0, 1);
	TH1F* h2 = new TH1F(aname, aname, 20, 0, 0.0003);
	TH1F* h3 = new TH1F(kname, kname, 20, 0, 0.000001);
	
	*/
	std::cout << "How many Volts?";
	std::cin >> Volts;
	std::cout << std::endl;
	float Volt_true = 1000000000 * Volts;
	std::string fullfname = fname + ".txt";
	const char* cf = fullfname.c_str();
	ofstream errorstream;
	errorstream.open("stackcrash.txt");
	fstream tf;
	tf.open(cf);
	if (tf.is_open() == true)std::cout << "open" << std::endl;
	std::string full = "", time = "", sec="";
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
		while (std::getline(tf, full)) {
			std::stringstream tbreak;
			tbreak.str(full);
			int l = 0;
			std::string milsec = "";
			while (std::getline(tbreak, time, ' '))
			{
				//	std::cout << k << std::endl;

				//if(time==" ") continue;
				if (l % 3 != 0) {
					l++;
					continue;
				}
				std::cout << time << std::endl;

				int m = 0;
			
					
					if (time.compare("Total") == 0 ||time.compare("time")==0 ) continue;
					std::cout << time << std::endl;
					std::stringstream full;
					full.str(time);
					while (std::getline(full, brokentime, ':')) {
						int n = brokentime.find(".");
						std::cout << n << std::endl;
						if (n == 0 || n == 1 ||n==-1)continue;
						sec = brokentime.substr(n - 2, 2);
						std::cout << "sec loaded" << std::endl;
						milsec = brokentime.substr(n + 1, 2);
						l++;

						if (i % 2 == 1 && i != 0) {
							errorstream << "Time: \"" << sec << "\", milsec: \"" << milsec << "\"" << std::endl;
							if (milsec.compare("") == 0 || time.compare("time") == 0) continue;
							float temp1 = std::stof(sec);
							std::cout << temp1 << std::endl;
							float temp2 = std::stof(milsec);
							temp1 = temp1 + 0.01*temp2;
							int j = i;
							//if (j % 2 == 1) continue;
							t[j] = temp1;
							//	std::cout << j << std::endl;

						}

						i++;
					}
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
				ne[j] = sqrt(alpha* pow(((18 * PI*eta*v[j] * alpha*4.97*0.001) / (-4 * PI*9.81*alpha*4.97*0.001 + 3 * Volt_true)), 3));
				a[j] = pow(ne[j] * alpha, 1 / 3);
				outf << t[j] << ',' << v[j] << ',' << a[j] << ',' << ne[j] << ',' << k[j] << std::endl;
			}
			if (Volts != 0 || (Volts != 600 && fname == "-600Vd"))
			{
				ne[j] = sqrt(alpha* powf(((18 * PI*eta*v[j] * alpha*4.97*0.001) / (4 * PI*9.81*alpha*4.97*0.001 - 3 * Volt_true)), 3));
				a[j] = pow(ne[j] * alpha, 1 / 3);
				k[j] = 6 * PI*eta*a[j];
				outf << t[j] << ',' << v[j] << ',' << a[j] << ',' << ne[j] << ',' << k[j] << std::endl;
			}

	//		h->SetBinContent(j, ne[j]);
	//		h1->Fill(v[j]);
	//		h2->Fill( a[j]);
	//		h3->Fill(k[j]);
		}
	}
//	h->Write();
//	h1->Write();
	//h2->Write();
//	h3->Write();

}
int main()
{
//	F = new TFile("millikan.root", "RECREATE");
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

