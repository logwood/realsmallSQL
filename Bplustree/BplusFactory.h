#include <iostream>
#include<time.h>
#include"BAddTree.h"
using namespace BAT;
using namespace std;
void testBTree();//功能测试
//记录
struct Entry
{
public:
	Entry() = default;
	Entry(const int64_t k, long e) { this->k = k; this->file_offset = e; };
	int Table_Size(int r){table_size=r;};
	~Entry()=default;
	int64_t key()const {//接口函数
		return k;
	};
	long file_offset;
	int table_size=3;
private:
	int k;
};
//遍历函数
auto itf = [](deque <Entry*>&e) {
	int _s = e.size();
	for (int i = 0; i < _s; ++i) {
		//printf(" %ld ", e[i]->key());
	}
};
auto itfs = [](deque <Entry*>&e,int vals,int operators) {
	int _s = e.size();
	deque <long> times;
	for (int i = 0; i < _s; ++i) {
		if(operators!=0?((e[i]->key()-vals)*operators>0):(e[i]->key()==vals))
		{
			long keyis=e[i]->file_offset;
			times.push_back(keyis);
		}
	}
	return times;
};
class BTreeForUse
{
	BTreeForUse(std::string file)
	{
		path="idx/"+file;
	}
	void ReadBTree()
	{
		
	}
	void BTreeAdd(std::vector<Entry> names)
	{

	}
	void BTreeAddOne(std::vector<Entry> names)
	{
		
	}
private:
	std::string path;
};



void testBTree() {
	srand(time(NULL));
	int order =7;
	BAddTree<int,Entry>tree(order);
	int low, up = 0;
	do {
		low = rand() % 100;
		up = rand() % 100;
		if (up < low) {
			swap(up, low);
		}
	} while (up - low > 1000 || up - low < 20);
	up = 200; low = 1;
	vector<Entry>num;
	for (int i = low; i < up; ++i) {
		num.insert(num.end(),Entry(rand()*rand()*rand()%10000,(float)rand()/rand()));
	}
	vector<Entry>r;
	//cout << "insert values the fllowing:\n" << endl;
	while (!num.empty())
	{
		int s = rand() % (num.size());
		auto it = num.begin();
		advance(it, s);

		//cout << num[s].key() << " ";
		bool ok=tree.insert(num[s]);
		if(ok){
		r.insert(r.end(), num[s]);
		}
		else {
			//cout << "\n\n\n inserting the entry had failed:" << it->key() << "\n\n";
		}
		num.erase(it);
	}
	cout << "\ntree size:\n" << tree.size() << endl;
	cout << "start  traversal:\n";
 	tree.tree_traversal(itf);
	cout << endl;
	cout << "remove randomly:\n\t";
	int n = 0;
	while (n++ < 3)
	{
		int x = rand() % r.size();
		auto it = r.begin();
		advance(it, x);
		bool ok=tree.remove(*it);
		if (ok) {
			cout <<it->key() << " ";
		}
		else {
			cout << "\n\n\n removing the entry had failed:"<<it->key()<<"\n\n";
		}
		r.erase(it);
	}
	cout << endl;
	cout << "now tree size:\n" << tree.size() << endl;
	cout << "now i traversal:\n";
	tree.tree_traversal(itf);
	cout << endl;
	cout << "remove randomly:\n\t";
	n = 0;
	while (n++ < 3)
	{
		int x = rand() % r.size();
		auto it = r.begin();
		advance(it, x);
		bool ok = tree.remove(*it);
		if (ok) {
			cout << it->key() << " ";
		}
		else {
			cout << "\n\n\n removing the entry had failed:" << it->key() << "\n\n";
		}
		r.erase(it);
	}
	cout << endl;
	cout << "now tree size:\n" << tree.size() << endl;
	cout << "now traversal:\n";
	tree.tree_traversal(itf);
	cout << endl;
	cout << "remove all:\n\t";
	n = 0;
	while ( r.size()>0)
	{
		int x = rand() % r.size();
		auto it = r.begin();
		advance(it, x);
		bool ok = tree.remove(*it);
		if (ok) {
			cout << it->key() << " ";
		}
		else {
			cout << "\n\n\n removing the entry had failed:" << it->key() << "\n\n";
		}
		r.erase(it);
	}
	cout << endl;
	cout << "now tree size:\n" << tree.size() << endl;
	cout << "now traversal:\n";
	tree.tree_traversal(itf);
	cout << endl;
	//重插
	{
		low, up = 0;
		do {
			low = rand() % 100;
			up = rand() % 100;
			if (up < low) {
				swap(up, low);
			}
		} while (up - low > 1000 || up - low < 20);
		for (int i = low; i < up; ++i) {
			num.insert(num.end(), Entry(rand(), (float)rand() / rand()));
		}
		cout << "insert values the fllowing:\n" << endl;
		while (!num.empty())
		{
			int s = rand() % (num.size());
			auto it = num.begin();
			advance(it, s);

			cout << num[s].key() << " ";
			bool ok = tree.insert(num[s]);
			if (ok) {
				r.insert(r.end(), num[s]);
			}
			else {
				cout << "\n\n\n inserting the entry had failed:" << it->key() << "\n\n";
			}
			num.erase(it);
		}
		cout << endl;
		cout << "now tree size:\n" << tree.size() << endl;
		cout << "now traversal:\n";
		tree.tree_traversal(itf);
		cout << endl;
		cout << "remove randomly:\n\t";
		int n =rand()%(r.size()-1)+1;
		while (n-- > 0)
		{
			int x = rand() % r.size();
			auto it = r.begin();
			advance(it, x);
			bool ok = tree.remove(*it);
			if (ok) {
				cout << it->key() << " ";
			}
			else {
				cout << "\n\n\n removing the entry had failed:" << it->key() << "\n\n";
			}
			r.erase(it);
		}
		cout << endl;
		cout << "now tree size:\n" << tree.size() << endl;
		tree.tree_traversal(itf);
		cout << "now i traversal:\n";
		tree.list_traversal(itfs,0,23200321);
		cout << "end" << endl;
		cout << endl;
 	}
 	BAddTree<int,Entry>ReadTree(order);
	std::string name ="name";
	tree.searchall(name);
 	std::ifstream in(name,std::ios::in|std::ios::binary);
 	ReadTree.ReadBtree(0,in);
	cout << "now tree size is:\n" << tree.size() << endl;
	ReadTree.list_traversal(itfs,0,232003212);
}