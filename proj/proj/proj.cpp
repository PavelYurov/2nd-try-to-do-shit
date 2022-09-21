#include <iostream>
#include <conio.h>
#include <cmath>
#include <time.h>
#include <fstream>
using namespace std;


const int longness = 40;
const int height = 40;
int generation = 1;	
int timer = 0;
const int max_time_eat = 400;

//========================================================================================================================================
//===================================================/сама нейросеть/=====================================================================

const int chan = 5;
const int max_timer = 5000;
const int input_count = 8 + 4 + 4-4;
const int layers = 3;
const int output_count = 4;
const int mutation = 2;
const int gen = 64;

class traveler {
private:
	
	float input_data[input_count];
	float newrons_first[32];
	float newrons_second[16];
	float output_data[output_count];
public:
	float weight_first[input_count][32];
	float weight_col_f[32];
	float weight_col_s[16];
	float weight_col_t[output_count];
	float weight_second[32][16];
	float weight_third[16][output_count];
private:
	float sigmoid(float a) {
		float b;
		b = 1.0f / (1.0f + pow(2.71828182846,(-1*a)));
		return b;
	}
	/*float sigmoid(float a) {
		float b;
		b = 1 / tan(a);
		return b;
	}*/
	int x;
	int y;
public:
	int timesurvived = 0;
	unsigned long long int score = 0;
	int oldvect = -1;
	int newvect;
	int countter = 0;
	int ritched = 0;
	bool dead = false;
	int time_eat = 0;
	long int BIGSHOT[2];
	traveler() {

	}
	traveler(int xi, int yi, float weight_first[input_count][32],float weight_col_f[32],float weight_col_s[16],float weight_col_t[output_count],float weight_second[32][16],float weight_third[16][output_count]) {
		x = xi;
		y = yi;
		for (int i = 0; i < input_count; i++) {
			for (int u = 0; u < 32; u++) {
				this -> weight_first [i][u] = weight_first[i][u];
			}
		}
		for (int i = 0; i < 32; i++) {
			for (int u = 0; u < 16; u++) {
				this->weight_second[i][u] = weight_second[i][u];
			}
		}
		for (int i = 0; i < 16; i++) {
			for (int u = 0; u < output_count; u++) {
				this->weight_third[i][u] = weight_third[i][u];
			}
		}
		for (int i = 0; i < 32; i++) {
			this -> weight_col_f [i] = weight_col_f[i];
		}
		for (int i = 0; i < 16; i++) {
			this -> weight_col_s [i] = weight_col_s[i];
		}
		for (int i = 0; i < output_count; i++) {
			this->weight_col_t [i] = weight_col_t[i];
		}

	}
	void score_counter(int r) {
		score = pow((timer-r)*200 , (ritched + 1));
		x = -1;
		y = -1;
		timesurvived = (timer-r);
	}
	void math(int arro[input_count]) {
		for (int i = 0; i < input_count; i++) {
			input_data[i] = arro[i];
		}
		for (int i = 0; i < 32; i++) {
			newrons_first[i] = 0;
		}
		for (int i = 0; i < 16; i++) {
			newrons_second[i] = 0;
		}
		for (int i = 0; i < output_count; i++) {
			output_data[i] = 0;
		}
		for (int u = 0; u < 32; u++) {
			for (int i = 0; i < input_count; i++) {
				newrons_first[u] += input_data[i] * weight_first[i][u];
			}
			newrons_first[u] += 1 * weight_col_f[u];
			newrons_first[u] = sigmoid(newrons_first[u]);
		}
		for (int u = 0; u < 16; u++) {
			for (int i = 0; i < 32; i++) {
				newrons_second[u] += newrons_first[i] * weight_second[i][u];
			}
			newrons_second[u] += 1 * weight_col_s[u];
			newrons_second[u] = sigmoid(newrons_second[u]);
		}
		for (int u = 0; u < output_count; u++) {
			for (int i = 0; i < 16; i++) {
				output_data[u] += newrons_second[i] * weight_third[i][u];
			}
			output_data[u] += 1 * weight_col_t[u];
			output_data[u] = sigmoid(output_data[u]);
		}
	}
	int get_x() {
		return x;
	}
	void ret() {
		cout << output_data[0] << "\t" << output_data[1] << "\t" << output_data[2] << "\t" << output_data[3] << endl;
	}
	int get_y() {
		return y;
	}
	void logic() {
		if (output_data[0] > output_data[1] && output_data[0] > output_data[2] && output_data[0] > output_data[3]) {
			y++;
			newvect = 0;
		}
		else if (output_data[1] > output_data[0] && output_data[1] > output_data[2] && output_data[1] > output_data[3]) {
			y--;
			newvect = 2;
		}
		else if (output_data[2] > output_data[1] && output_data[2] > output_data[0] && output_data[2] > output_data[3]) {
			x++;
			newvect = 1;
		}
		else if (output_data[3] > output_data[1] && output_data[3] > output_data[2] && output_data[3] > output_data[0]) {
			x--;
			newvect = 3;
		}
		if (newvect == (oldvect + 2) % 4) {
			if (countter > 5) {
				dead = true;
				score_counter(0);
			}
			countter++;
		}
		else {
			countter = 0;
			
		}
		oldvect = newvect;
	}
};

traveler arr[gen];
traveler winners[2];

//=============================================================/место/===================================================


class place {
private:
	int area_[longness][height];
	int area__[longness][height][gen];
public:
	int end_X[gen];
	int end_Y[gen];
	int start_x[gen];
	int start_y[gen];
	place(int xr, int yr) {
		srand(time(NULL));
		for (int x = 0; x < longness; x++) {
			for (int y = 0; y < height; y++) {
				if ((x == longness - 1 || y == 0 || x == 0 || y == height - 1)) {
					area_[x][y] = 1;
					area__[x][y][0] = 1;
				}
				else {
					area_[x][y] = 0;
					area__[x][y][0] = 0;
				}
			}
		}
			start_x[0] = xr;
			start_y[0] = yr;
			area_[start_x[0]][start_y[0]] = 0;
			area__[start_x[0]][start_y[0]][0] = 0;
			do
			{
				end_X[0] = rand() % (longness - 2) + 1;
				end_Y[0] = rand() % (height - 2) + 1;

			} while (end_X[0] == start_x[0] || end_Y[0] == start_y[0]);
			area_[end_X[0]][end_Y[0]] = -100;
			area__[end_X[0]][end_Y[0]][0] = -100;
		
	}
	 place(int x[gen], int y [gen]) {
		srand(time(NULL));
		for (int x = 0; x < longness; x++) {
			for (int y = 0; y < height; y++) {
				if ((x == longness-1 || y == 0 || x == 0 || y == height-1)) {
					area_[x][y] = 1;
					for (int i = 0; i < gen; i++) {
						area__[x][y][i] = 1;
					}
				}
				
				/*else {
					int chance;
					chance = rand() % 100;
					if (chance < chan) {
						area_[x][y] = 1;
						for (int i = 0; i < gen; i++) {
							area__[x][y][i] = 1;
						}
					}
					else {
						area_[x][y] = 0;
						for (int i = 0; i < gen; i++) {
							area__[x][y][i] = 0;
						}
					}
				}*/
				/*else if ((x == 6 || x == longness - 6 || y == height / 2 || y == height / 2 + 1) && (y != 4 && y != 5 && y < height - 5 && x != 1 && x != longness - 2 && (x < longness / 2 - 2 || x > longness / 2 + 2))) {
					area_[x][y] = 1;
					for (int i = 0; i < gen; i++) {
						area__[x][y][i] = 1;
					}
				}*/else {
					area_[x][y] = 0;
					for (int i = 0; i < gen; i++) {
						area__[x][y][i] = 0;
					}
				}
			}
		}
		for (int i = 0; i < gen; i++) {
			start_x[i] = x[i];
			start_y[i] = y[i];
			area_[start_x[i]][start_y[i]] = 0;
			area__[start_x[i]][start_y[i]][i] = 0;
		}
		for (int i = 0; i < gen; i++) {
			end_X[i] = rand() % (longness - 2) + 1;
			end_Y[i] = rand() % (height - 2) + 1;
			if (end_X[i] == start_x[i] || end_Y[i] == start_y[i]) {
				i--;
			}
			else {
				area_[end_X[i]][end_Y[i]] = -100;
				area__[end_X[i]][end_Y[i]][i] = -100;
			}
		}
	}

	 void erase(int obj) {
		 area_[end_X[obj]][end_Y[obj]] = 0;
		 area__[end_X[obj]][end_Y[obj]][obj] = 0;
	 }


	 int find(int x, int y, int x_vect, int y_vect, int who) {
		 int S = 0;
		 int C = 0;
		 while ((C == 0 || C == -100) && S <= longness) {
			 S++;
			 C = area__[x][y][who];
			 x += x_vect;
			 y += y_vect;
		 }
		 return longness - S;
		 //return S * 10;
	 }

	 int findl(int x, int y, int x_vect, int y_vect, int who) {
		 int S = 0;
		 int C = 0;
		 while (C == 0 && S < 100) {
			 S++;
			 C = area__[x][y][who];
			 if (C == -100) {
				 return 1000;
			 }
			 x += x_vect;
			 y += y_vect;
		 }
		 return 0;
	 }

	 void logical (int obj) {
		 if (timer - arr[obj].time_eat >= max_time_eat) {
			 arr[obj].dead = true;
			 arr[obj].score_counter(max_time_eat);
		 }
		 if (arr[obj].get_x() < 0 || arr[obj].get_x() >= longness || arr[obj].get_y() < 0 || arr[obj].get_y() >= height || area__[arr[obj].get_x()][arr[obj].get_y()][obj] == 1 || (timer - arr[obj].time_eat >= max_time_eat)) {
			 arr[obj].dead = true;
			 arr[obj].score_counter(0);
		 }
		 else if (area__[arr[obj].get_x()][arr[obj].get_y()][obj] == -100) {
			 arr[obj].ritched ++;
			 erase(obj);
			 arr[obj].time_eat = timer;
			 do{
			 end_X[obj] = rand() % (longness - 2) + 1;
			 end_Y[obj] = rand() % (height - 2) + 1;
			 }while (area__[end_X[obj]][end_Y[obj]][obj] == 1);
			 area_[end_X[obj]][end_Y[obj]] = -100;
			 area__[end_X[obj]][end_Y[obj]][obj] = -100;
		 }
	 }

	void view() {
		char are[longness][height];
		
		for (int x = 0; x < longness; x++) {
			for (int y = 0; y < height; y++) {
				switch (area_[x][y])
				{
				case 0: are[x][y] = ' ';
					break;
				case 1: are[x][y] = 177;
					break;
				case -100: are[x][y] = '+';
					break;
				default: are[x][y] = 'e';
					break;
				}
			}
		}
		for (int i = 0; i < gen; i++) {
			if (arr[i].get_x() >= 0 && arr[i].get_x() < longness && arr[i].get_y() >= 0 && arr[i].get_y() < height) {
				are[arr[i].get_x()][arr[i].get_y()] = 254; //254
			}
		}
		system("cls");
		for (int x = 0; x < height; x++) {
			for (int y = 0; y < longness; y++) {
				cout << are[y][x];
			}
			cout << endl;
		}
		cout << endl << endl << generation << endl;
		/*for (int i = 0; i < 0 * gen; i++) {
			arr[i].ret();
		}*/
		
		//cout << endl << arr[0].get_x() << "\t" << arr[0].get_y();
	}
	
	
};


//===========================================================================================================================
void selection(int stx[gen], int sty[gen]) {
	srand(time(NULL));
	unsigned long long int chance = 0;
	for (int i = 0; i < gen; i++) {
		arr[i].BIGSHOT[1] = chance;
		chance += arr[i].score;
		arr[i].BIGSHOT[2] = chance - 1;
	}
	for (int i = 0; i < 2; i++) {
		int a = rand() % chance;
		for (int u = 0; u < gen; u++) {
			if (a >= arr[u].BIGSHOT[1] && a < arr[u].BIGSHOT[2]) {
				winners[i] = arr[u];
			}
		}
	}
	
		
		int a;
		float first_newrons_weight[input_count][32];
		float second_newrons_weight[32][16];
		float third_newrons_weight[16][output_count];
		float first_s_weight[32];
		float second_s_weight[16];
		float third_s_weight[output_count];

		a = rand() % input_count;
		for (int n = 0; n < a; n++) {
			for (int i = 0; i < 32; i++) {
				first_newrons_weight[n][i] = winners[0].weight_first[n][i];
			}	
		}
		for (int n = a; n < input_count; n++) {
			for (int i = 0; i < 32; i++) {
				first_newrons_weight[n][i] = winners[1].weight_first[n][i];
			}
		}
		for (int i = 0; i < 32; i++) {
			first_s_weight[i] = winners[1].weight_col_f[i];
		}
		//=============================================================
		a = rand() % 32;
		for (int n = 0; n < a; n++) {
			for (int i = 0; i < 16; i++) {
				second_newrons_weight[n][i] = winners[0].weight_second[n][i];
			}
		}
		for (int n = a; n < 32; n++) {
			for (int i = 0; i < 16; i++) {
				second_newrons_weight[n][i] = winners[1].weight_second[n][i];
			}
		}
		for (int i = 0; i <16; i++) {
			second_s_weight[i] = winners[1].weight_col_s[i];
		}
		//==============================================================
		a = rand() % 16;
		for (int n = 0; n < a; n++) {
			for (int i = 0; i < output_count; i++) {
				third_newrons_weight[n][i] = winners[0].weight_third[n][i];
			}	
		}
		for (int n = a; n < 16; n++) {
			for (int i = 0; i <output_count; i++) {
				third_newrons_weight[n][i] = winners[1].weight_third[n][i];
			}
		}
		for (int i = 0; i < output_count; i++) {
			third_s_weight[i] = winners[1].weight_col_t[i];
		}
		//=============================================================
		for (int i = 0; i < gen; i++) {
			int m;
			//================================================================
			float dfirst_newrons_weight[input_count][32];
			float dsecond_newrons_weight[32][16];
			float dthird_newrons_weight[16][output_count];
			float dfirst_s_weight[32];
			float dsecond_s_weight[16];
			float dthird_s_weight[output_count];
			//================================================================
			for (int iii = 0; iii < input_count; iii++) {
				for (int ii = 0; ii < 32; ii++) {
					dfirst_newrons_weight[iii][ii] = first_newrons_weight[iii][ii];
				}
			}
			for (int iii = 0; iii < 32; iii++) {
				for (int ii = 0; ii < 16; ii++) {
					dsecond_newrons_weight[iii][ii] = second_newrons_weight[iii][ii];
				}
			}
			for (int iii = 0; iii < 16; iii++) {
				for (int ii = 0; ii < output_count; ii++) {

					dthird_newrons_weight[iii][ii] = third_newrons_weight[iii][ii];
				}
			}
		for(int ii = 0; ii< 32; ii++)
		dfirst_s_weight[ii] = first_s_weight[ii];
		for (int ii = 0; ii < 16; ii++)
		dsecond_s_weight [ii] = second_s_weight[ii];
		for (int ii = 0; ii < output_count; ii++)
		dthird_s_weight [ii] = third_s_weight[ii];

		for (int u = 0; u < 32; u++) {
			for (int w = 0; w < input_count; w++) {
				m = rand() % 100;
				if (m < mutation) {
					dfirst_newrons_weight[w][u] = (rand() % 2001 - 1000) / 1000.0f;
				}
			}
			m = rand() % 100;
			if (m < mutation) {
				dfirst_s_weight[u] = (rand() % 2001 - 1000) / 1000.0f;
			}
		}

		for (int u = 0; u < 16; u++) {
			for (int w = 0; w < 32; w++) {
				m = rand() % 100;
				if (m < mutation) {
					dsecond_newrons_weight[w][u] = (rand() % 2001 - 1000) / 1000.0f;
				}
			}
			m = rand() % 100;
			if (m < mutation) {
				dsecond_s_weight[u] = (rand() % 2001 - 1000) / 1000.0f;
			}
		}

		for (int u = 0; u < output_count; u++) {
			for (int w = 0; w < 16; w++) {
				m = rand() % 100;
				if (m < mutation) {
					dthird_newrons_weight[w][u] = (rand() % 2001 - 1000) / 1000.0f;
				}
			}
			m = rand() % 100;
			if (m < mutation) {
				dthird_s_weight[u] = (rand() % 2001 - 1000) / 1000.0f;
			}
		}
		traveler odject(stx[i], sty[i], dfirst_newrons_weight, dfirst_s_weight, dsecond_s_weight, dthird_s_weight, dsecond_newrons_weight, dthird_newrons_weight);
		arr[i] = odject;
	}
}


//===========================================================================================================================

int best() {
	int b = 0;
	for (int i = 0; i < gen; i++) {
		if (arr[b].timesurvived < arr[i].timesurvived) {
			b = i;
		}
	}
	return b;
}
char hhh;
int main(){
	cout << "work / study/continue" << endl << "enter w or s or c" << endl;
	cin >> hhh;
	if (hhh == 's' || hhh == 'c') {
		while (!_kbhit()) {
			srand(time(NULL));
			int stx[gen];
			int sty[gen];
			for (int i = 0; i < gen; i++) {
				stx[i] = rand() % (longness - 2) + 1;
				sty[i] = rand() % (height - 2) + 1;
			}
			place areal(stx, sty);


			if (generation == 1 && hhh == 's') {
				float weight_f[input_count][32];
				float weight_s[32][16];
				float weight_t[16][output_count];
				float weight_col_ff[32];
				float weight_col_ss[16];
				float weight_col_tt[output_count];
				for (int ii = 0; ii < gen; ii++) {

					for (int i = 0; i < input_count; i++) {
						for (int u = 0; u < 32; u++) {
							weight_f[i][u] = (rand() % 2001 - 1000) / 1000.0f;
						}
					}
					for (int i = 0; i < 32; i++) {
						for (int u = 0; u < 16; u++) {
							weight_s[i][u] = (rand() % 2001 - 1000) / 1000.0f;
						}
					}
					for (int i = 0; i < 16; i++) {
						for (int u = 0; u < output_count; u++) {
							weight_t[i][u] = (rand() % 2001 - 1000) / 1000.0f;
						}
					}
					for (int i = 0; i < 32; i++) {
						weight_col_ff[i] = (rand() % 2001 - 1000) / 1000.0f;
					}
					for (int i = 0; i < 16; i++) {
						weight_col_ss[i] = (rand() % 2001 - 1000) / 1000.0f;
					}
					for (int i = 0; i < output_count; i++) {
						weight_col_tt[i] = (rand() % 2001 - 1000) / 1000.0f;
					}
					traveler entity(stx[ii], sty[ii], weight_f, weight_col_ff, weight_col_ss, weight_col_tt, weight_s, weight_t);
					arr[ii] = entity;
				}
			}
			else if (generation == 1 && hhh == 'c') {
				float w11[input_count][32];
				float w22[32][16];
				float w33[16][output_count];
				float wc11[32];
				float wc22[16];
				float wc33[output_count];
				ifstream data("D:\\8suc.txt");
				if (data.is_open()) {
					for (int i = 0; i < input_count; i++) {
						for (int ii = 0; ii < 32; ii++) {
							data >> w11[i][ii];
						}
					}
					for (int i = 0; i < 32; i++) {
						for (int ii = 0; ii < 16; ii++) {
							data >> w22[i][ii];
						}
					}
					for (int i = 0; i < 16; i++) {
						for (int ii = 0; ii < output_count; ii++) {
							data >> w33[i][ii];
						}
					}
					for (int i = 0; i < 32; i++) {
						data >> wc11[i];
					}
					for (int i = 0; i < 16; i++) {
						data >> wc22[i];
					}
					for (int i = 0; i < output_count; i++) {
						data >> wc33[i];
					}
				}
				
				for (int ww = 0; ww < gen; ww++) {
					traveler entity(stx[ww], sty[ww], w11, wc11, wc22, wc33, w22, w33);
					arr[ww] = entity;
				}
			}
			else {
				selection(stx, sty);

			}
			bool got = true;
			while (got && (timer <= max_timer)) {
				timer++;
				for (int i = 0; i < gen; i++) {
					if (!arr[i].dead) {
						int input[input_count];
						int count = 0;
						/*for (int ii = -1; ii < 2; ii++) {
							for (int uu = -1; uu < 2; uu++) {
								if (!(uu == 0) || !(ii == 0)) {
									input[count] = areal.find(arr[i].get_x(), arr[i].get_y(), ii, uu,i);
									count++;
								}
							}
						}*/
						input[count] = areal.find(arr[i].get_x(), arr[i].get_y(), 1, 0, i);
						count++;
						input[count] = areal.find(arr[i].get_x(), arr[i].get_y(), -1, 0, i);
						count++;
						input[count] = areal.find(arr[i].get_x(), arr[i].get_y(), 0, 1, i);
						count++;
						input[count] = areal.find(arr[i].get_x(), arr[i].get_y(), 0, -1, i);
						count++;
						//================================test=====================================================
						/*if ((arr[i].get_x() - areal.end_X[i] < 0) && (arr[i].get_y() - areal.end_Y[i] == 0)) {
							input[count] = 100;
						}
						else {
							input[count] = 0;
						}
						count++;
						if ((arr[i].get_x() - areal.end_X[i] > 0) && (arr[i].get_y() - areal.end_Y[i] == 0)) {
							input[count] = 100;
						}
						else {
							input[count] = 0;
						}
						count++;
						if ((arr[i].get_x() - areal.end_X[i] == 0) && (arr[i].get_y() - areal.end_Y[i] < 0)) {
							input[count] = 100;
						}
						else {
							input[count] = 0;
						}
						count++;
						if ((arr[i].get_x() - areal.end_X[i] == 0) && (arr[i].get_y() - areal.end_Y[i] > 0)) {
							input[count] = 100;
						}
						else {
							input[count] = 0;
						}
						count++;*/
						//======================================================================================
						if ((arr[i].get_x() - areal.end_X[i] < 0) && (arr[i].get_y() - areal.end_Y[i] < 0)) {
							input[count] = 100;
						}
						else {
							input[count] = 0;
						}
						count++;
						if ((arr[i].get_x() - areal.end_X[i] > 0) && (arr[i].get_y() - areal.end_Y[i] < 0)) {
							input[count] = 100;
						}
						else {
							input[count] = 0;
						}
						count++;
						if ((arr[i].get_x() - areal.end_X[i] < 0) && (arr[i].get_y() - areal.end_Y[i] > 0)) {
							input[count] = 100;
						}
						else {
							input[count] = 0;
						}
						count++; if ((arr[i].get_x() - areal.end_X[i] > 0) && (arr[i].get_y() - areal.end_Y[i] > 0)) {
							input[count] = 100;
						}
						else {
							input[count] = 0;
						}

						count++; input[count] = areal.findl(arr[i].get_x(), arr[i].get_y(), 1, 0, i);
						count++; input[count] = areal.findl(arr[i].get_x(), arr[i].get_y(), -1, 0, i);
						count++; input[count] = areal.findl(arr[i].get_x(), arr[i].get_y(), 0, 1, i);
						count++; input[count] = areal.findl(arr[i].get_x(), arr[i].get_y(), 0, -1, i);
						arr[i].math(input);
						arr[i].logic();
						areal.logical(i);

					}
				}
				areal.view();
				/*if (got == true) {
					cout << endl << "alive"  << endl;
				}*/
				int exitance = 0;
				for (int i = 0; i < gen; i++) {
					if (arr[i].dead) {
						exitance++;
						areal.erase(i);
					}
				}
				if (exitance == gen) {
					got = false;
				}
				cout << "alive" << gen - exitance << endl;
				//system("pause");
			}

			generation += 1;
			timer = 0;
		}
		for (int r = 0; r < gen; r++) {
			if (!arr[r].dead) {
				arr[r].score_counter(0);
			}
		}
		system("cls");
		cout << "save best? (y/n)" << endl;
		char hhhh;
		cin >> hhhh;
		if (hhhh == 'y') {
			ofstream data;
			data.open("D:\\newata.txt");
			if (data.is_open()) {
				for (int i = 0; i < input_count; i++) {
					for (int ii = 0; ii < 32; ii++) {
						data << arr[best()].weight_first[i][ii] << endl;
					}
				}
				for (int i = 0; i < 32; i++) {
					for (int ii = 0; ii < 16; ii++) {
						data << arr[best()].weight_second[i][ii] << endl;
					}
				}
				for (int i = 0; i < 16; i++) {
					for (int ii = 0; ii < output_count; ii++) {
						data << arr[best()].weight_third[i][ii] << endl;
					}
				}
				for (int i = 0; i < 32; i++) {
					data << arr[best()].weight_col_f[i] << endl;
				}
				for (int i = 0; i < 16; i++) {
					data << arr[best()].weight_col_s[i] << endl;
				}
				for (int i = 0; i < output_count; i++) {
					data << arr[best()].weight_col_t[i] << endl;
				}
			}
		}
	}
	if (hhh == 'w') {
		float w1[input_count][32];
		float w2[32][16];
		float w3[16][output_count];
		float wc1[32];
		float wc2[16];
		float wc3[output_count];
		ifstream data("D:\\newata.txt");
		if (data.is_open()) {
			for (int i = 0; i < input_count; i++) {
				for (int ii = 0; ii < 32; ii++) {
					data >> w1[i][ii];
				}
			}
			for (int i = 0; i < 32; i++) {
				for (int ii = 0; ii < 16; ii++) {
					data >> w2[i][ii];
				}
			}
			for (int i = 0; i < 16; i++) {
				for (int ii = 0; ii < output_count; ii++) {
					data >> w3[i][ii];
				}
			}
			for (int i = 0; i < 32; i++) {
				data >> wc1[i];
			}
			for (int i = 0; i < 16; i++) {
				data >> wc2[i];
			}
			for (int i = 0; i < output_count; i++) {
				data >> wc3[i];
			}
		}
		srand(time(NULL));
		int stxr = rand() % (longness - 2) + 1;
		int styr = rand() % (height - 2) + 1;
		place areal(stxr, styr);
		traveler obj(stxr, styr, w1, wc1, wc2, wc3, w2, w3);
		arr[0] = obj;
		while (!arr[0].dead && (timer <= max_timer)) {
			timer++;

			if (!arr[0].dead) {
				int input[input_count];
				int count = 0;
				/*for (int ii = -1; ii < 2; ii++) {
					for (int uu = -1; uu < 2; uu++) {
						if (!(uu == 0) || !(ii == 0)) {
							input[count] = areal.find(arr[i].get_x(), arr[i].get_y(), ii, uu,i);
							count++;
						}
					}
				}*/
				input[count] = areal.find(arr[0].get_x(), arr[0].get_y(), 1, 0, 0);
				count++;
				input[count] = areal.find(arr[0].get_x(), arr[0].get_y(), -1, 0, 0);
				count++;
				input[count] = areal.find(arr[0].get_x(), arr[0].get_y(), 0, 1, 0);
				count++;
				input[count] = areal.find(arr[0].get_x(), arr[0].get_y(), 0, -1, 0);
				count++;
				if ((arr[0].get_x() - areal.end_X[0] < 0) && (arr[0].get_y() - areal.end_Y[0] < 0)) {
					input[count] = 100;
				}
				else {
					input[count] = 0;
				}
				count++;
				if ((arr[0].get_x() - areal.end_X[0] > 0) && (arr[0].get_y() - areal.end_Y[0] < 0)) {
					input[count] = 100;
				}
				else {
					input[count] = 0;
				}
				count++;
				if ((arr[0].get_x() - areal.end_X[0] < 0) && (arr[0].get_y() - areal.end_Y[0] > 0)) {
					input[count] = 100;
				}
				else {
					input[count] = 0;
				}
				count++; if ((arr[0].get_x() - areal.end_X[0] > 0) && (arr[0].get_y() - areal.end_Y[0] > 0)) {
					input[count] = 100;
				}
				else {
					input[count] = 0;
				}

				count++; input[count] = areal.findl(arr[0].get_x(), arr[0].get_y(), 1, 0, 0);
				count++; input[count] = areal.findl(arr[0].get_x(), arr[0].get_y(), -1, 0, 0);
				count++; input[count] = areal.findl(arr[0].get_x(), arr[0].get_y(), 0, 1, 0);
				count++; input[count] = areal.findl(arr[0].get_x(), arr[0].get_y(), 0, -1, 0);
				arr[0].math(input);
				arr[0].logic();
				areal.logical(0);

			}

			areal.view();
			cout << endl << "take already: " << arr[0].ritched;
		}
	}
	return 0;
}