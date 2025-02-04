#include <iostream> //cin cout
#include <cstdio> //printf scanf fscanf FILE
#include <cstdlib> //qsort
#include <ctime> //time
#include <vector> //vector
#include <string> //string compare
using namespace std;

#define USER_MAX 24000000
#define AD_MAX 24000000
#define URL_MAX 23
#define IMPRESSED_MAX 80000000

#define READ
#define READ_PROCESS
#define INIT
#define PRINT
#define TIME
#define IMPRESSED

class DATA_USER { //user branch
public:
	vector<int> Click;
	vector<int> Impression;
	vector<int> Ad;
	vector<int> Depth;
	vector<int> Position;
	vector<int> Query;
	#ifdef PRINT
	vector<string> URL;
	vector<int> Advertiser;
	vector<int> Keyword;
	vector<int> Title;
	vector<int> Description;
	#endif
};

class DATA_AD { //ad branch
public:
	vector<int> Click;
	vector<int> Impression;
	vector<string> URL;
	vector<int> Ad;
	vector<int> Advertiser;
	vector<int> Keyword;
	vector<int> Title;
	vector<int> Description;
	vector<int> User;
};

class DATA { //root of all data
public:
	DATA();
	~DATA();

	void Read(FILE*);
	void Get(int&, int&, int&, int&, int&);
	void Clicked(int&);
	void Impressed(int&, int&);
	void Profit(int&, double&);
	void Quit();
	#ifdef PRINT
	void Print(int&);
	#endif

	DATA_USER *USER;
	DATA_AD *AD;
	int max_usr;
	int max_ad;
	int min_ad;
};

DATA::DATA()
{
	#ifdef INIT
	cout << "initializing..." << endl;
	#endif
	USER = new DATA_USER[USER_MAX];
	AD = new DATA_AD[AD_MAX];
	#ifdef INIT
	printf("initialization success\tat %f secs\n", (double)clock()/CLOCKS_PER_SEC);
	#endif
}

DATA::~DATA()
{
	delete [] USER;
	delete [] AD;
}

void DATA::Read(FILE *ptr)
{
	int click, imp, ad, adv, depth, pos, query, key, title, des, usr, count = 0;
	char url[URL_MAX];
	max_usr = max_ad = 0;
	min_ad = AD_MAX;
	#ifdef READ
	printf("file open success\tat %f secs\nreading...\n", (double)clock()/CLOCKS_PER_SEC);
	#endif
	do {
		fscanf(ptr, "%d%d%s%d%d%d%d%d%d%d%d%d",&click, &imp, &url, &ad, &adv, &depth, &pos, &query, &key, &title, &des, &usr);
		string s_url(url);
		if (feof(ptr)) break;
		USER[usr].Click.push_back(click);//
		USER[usr].Impression.push_back(imp);//
		USER[usr].Ad.push_back(ad);//
		USER[usr].Depth.push_back(depth);//
		USER[usr].Position.push_back(pos);//
		USER[usr].Query.push_back(query);//
		#ifdef PRINT
		USER[usr].URL.push_back(s_url);
		USER[usr].Advertiser.push_back(adv);
		USER[usr].Keyword.push_back(key);
		USER[usr].Title.push_back(title);
		USER[usr].Description.push_back(des);
		#endif
		AD[ad].Click.push_back(click);
		AD[ad].Impression.push_back(imp);
		AD[ad].URL.push_back(s_url);
		AD[ad].Ad.push_back(ad);
		AD[ad].Advertiser.push_back(adv);
		AD[ad].Keyword.push_back(key);
		AD[ad].Title.push_back(title);
		AD[ad].Description.push_back(des);
		AD[ad].User.push_back(usr);
		if (usr > max_usr) max_usr = usr;
		if (ad > max_ad) max_ad = ad;
		if (ad < min_ad) min_ad = ad;
		#ifdef READ_PROCESS
		count++;
		if (count==37500000) printf("25%%\n");
		if (count==75000000) printf("50%%\n");
		if (count==112500000) printf("75%%\n");
		#endif
	} while (!feof(ptr));
	#ifdef READ
	printf("100%%\n");
	printf("read success\t\tat %f secs\n",(double)clock()/CLOCKS_PER_SEC);
	#endif
	fclose(ptr);
}

void DATA::Get(int& user, int& ad, int& query, int& pos, int& depth)
{
	int t1 = clock();
	int sum_click = 0, sum_impression = 0;
	for (int i = 0 ; i < USER[user].Ad.size() ; i++) {
		if (USER[user].Ad[i] == ad &&
			USER[user].Query[i] == query &&
			USER[user].Position[i] == pos &&
			USER[user].Depth[i] == depth) {
			sum_click += USER[user].Click[i];
			sum_impression += USER[user].Impression[i];
		}
	}
	printf("********************\n");
	printf("%d %d\n", sum_click, sum_impression);
	printf("********************\n");
}

typedef struct temp1{
	int Ad;
	int Query;
} Ck_output;

int Ck_compare(const void *a, const void *b)
{
	int ad1 = ((Ck_output*)a)->Ad;
	int ad2 = ((Ck_output*)b)->Ad;
	int query1 = ((Ck_output*)a)->Query;
	int query2 = ((Ck_output*)b)->Query;
	return ( (ad1==ad2)?(query1==query2)? 0:(query1-query2):(ad1-ad2) );
}

void DATA::Clicked(int& user)
{
	int dataSize = USER[user].Click.size();
	int j = 0;
	Ck_output *output = new Ck_output[dataSize];
	for (int i = 0 ; i < dataSize ; i++) {
		if (USER[user].Click[i] >= 1) {
			output[j].Ad = USER[user].Ad[i];
			output[j].Query = USER[user].Query[i];
			j++;
		}
	}
	int outputSize = j, count = 0;
	qsort(output, outputSize, sizeof(Ck_output), Ck_compare);
	printf("********************\n");
	if (outputSize > 0) {
		printf("%d %d\n", output[0].Ad, output[0].Query);
		count++;
		for (int i = 1 ; i < outputSize ; i++) {
			if (output[i].Ad != output[i-1].Ad || 
				output[i].Query != output[i-1].Query) {
			printf("%d %d\n", output[i].Ad, output[i].Query);
			count++;
			}
		}
	}
	printf("********************\n");
	printf("total : %d\t", count);
	delete [] output;
}

typedef struct temp2 {
	int Ad; string url; int adv; 
	int key; int title; int des;
} Imp_output;

int Imp_compare(const void *a, const void *b)
{
	int ad1 = ((Imp_output*)a)->Ad;
	int ad2 = ((Imp_output*)b)->Ad;
	string url1 = ((Imp_output*)a)->url;
	string url2 = ((Imp_output*)b)->url;
	int adv1 = ((Imp_output*)a)->adv;
	int adv2 = ((Imp_output*)b)->adv;
	int key1 = ((Imp_output*)a)->key;
	int key2 = ((Imp_output*)b)->key;
	int tle1 = ((Imp_output*)a)->title;
	int tle2 = ((Imp_output*)b)->title;
	int des1 = ((Imp_output*)a)->des;
	int des2 = ((Imp_output*)b)->des;
	return ( (ad1==ad2)?(url1.compare(url2)==0)?(adv1==adv2)?(key1==key2)?(tle1==tle2)?(des1==des2)? 
		0:(des1-des2):(tle1-tle2):(key1-key2):(adv1-adv2):(url1.compare(url2)):(ad1-ad2) );
}

void Imp_assign(Imp_output *out, int tmp, DATA_AD *in, int ad, int index)
{
	out[tmp].Ad = in[ad].Ad[index];
	out[tmp].url = in[ad].URL[index];
	out[tmp].adv = in[ad].Advertiser[index];
	out[tmp].key = in[ad].Keyword[index];
	out[tmp].title = in[ad].Title[index];
	out[tmp].des = in[ad].Description[index];
}

int Imp_isSame(Imp_output *tmp, int a, int b)
{
	return (tmp[a].Ad==tmp[b].Ad && tmp[a].adv==tmp[b].adv && tmp[a].key==tmp[b].key &&
		tmp[a].title==tmp[b].title && tmp[a].des==tmp[b].des && tmp[a].url.compare(tmp[b].url)==0);
}

void DATA::Impressed(int& user1, int& user2)
{
	Imp_output *output = new Imp_output[IMPRESSED_MAX];
	#ifdef IMPRESSED
	cout << "new struct init success\n";
	#endif
	int found1, found2, tmp = 0;
	for (int i = min_ad ; i < max_ad ; i++) {
		found1 = found2 = 0;
		vector<int> index;
		for (int j = 0 ; j < AD[i].Click.size() ; j++) {
			if (AD[i].User[j] == user1) {
				found1 = 1;
				index.push_back(j);
			}
			if (AD[i].User[j] == user2) {
				found2 = 1;
				index.push_back(j);
			}
		}
		if (found1 && found2) {
			for (int k = 0 ; k < index.size() ; k++) {
				Imp_assign(output, tmp, AD, i, index[k]);
				tmp++;
			}
		}
	}
	#ifdef IMPRESSED
	cout << "filter success\n";
	#endif
	qsort(output, tmp, sizeof(Imp_output), Imp_compare);
	#ifdef IMPRESSED
	cout << "qsort success\n";
	#endif
	int count = 0;
	printf("********************\n");
	if (tmp > 0) {
		printf("%d\n", output[0].Ad);
		cout << "\t" << output[0].url;
		printf(" %d %d %d %d\n", output[0].adv, output[0].key, output[0].title, output[0].des);
		count++;
		for (int i = 1 ; i < tmp ; i++) {
			if (output[i].Ad != output[i-1].Ad) printf("%d\n", output[i].Ad);
			if (!Imp_isSame(output, i, i-1)) {
				cout << "\t" << output[i].url;
				printf(" %d %d %d %d\n", output[i].adv, output[i].key, output[i].title, output[i].des);
				count++;
			}
		}
	}
	printf("********************\n");
	printf("total : %d\ttemp total : %d\n", count, tmp);
	delete [] output;
}


typedef struct temp3 {
	int Click;
	int Impression;
} Pro_output;

void DATA::Profit(int& ad, double& std_ratio)
{
	int size = AD[ad].Click.size(), count = 0;
	Pro_output *temp_usr = new Pro_output[max_usr];
	for (int i = 0 ; i < size ; i++) {
		temp_usr[AD[ad].User[i]].Click += AD[ad].Click[i];
		temp_usr[AD[ad].User[i]].Impression += AD[ad].Impression[i];
	}
	double usr_ratio = 0;
	printf("********************\n");
	for (int i = 0 ; i < max_usr ; i++) {
		if (temp_usr[i].Click == 0 || temp_usr[i].Impression == 0) continue;
		usr_ratio = (double)temp_usr[i].Click/(double)temp_usr[i].Impression;
		if (usr_ratio >= std_ratio) {
			printf("%d\n", i);
			count++;
		}
	}
	printf("********************\n");
	printf("total : %d\t", count);
	delete [] temp_usr;
}

#ifdef PRINT
void DATA::Print(int& user)
{
	if (user < 0) {
		printf("biggest usr = %d\nbiggest ad = %d\nsmallest ad = %d\n", max_usr, max_ad, min_ad);
		return;
	}
	printf("Click\tImp\tURL\t\t\tAd\t\tAdv\tDepth\tPos\tQuery\t\tKeyword\tTitle\tDes\n");
	for (int i = 0 ; i < USER[user].Click.size() ; i++) {
		cout << USER[user].Click[i] << "\t";
		cout << USER[user].Impression[i] << "\t";
		cout << USER[user].URL[i] << "\t";
		cout << USER[user].Ad[i] << "  \t";
		cout << USER[user].Advertiser[i] << "\t";
		cout << USER[user].Depth[i] << "\t";
		cout << USER[user].Position[i] << "\t";
		cout << USER[user].Query[i] << "       \t";
		cout << USER[user].Keyword[i] << "\t";
		cout << USER[user].Title[i] << "\t";
		cout << USER[user].Description[i] << "\t\n";
	}
	printf("total : %d\t", USER[user].Click.size());
}
#endif

void DATA::Quit(){ cout << "# leave the program" << endl;}

int main(int argc, char const *argv[])
{
	DATA data;
	FILE *ptr = fopen(argv[1], "r");
	data.Read(ptr);
	string input;
	int ad, depth, pos, query, usr1, usr2;
	double ratio;
	cin >> input;
	while(input.compare("quit") != 0) {
		int t1 = clock();
		if (input.compare("get") == 0) {
			scanf("%d%d%d%d%d", &usr1, &ad, &query, &pos, &depth);
			data.Get(usr1, ad, query, pos, depth);
		}
		if (input.compare("clicked") == 0) {
			scanf("%d", &usr1);
			data.Clicked(usr1);
		}
		if (input.compare("impressed") == 0) {
			scanf("%d%d", &usr1, &usr2);
			data.Impressed(usr1, usr2);
		}
		if (input.compare("profit") == 0) {
			scanf("%d%lf", &ad, &ratio);
			data.Profit(ad, ratio);
		}
		#ifdef PRINT
		if (input.compare("print") == 0) {
			scanf("%d", &usr1);
			data.Print(usr1);
		}
		#endif
		#ifdef TIME
		int t2 = clock();
		printf("spend %f secs\n", (double)(t2 - t1)/CLOCKS_PER_SEC);
		#endif
		cin >> input;
	}
	data.Quit();
	return 0;
}