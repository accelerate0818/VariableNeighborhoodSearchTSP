////////////////////////
//TSP���� ����������������
//����Berlin52�������
//���ߣ�infinitor
//ʱ�䣺2018-04-12
////////////////////////


#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <windows.h>
#include <memory.h>
#include <string.h>
#include <iomanip>
#include <algorithm> 
#define DEBUG

using namespace std;

#define CITY_SIZE 52 //��������


//��������
typedef struct candidate
{
	int x;
	int y;
}city, CITIES;

//�������
typedef struct Solution
{
	int permutation[CITY_SIZE]; //��������
	int cost;						 //�����ж�Ӧ����·�߳���
}SOLUTION;

//��������
int permutation[CITY_SIZE];
//������������
CITIES cities[CITY_SIZE];


//berlin52�������꣬���Ž�7542����
CITIES berlin52[CITY_SIZE] =
{ 
{ 565,575 },{ 25,185 },{ 345,750 },{ 945,685 },{ 845,655 },
{ 880,660 },{ 25,230 },{ 525,1000 },{ 580,1175 },{ 650,1130 },{ 1605,620 },
{ 1220,580 },{ 1465,200 },{ 1530,5 },{ 845,680 },{ 725,370 },{ 145,665 },
{ 415,635 },{ 510,875 },{ 560,365 },{ 300,465 },{ 520,585 },{ 480,415 },
{ 835,625 },{ 975,580 },{ 1215,245 },{ 1320,315 },{ 1250,400 },{ 660,180 },
{ 410,250 },{ 420,555 },{ 575,665 },{ 1150,1160 },{ 700,580 },{ 685,595 },
{ 685,610 },{ 770,610 },{ 795,645 },{ 720,635 },{ 760,650 },{ 475,960 },
{ 95,260 },{ 875,920 },{ 700,500 },{ 555,815 },{ 830,485 },{ 1170,65 },
{ 830,610 },{ 605,625 },{ 595,360 },{ 1340,725 },{ 1740,245 } 
};
//�Ż�ֵ
int Delta1[CITY_SIZE][CITY_SIZE] = { 0 };


//�����������м����
int distance_2city(city c1, city c2)
{
	int distance = 0;
	distance = sqrt((double)((c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y)));

	return distance;
}

//���ݲ����ĳ������У����������ܾ���
//��ν�������У����ǳ����Ⱥ���ʵ�˳�򣬱�������ȷ���ABC��Ҳ�����ȷ���BAC�ȵ�
//����˳��ͬ����ô��·�߳���Ҳ�ǲ�ͬ��
//p_perm �������в���
int cost_total(int * cities_permutation, CITIES * cities)
{
	int total_distance = 0;
	int c1, c2;
	//��һȦ�����������ܾ����Ƕ���
	for (int i = 0; i < CITY_SIZE; i++)
	{
		c1 = cities_permutation[i];
		if (i == CITY_SIZE - 1) //���һ�����к͵�һ�����м������
		{
			c2 = cities_permutation[0];
		}
		else
		{
			c2 = cities_permutation[i + 1];
		}
		total_distance += distance_2city(cities[c1], cities[c2]);
	}

	return total_distance;
}

//��ȡ�����������
void random_permutation(int * cities_permutation)
{
	int i, r, temp;
	for (i = 0; i < CITY_SIZE; i++)
	{
		cities_permutation[i] = i; //��ʼ���������У���ʼ��˳����
	}
	
	random_shuffle(cities_permutation, cities_permutation + CITY_SIZE); //��������� 

}
//��Ӧtwo_opt_swap��ȥ��
int calc_delta1(int i, int k, int *tmp, CITIES * cities) {
	int delta = 0;
	/*
	���¼���˵����
	����ÿ����������ת�Ժ�û��Ҫ�ٴ����¼����ܾ���
	ֻ��Ҫ�ڷ�ת��ͷβ����СС����

	���磺
	�г�������   1-2-3-4-5 �ܾ��� = d12 + d23 + d34 + d45 + d51 = A
	��ת������� 1-4-3-2-5 �ܾ��� = d14 + d43 + d32 + d25 + d51 = B
	���� dij �� dji��һ���ģ�����BҲ���Ա�ʾ�� B = A - d12 - d45 + d14 + d25
	������Ż����ǻ�������ԭ��
	*/
	if (i == 0)
	{
		if (k == CITY_SIZE - 1)
		{
			delta = 0;
		}
		else
		{
			delta = 0
				- distance_2city(cities[tmp[k]], cities[tmp[k + 1]])
				+ distance_2city(cities[tmp[i]], cities[tmp[k + 1]])
				- distance_2city(cities[tmp[CITY_SIZE - 1]], cities[tmp[i]])
				+ distance_2city(cities[tmp[CITY_SIZE - 1]], cities[tmp[k]]);
		}

	}
	else
	{
		if (k == CITY_SIZE - 1)
		{
			delta = 0
				- distance_2city(cities[tmp[i - 1]], cities[tmp[i]])
				+ distance_2city(cities[tmp[i - 1]], cities[tmp[k]])
				- distance_2city(cities[tmp[0]], cities[tmp[k]])
				+ distance_2city(cities[tmp[i]], cities[tmp[0]]);
		}
		else
		{
			delta = 0
				- distance_2city(cities[tmp[i - 1]], cities[tmp[i]])
				+ distance_2city(cities[tmp[i - 1]], cities[tmp[k]])
				- distance_2city(cities[tmp[k]], cities[tmp[k + 1]])
				+ distance_2city(cities[tmp[i]], cities[tmp[k + 1]]);
		}
	}

	return delta;
}


/*
ȥ�ش�������Delta������˵�����ڳ�������1-2-3-4-5-6-7-8-9-10�������3-5Ӧ�����������2-opt �� ��ʵ�϶���
7-10֮��ķ�ת�ǲ���Ҫ�ظ�����ġ� ������Delta��ǰԤ����һ�¡�

��Ȼ��������ļ��㱾����O��1�� �ģ���ʵ�ϲ�û�д���ʱ�临�Ӷȵļ��٣����²������������˸��Ӷȣ�
���delta���� ��O��n���ģ�����ȥ�ز���Ч�������Եġ�
*/
//��Ӧtwo_opt_swap��ȥ�ظ���
void Update1(int i, int k, int *tmp, CITIES * cities, int Delta[CITY_SIZE][CITY_SIZE]) {
	if (i && k != CITY_SIZE - 1) {
		i--; k++;
		for (int j = i; j <= k; j++) {
			for (int l = j + 1; l < CITY_SIZE; l++) {
				Delta[j][l] = calc_delta1(j, l, tmp, cities);
			}
		}

		for (int j = 0; j < k; j++) {
			for (int l = i; l <= k; l++) {
				if (j >= l) continue;
				Delta[j][l] = calc_delta1(j, l, tmp, cities);
			}
		}
	}// ������Ǳ߽磬����(i-1, k + 1)֮��� 
	else {
		for (i = 0; i < CITY_SIZE - 1; i++)
		{
			for (k = i + 1; k < CITY_SIZE; k++)
			{
				Delta[i][k] = calc_delta1(i, k, tmp, cities);
			}
		}
	}// �߽�Ҫ������� 

}


// two_opt_swap���� 
void two_opt_swap(int *cities_permutation, int b, int c) 
{
	vector<int> v;
	for (int i = 0; i < b; i++) 
	{
		v.push_back(cities_permutation[i]);
	}
	for (int i = c; i >= b; i--) 
	{
		v.push_back(cities_permutation[i]);
	}
	for (int i = c + 1; i < CITY_SIZE; i++) 
	{
		v.push_back(cities_permutation[i]);
	}

	for (int i = 0; i < CITY_SIZE; i++)
	{
		cities_permutation[i] = v[i];
	}

}

//����ṹ1 ʹ��two_opt_swap����
void neighborhood_one(SOLUTION & solution, CITIES *cities)
{
	int i, k, count = 0;
	int max_no_improve = 60;

	int inital_cost = solution.cost; //��ʼ����
	int now_cost = 0;

	//SOLUTION current_solution = solution;

	for (int i = 0; i < CITY_SIZE - 1; i++)
	{
		for (k = i + 1; k < CITY_SIZE; k++)
		{
			Delta1[i][k] = calc_delta1(i, k, solution.permutation, cities);
		}
	}

	do 
	{
		count++;
		for (i = 0; i < CITY_SIZE - 1; i++)
		{
			for (k = i + 1; k < CITY_SIZE; k++)
			{
				if (Delta1[i][k] < 0)
				{
					//current_solution = solution;
					two_opt_swap(solution.permutation, i, k);

					now_cost = inital_cost + Delta1[i][k];
					solution.cost = now_cost;

					inital_cost = solution.cost;

					Update1(i, k, solution.permutation, cities, Delta1);

					count = 0; //count��λ
					
				}

			 }
		  }
	}while (count <= max_no_improve);

}

//two_h_opt_swap��ȥ��
int calc_delta2(int i, int k, int *cities_permutation, CITIES * cities)
{
	int delta = 0;
	if (i == 0)
	{
		if ( k == i+1)
		{
			delta = 0;
		}
		else if ( k == CITY_SIZE -1)
		{
			delta = 0
				- distance_2city(cities[cities_permutation[i]], cities[cities_permutation[i + 1]])
				- distance_2city(cities[cities_permutation[k]], cities[cities_permutation[k - 1]])
				+ distance_2city(cities[cities_permutation[k]], cities[cities_permutation[i+1]])
				+ distance_2city(cities[cities_permutation[k - 1]], cities[cities_permutation[i]]);
		}
		else
		{
			delta = 0
				- distance_2city(cities[cities_permutation[i]], cities[cities_permutation[i + 1]])
				- distance_2city(cities[cities_permutation[k]], cities[cities_permutation[k - 1]])
				- distance_2city(cities[cities_permutation[k]], cities[cities_permutation[k + 1]])
				+ distance_2city(cities[cities_permutation[k - 1]], cities[cities_permutation[k + 1]])
				+ distance_2city(cities[cities_permutation[i]], cities[cities_permutation[k]])
				+ distance_2city(cities[cities_permutation[k]], cities[cities_permutation[i + 1]]);
		}
	}
	else
	{
		if (k == i + 1)
		{
			delta = 0;
		}
		else if ( k == CITY_SIZE - 1)
		{
			delta = 0
				- distance_2city(cities[cities_permutation[i]], cities[cities_permutation[i + 1]])
				- distance_2city(cities[cities_permutation[0]], cities[cities_permutation[k]])
				- distance_2city(cities[cities_permutation[k]], cities[cities_permutation[k-1]])
				+ distance_2city(cities[cities_permutation[k]], cities[cities_permutation[i + 1]])
				+ distance_2city(cities[cities_permutation[k-1]], cities[cities_permutation[0]])
				+ distance_2city(cities[cities_permutation[i]], cities[cities_permutation[k]]);
		}
		else
		{
			delta = 0
				- distance_2city(cities[cities_permutation[i]], cities[cities_permutation[i + 1]])
				- distance_2city(cities[cities_permutation[k]], cities[cities_permutation[k + 1]])
				- distance_2city(cities[cities_permutation[k]], cities[cities_permutation[k - 1]])
				+ distance_2city(cities[cities_permutation[i]], cities[cities_permutation[k]])
				+ distance_2city(cities[cities_permutation[k]], cities[cities_permutation[i + 1]])
				+ distance_2city(cities[cities_permutation[k - 1]], cities[cities_permutation[k + 1]]);

		}
	}

	return delta;

}



//two_h_opt_swap����
void two_h_opt_swap(int *cities_permutation, int a, int d) 
{
	int n = CITY_SIZE;
	vector<int> v;
	v.push_back(cities_permutation[a]);
	v.push_back(cities_permutation[d]);
	// i = 1 to account for a already added
	for (int i = 1; i < n; i++) 
	{
		int idx = (a + i) % n;
		// Ignore d which has been added already
		if (idx != d) 
		{
			v.push_back(cities_permutation[idx]);
		}
	}

	for (int i = 0; i < v.size(); i++)
	{
		cities_permutation[i] = v[i];
	}

}


//����ṹ2 ʹ��two_h_opt_swap����
void neighborhood_two(SOLUTION & solution, CITIES *cities)
{
	int i, k, count = 0;
	int max_no_improve = 60;
	int inital_cost = solution.cost; //��ʼ����
	int now_cost = 0;
	int delta = 0;

	do
	{
		count++;
		for (i = 0; i < CITY_SIZE - 1; i++)
		{
			for (k = i + 1; k < CITY_SIZE; k++)
			{
				
				delta = calc_delta2(i, k, solution.permutation, cities);

				if (delta < 0)
				{
					//cout<<"delta = " <<delta<<endl; 

					two_h_opt_swap(solution.permutation, i, k);

					now_cost = inital_cost + delta;
					solution.cost = now_cost;

					inital_cost = solution.cost;

					count = 0; //count��λ
				}
			}
		}
	} while (count <= max_no_improve);
}


//VND
//best_solution���Ž�
//current_solution��ǰ��
void variable_neighborhood_descent(SOLUTION & solution, CITIES * cities)
{

	SOLUTION current_solution = solution;
	int l = 1;
	cout  <<"=====================VariableNeighborhoodDescent=====================" << endl;
	while(true)
	{
		switch (l)
		{
		case 1:
			neighborhood_one(current_solution, cities);
			cout << setw(45) << setiosflags(ios::left)  <<"Now in neighborhood_one , current_solution = " << current_solution.cost << setw(10) << setiosflags(ios::left) << "  solution = " << solution.cost << endl;
			if (current_solution.cost < solution.cost)
			{
				solution = current_solution;
				l = 0;
			}
			break;
		case 2:
			neighborhood_two(current_solution, cities);
			cout << setw(45) << setiosflags(ios::left) << "Now in neighborhood_two , current_solution = " << current_solution.cost << setw(10) << setiosflags(ios::left) << "  solution = " << solution.cost << endl;
			if (current_solution.cost < solution.cost)
			{
				solution = current_solution;
				l = 0;
			}
			break;

		default:
			return;
		}
		l++;

	}

}

//���������зֳ�4�飬Ȼ�󰴿����´���˳��
//�����Ŷ�����
void double_bridge_move(int * cities_permutation)
{
	int pos1 = 1 + rand() % (CITY_SIZE / 4);
	int pos2 = pos1 + 1 + rand() % (CITY_SIZE / 4);
	int pos3 = pos2 + 1 + rand() % (CITY_SIZE / 4);

	int i;
	vector<int> v;
	//��һ��
	for (i = 0; i < pos1; i++)
	{
		v.push_back(cities_permutation[i]);
	}

	//�ڶ���
	for (i = pos3; i < CITY_SIZE; i++)
	{
		v.push_back(cities_permutation[i]);
	}
	//������
	for (i = pos2; i < pos3; i++)
	{
		v.push_back(cities_permutation[i]);
	}

	//���Ŀ�
	for (i = pos1; i < pos2; i++)
	{
		v.push_back(cities_permutation[i]);
	}


	for (i = 0; i < (int)v.size(); i++)
	{
		cities_permutation[i] = v[i];
	}


}

//����
void shaking(SOLUTION &solution, CITIES *cities)
{
	double_bridge_move(solution.permutation);
	solution.cost = cost_total(solution.permutation, cities);
}


void variable_neighborhood_search(SOLUTION & best_solution, CITIES * cities)
{

	int max_iterations = 5;

	int count = 0, it = 0;

	SOLUTION current_solution = best_solution;

	//�㷨��ʼ
	do 
	{
		cout << endl << "\t\tAlgorithm VNS iterated  " << it+1 << "  times" << endl;
		count++;
		it++;
		shaking(current_solution, cities);

		variable_neighborhood_descent(current_solution, cities); 

		if (current_solution.cost < best_solution.cost)
		{
			best_solution = current_solution;
			count = 0;
		}

		cout << "\t\tȫ��best_solution = " << best_solution.cost << endl;

	} while (count <= max_iterations);


}


int main()
{

	srand((unsigned) time(0));

	SOLUTION best_solution;

	random_permutation(best_solution.permutation);
	best_solution.cost = cost_total(best_solution.permutation, berlin52);

	cout << "��ʼ��·�߳��� = " << best_solution.cost << endl;

	variable_neighborhood_search(best_solution, berlin52);

	cout << endl << endl << "������ɣ� ����·���ܳ��� = " << best_solution.cost << endl;
	cout << "���ŷ��ʳ����������£�" << endl;
	for (int i = 0; i < CITY_SIZE; i++)
	{
		cout << setw(4) << setiosflags(ios::left) << best_solution.permutation[i];
	}

	cout << endl << endl;

	return 0;
}
