#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;



void Shuffle(int*);
bool HitStatus(int*, int, int);
int Betting(int*, int, int);
int SumofNum(int*);



int person_card[3][15]; // 딜러와 player의 카드

int main()
{
	int allCard[312] = { 0, };			// 전체 카드 312개(6 deck)
	int card_counting[11] = { 0, };		// A~K까지의 카드를 카운팅 ([0]은 사용X)
	int card_cnt = 0;					// 현재까지 나온 카드의 수
	int card_shuffle_num = 312 * 4 / 5;	// 전체 카드 수의 80퍼센트가 되는 수(249)
	
	int player1_win = 0, player1_draw = 0;	// card counting player의 이긴 횟수, 비긴 횟수
	int player2_win = 0, player2_draw = 0;	// simple player의 이긴 횟수, 비긴 횟수

	int dealer_sum=0, player1_sum=0, player2_sum=0;		// 딜러와 player의 카드 합
	int dealer_cnt=0, player1_cnt=0, player2_cnt=0;		// 딜러와 player의 카드 수

	int player1_money = 100000, player1_betting = 0;	// card counting player의 초기 자금, 베팅 금액
	int player2_money = 100000, player2_betting = 0;	// simple player의 초기 자금, 베팅 금액



	int play_cnt = 0;		// 입력받은 게임 횟수
	int cur_play_cnt = 0;	// 현재까지의 게임 횟수
	cout << "Play Game : ";
	cin >> play_cnt;	// 게임 횟수 입력 받음

	Shuffle(allCard);	// 모든 카드를 섞음

	while (cur_play_cnt != play_cnt)	// 입력 횟수만큼 play
	{
		// 초기화
  		fill_n(person_card[0], 45, 0);
		dealer_sum = 0, player1_sum = 0, player2_sum=0;
		dealer_cnt = 0, player1_cnt = 0, player2_cnt=0;
		player1_betting = 0, player2_betting = 0;


		// 카드의 80% 이상이 사용된 경우
		if (card_cnt > card_shuffle_num) {
			Shuffle(allCard);	// 모든 카드를 다시 섞음
			fill_n(card_counting, 11, 0);
			card_cnt = 0;
		}

		cur_play_cnt++;	// 플레이한 게임 횟수 증가


		// Betting
		player1_betting = Betting(card_counting, card_cnt, 1);	// player1의 베팅할 금액 설정
		player1_money -= player1_betting;
		player2_betting = Betting(card_counting, card_cnt, 2);	// player2의 베팅할 금액 설정
		player2_money -= player2_betting;


		// 딜러와 player1, player2가 카드를 번갈아가며 2장씩 받음
		for (int i = 0; i < 2; i++) 		// 2장
			for (int j = 0; j < 3; j++) {	// 3명
				if (j == 0) {	// 딜러
					person_card[0][dealer_cnt] = allCard[card_cnt];	// 한 장 나눠줌
					dealer_cnt++;
					if (i != 0)		// 딜러의 첫번째 카드가 아닌 경우
						card_counting[allCard[card_cnt]]++;	// Card Counting
				}
				else if (j == 1) {  // player1
					person_card[1][player1_cnt] = allCard[card_cnt];	// 한 장 나눠줌
					player1_cnt++;
					card_counting[allCard[card_cnt]]++;	// Card Counting
				}
				else {	// player2
					person_card[2][player2_cnt] = allCard[card_cnt];	// 한 장 나눠줌
					player2_cnt++;
					card_counting[allCard[card_cnt]]++;	// Card Counting
				}
				card_cnt++;
			}


		while (1)
		{
			bool hit1, hit2;
			if (hit1 = HitStatus(card_counting, card_cnt, 1))	// player1 Hit/Stand
			{
				person_card[1][player1_cnt] = allCard[card_cnt];	// 한 장 더 받음
				player1_cnt++;
				card_counting[allCard[card_cnt]]++;		// Card Counting
				card_cnt++;
			}
			if (hit2 = HitStatus(card_counting, card_cnt, 2))	// player2 Hit/Stand
			{
				person_card[2][player2_cnt] = allCard[card_cnt];	// 한 장 더 받음
				player2_cnt++;
				card_counting[allCard[card_cnt]]++;		// Card Counting
				card_cnt++; 
			}

			if (hit1 == false && hit2 == false)	// 둘 다 Stand이면 반복문 탈출
				break;
		}

		// dealer Hit/Stand
		while (HitStatus(card_counting, card_cnt, 0)) // 합<17이면 계속 Hit
		{
			person_card[0][dealer_cnt] = allCard[card_cnt];	// 한 장 더 받음
			dealer_cnt++;
			card_counting[allCard[card_cnt]]++;		// Card Counting
			card_cnt++;
		}


		dealer_sum  = SumofNum(person_card[0]);	// dealer의 카드 합
		player1_sum = SumofNum(person_card[1]);	// player1의 카드 합
		player2_sum = SumofNum(person_card[2]);	// player2의 카드 합

		// player1 승부 가림
		if (dealer_sum < player1_sum)
		{
			if (player1_sum > 21) {		// player1 카드 합 > 21
			}
			else {	// 이김
				player1_win++;
				player1_money += player1_betting * 2;	// 베팅 금액의 2배 돌려 받음
			}
		}
		else // dealer_sum >= player1_sum
		{
			if (dealer_sum == player1_sum) {  
				if (player1_sum > 21) {
				}
				else {
					player1_draw++;	// 비김
					player1_money += player1_betting;
				}
			}
			else {	// dealer_sum > player1_sum, 플레이어가 짐
				if (dealer_sum > 21) {		// player1 카드 합 > 21
					if (player1_sum > 21) {	// dealer 카드 합 > 21
					}
					else {	// player1 이김
						player1_win++;
						player1_money += player1_betting * 2;	// 베팅 금액의 2배 돌려 받음
					}
				}
				else {	// player1 짐
				}
			}
		}

		// player2 승부 가림
		if (dealer_sum < player2_sum)
		{
			if (player2_sum > 21) {		// player2 카드 합 > 21
			}
			else {	// 이김
				player2_win++;
				player2_money += player2_betting * 2;	// 베팅 금액의 2배 돌려 받음
			}
		}
		else // dealer_sum >= player2_sum
		{
			if (dealer_sum == player2_sum) {
				if (player2_sum > 21) {
				}
				else {
					player2_draw++;	// 비김
					player2_money += player2_betting;
				}
			}
			else {	// dealer_sum > player2_sum, 플레이어가 짐
				if (dealer_sum > 21) {		// player2 카드 합 > 21
					if (player2_sum > 21) {	// dealer 카드 합 > 21
					}
					else {	// player2 이김
						player2_win++;
						player2_money += player2_betting * 2;	// 베팅 금액의 2배 돌려 받음
					}
				}
				else {	// player2 짐
				}
			}
		}


		/*printf("=========%d번째============\n", cur_play_cnt);
		printf("card counting\n");
		for (int i = 1; i <= 10; i++)
			printf("%d:%d  ", i, card_counting[i]);
		printf("\ncard cnt: %d\n\n", card_cnt);
		printf("dealer\n");
		for (int i = 0; i < 10; i++)
		{
			printf("[%d]:%d ", i, person_card[0][i]);
		}
		printf("\n\ncard counting player\n");
		for (int i = 0; i < 10; i++)
		{
			printf("[%d]:%d ", i, person_card[1][i]);
		}
		printf("\nsimple player\n");
		for (int i = 0; i < 10; i++)
		{
			printf("[%d]:%d ", i, person_card[2][i]);
		}
		printf("\nwin: %d, draw: %d\n", player1_win, player1_draw);
		printf("win: %d, draw: %d\n\n", player2_win, player2_draw);*/

		
		card_counting[person_card[0][0]]++;	// dealer의 첫번째 카드를 공개하고 카운팅
	}


	float win_rate1 = 0.0f;
	int up = player1_win;
	int down = play_cnt - player1_draw;
	win_rate1 = (float)up / down;
	win_rate1 *= 100.0f;
	printf("Card counting Player     win: %d / draw: %d / 승률: %.4f%% / 남은 돈: %d\n", player1_win, player1_draw, win_rate1, player1_money);

	float win_rate2 = 0.0f;
	up = player2_win;
	down = play_cnt - player2_draw;
	win_rate2 = (float)up/down;
	win_rate2 *= 100.0f;
	printf("Simple Player\t\t win: %d / draw: %d / 승률: %.4f%% / 남은 돈: %d\n\n", player2_win, player2_draw, win_rate2, player2_money);

	

	return 0;
}

void Shuffle(int* arr)	// 모든 카드를 다시 섞음
{
	memset(arr, 0, 312);	// arr 초기화

	int idx = 0;
	for (int i = 0; i < 6; i++) {			// 6 deck
		for (int j = 0; j < 4; j++) {		// 4개의 무늬
			for (int k = 1; k <= 10; k++) { // 카드의 값
				if (k != 10)
				{
					arr[idx] = k;		// A ~ 9 저장
					idx++;
				}
				else
				{
					for (int l = 0; l < 4; l++) {
						arr[idx] = 10;	// 10, J, Q, K 저장
						idx++;
					}
				}
			}
		}
	}
	
	int randNum = 0, temp = 0;
	srand((unsigned int)time(NULL));
	for (int i = idx - 1; i >= 0; i--)
	{
		randNum = rand() % idx;		// 난수 생성
		// swap
		temp = arr[i];
		arr[i] = arr[randNum];
		arr[randNum] = temp;
	}
}

bool HitStatus(int* card_counting, int card_cnt, int person_num)	// Hit할지 Stand할지를 정해줌
{
	int player_sum = SumofNum(person_card[person_num]);	// 현재 플레이어의 카드 합

	if (person_num == 0 || person_num == 2)	 // 딜러 또는 simple player인 경우
	{
		if (player_sum >= 17)	// 카드 합이 17 이상이면 Stand
			return false;
		else                // 카드 합이 17 미만이면 Hit
			return true;

	}
	else if (person_num == 1)	// card counting player인 경우
	{
		if (player_sum >= 21)	// 합이 21이면 무조건 Stand
			return false;
		if (player_sum <= 11)	// 최대 값 10이 나와도 Bust되지 않으므로 무조건 Hit
			return true;

		int small_sum = 0;		 // A~5 카드 수의 합 
		int	big_sum = 0;		 // 9,10,J,Q,K 카드 수의 합	(A는 무조건 1로 쓸 것이므로 제외)
		float small_prob = 0.0f; // A~5 카드가 나올 확률 
		float big_prob = 0.0f;	 // 9,10,J,Q,K 카드가 나올 확률

		for (int i = 1; i <= 5; i++)
			small_sum += card_counting[i];
		big_sum = card_counting[9] + card_counting[10];

		small_prob = (float)(6 * 4 * 5 - small_sum) / (312 - card_cnt);	// A~5 카드 나올 확률
		big_prob = (float)(6 * 4 * 5 - big_sum) / (312 - card_cnt);	   // 9~10 카드 나올 확률

		int dealer_sum = 0;
		if (person_card[0][1] != 1)	
			dealer_sum = 10 + person_card[0][1]; 	// dealer의 카드 합 (첫 번째 카드 비공개이므로 10이라고 가정)
		else  // dealer의 두번째 카드가 A이면 11로 계산
			dealer_sum = 10 + 11;
		
		
		if (dealer_sum == 21)	// 딜러의 두번째 카드가 A인 경우 (합이 21이라 가정)
		{
			int max = 21 - player_sum;			// 합이 21이 되기 위한 최대값
			int temp_sum = 0;
			for (int i = 1; i <= max; i++)
				temp_sum += card_counting[i];	// 1~max 카드 수 더함
			float temp_prob = (float)(6 * 4 * max - temp_sum) / (312 - card_cnt);	// 1~max 카드 나올 확률

			if (temp_prob >= 0.5f)	// min~max 카드 나올 확률이 절반 이상인 경우 Hit
				return true;
			else
				return false;
		}

		if (player_sum < 17)
		{
			if (small_prob <= big_prob) 	// 큰수가 나올 확률이 더 큰 경우
			{
				if (dealer_sum >= 17)	// dealer는 무조건 Stand
				{
					int max = 21 - player_sum;			// 합이 21이 되기 위한 최대값
					int temp_sum = 0;
					for (int i = 1; i <= max; i++) 
						temp_sum += card_counting[i];	// 1~max 카드 수 더함
					float temp_prob = (float)(6 * 4 * max - temp_sum) / (312 - card_cnt);	// 1~max 카드 나올 확률
					
					if (temp_prob >= 0.5f)	// 1~max 카드 나올 확률이 절반 이상인 경우 Hit
						return true;
					else
						return false;
				}
				else  // dealer는 무조건 Hit
				{
					return false;	// dealer가 Bust할 확률이 크므로 player는 Stand함
				}
			}
			else  // 작은수가 나올 확률이 더 크면 Hit
			{
				return true;
			}
		}
		else // player_sum >= 17
		{
			if (dealer_sum >= 17)	// dealer는 무조건 Stand
			{
				if (player_sum >= dealer_sum)	// player_sum이 dealer_sum보다 크거나 같으면 Stand
				{
					return false;
				}
				else 		// player_sum이 dealer_sum보다 작은 경우 (그냥 Stand하면 짐)
				{
					int max = 21 - player_sum;			// 합이 21이 되기 위한 최대값
					int temp_sum = 0;
					for (int i = 1; i <= max; i++)
						temp_sum += card_counting[i];	// 1~max 카드 수 더함
					float temp_prob = (float)(6 * 4 * max - temp_sum) / (312 - card_cnt);	// 1~max 카드 나올 확률

					if (temp_prob >= 0.5f)	// min~max 카드 나올 확률이 절반 이상인 경우 Hit
						return true;
					else
						return false;
				}
			}
			else	// dealer는 무조건 Hit
			{
				if (small_prob <= big_prob) 	// 큰수가 나올 확률이 더 큰 경우
				{
					return false;	// dealer가 Bust할 확률이 크므로 player는 Stand함
				}
				else	// 작은수가 나올 확률이 더 큰 경우
				{
					int max = 21 - player_sum;			// 합이 21이 되기 위한 최대값
					int temp_sum = 0;
					for (int i = 1; i <= max; i++)
						temp_sum += card_counting[i];	// min~max 카드 수 더함
					float temp_prob = (float)(6 * 4 * max - temp_sum) / (312 - card_cnt);	// min~max 카드 나올 확률

					if (temp_prob >= 0.5f)	// min~max 카드 나올 확률이 절반 이상인 경우 Hit
						return true;
					else
						return false;
				}
			}

		}

	}
}

int Betting(int* card_counting, int card_cnt, int person_num)	// 베팅 금액을 정해주는 함수
{
	if (person_num == 2)	// simple player는 무조건 100원 베팅
		return 100;

	else {	// card counting player
		int small_sum = 0;		 // A~5 카드 수의 합 
		int	big_sum = 0;		 // 9,10,J,Q,K 카드 수의 합	(A는 무조건 1로 쓸 것이므로 제외)
		int basic_betting = 100;	// 기본 베팅 금액
		float betting_rate = 0.0f;	// 베팅 금액 배수

		for (int i = 1; i <= 5; i++)
			small_sum += card_counting[i];
		big_sum = card_counting[9] + card_counting[10];

		betting_rate = 1.0f + (float)(small_sum - big_sum) / (312 - card_cnt);


		return (int)basic_betting * betting_rate;	// 기본 베팅 금액 * 배수 반환
	}
}


int SumofNum(int* card)	// 현재 플레이어의 카드 숫자 합을 반환
{
	int sum = 0;
	int i = 0;
	int A_count = 0;	// A 카드의 수

	while (card[i] != 0)
	{
		if (card[i] != 1)	// 2~10
			sum += card[i];
		else {
			sum += 11;	// A는 11로 계산
			A_count++;
		}
		i++;
	}
	while (A_count > 0) {
		if (sum > 21) {
			sum -= 10;	// A를 1로 계산
			A_count--;
		}
		else
			break;
	}

	return sum;
}
