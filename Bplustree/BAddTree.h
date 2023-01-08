#ifndef BADDTREE_H_H_H
#define BADDTREE_H_H_H
#endif
/*
B+树
K是码,E是记录
E类型必须实现函数（K key()）定义
*/


#include"BAddTreeNode.h"
#include<queue>
#include <iostream>
#include<fstream>
namespace BAT {	
	struct write_for_binary
	{
		int offset[7]={0};
		int childoffset[8]={0};
		int key[7]={-114514,-114514,-114514,-114514,-114514,-114514,-114514};
		int isleaf=0;
	};
	using namespace BATN;
	template<typename K, typename E>
	class BAddTree
	{
	public:
		explicit BAddTree(int order);//当小于2时默认为2阶,即退化为二叉搜索树 (m>=2);这里不设默认值，便于继承
		~BAddTree();

		int const order()const;//阶次
		int const size()const;//规模

			//操作接口
		virtual BAddTreeLeafNode<K,E>*  search(E const&);//查找
		virtual BAddTreeLeafNode<K,E>*  searchall();//查找
		virtual void saveinfile(BAddTreeLeafNode<K, E>*,std::string );//查找
		virtual void traverse(BAddTreeLeafNode<K, E>*);
		virtual void traversenew(BAddTreeLeafNode<K, E>*);
		virtual void ReadBtree(int,std::ifstream&);
		virtual bool insert(E const&);//插入
		virtual bool remove(E const&);//删除
		template<typename Visist>
		void list_traversal(Visist );//遍历
		template<typename Visist>
		void tree_traversal(Visist );//树上的遍历	
	protected:
		BAddTree();//可继承
		void solveOverFlow(BAddTreeLeafNode<K, E>*);//解决插入上溢现象 参数为节点
		void solveUnderFlow(BAddTreeLeafNode<K, E>*);//解决插入下溢现象 参数为节点
	protected:
		int m_size;//关键码总数
		int m_order;//B-树的阶次,至少为2(即退化为二叉搜索树) 创建时指定 一般不做修改
		BAddTreeNode<K,E>* m_root;//树根
		BAddTreeNode<K,E>* m_hitParentNode;//查找访问时命中节点的父亲或者穿透底层后最后一个访问的节点，当树为空时 其访问后为空
		BAddTreeLeafNode<K, E>*m_header, *m_trail;//双向链表首尾
	};

	template<typename K, typename E>
	inline BAddTree<K, E>::BAddTree(int order)
	{
		if (order < 2)order = 2;//默认2阶
		m_order = order;
		//注：根初始化为叶子节点
		this->m_root = new BAddTreeLeafNode<K,E>(); m_size = 0; m_hitParentNode = nullptr;
		m_header = new BAddTreeLeafNode<K,E>();
		m_trail = new BAddTreeLeafNode<K,E>();
		BAddTreeLeafNode<K, E>*_leaf = (BAddTreeLeafNode<K, E>*)(this->m_root);
		m_header->next = _leaf; m_trail->last = _leaf;//链表
		_leaf->last = m_header; _leaf->next = m_trail; 
	}

	template<typename K, typename E>
	inline BAddTree<K, E>::~BAddTree()
	{
		if (!this->m_root)return;//没有根就不删除
		std::queue<BAddTreeNode<K,E>*>q;//引入辅助队列按层次遍历删除
		q.push(this->m_root);//树根入队
		while (!q.empty())
		{
			BAddTreeNode<K,E>*p = q.front();
			q.pop();
			int count = p->child.size();
			for (int i = 0; i < count; ++i) {
				if (p->child[i]) {
					q.push(p->child[i]);//添加所有孩子入队
				}
			}
			if (p->isLeaf()) {
				BAddTreeLeafNode<K, E>*_t = (BAddTreeLeafNode<K, E>*)p;
				delete _t, _t = nullptr;//析构叶子节点
			}
			else {
				delete p, p = nullptr;//释放每个内部节点
			}
		}
		delete m_header, m_header = nullptr;
		delete m_trail, m_trail = nullptr;
		this->m_root = nullptr;
		this->m_size = 0;
	}

	template<typename K, typename E>
	inline int const BAddTree<K, E>::order() const
	{
		return m_order;
	}

	template<typename K, typename E>
	inline int const BAddTree<K, E>::size() const
	{
		return m_size;
	}

	template<typename K, typename E>
	inline BAddTreeLeafNode<K, E>* BAddTree<K, E>::search(E const &e)
	{
		this->m_hitParentNode = nullptr;
		K k = e.key();
		BAddTreeLeafNode<K, E>*v = static_cast<BAddTreeLeafNode<K, E>*>(this->m_root);//c++式静态转换
		while (!v->isLeaf())
		{
			int _s = v->key.size();
			int i = 0;
			for (; i < _s; ++i) {
				K&tk = v->key[i];
				if (tk > k)break;
			}
			this->m_hitParentNode = static_cast<BAddTreeNode<K, E>*>(v);
			v = static_cast<BAddTreeLeafNode<K, E>*>(v->child[i]);//转到下一层
		}
		if (v&&v->isLeaf()) {
			int _s = v->key.size();
			int i = 0;
			for (; i < _s; ++i) {
				K&tk = v->key[i];
				if (tk == k) {
					return v;
				}
				else if (tk > k) 
					break;
              }
			this->m_hitParentNode = static_cast<BAddTreeNode<K, E>*>(v);
			v = static_cast<BAddTreeLeafNode<K, E>*>(v->child[i]);//转到下一层
		}
		return v;
	}
	template<typename K, typename E>
	void BAddTree<K,E>::traverse(BAddTreeLeafNode<K, E>* v)
	{
			int _s = v->key.size();
			int i = 0;
			if(v->isLeaf())
			for (; i < _s; ++i) {
				if(v->key.size()>i)
				{
					std::cout<<v->key[i]<<' '<<std::endl;
					if(v->offset>offset)
					{
						v->offset=offset++;
					}
				}
				if(v->child[i]!=nullptr)
				traverse(static_cast<BAddTreeLeafNode<K, E>*>(v->child[i]));
			}
			else{
				for (; i < _s+1; ++i) {
				if(v->key.size()>i)
				{
				std::cout<<v->key[i]<<' '<<std::endl;
				if(v->offset>offset)
					{
						v->offset=offset++;
					}
				}
				if(v->child[i]!=nullptr)
				traverse(static_cast<BAddTreeLeafNode<K, E>*>(v->child[i]));
			}
			}
	}
	template<typename K, typename E>
	void BAddTree<K,E>::traversenew(BAddTreeLeafNode<K, E>* v)
	{
			int _s = v->key.size();
			int i = 0;
			if(v->isLeaf())
			for (; i < _s; ++i) {
				if(v->key.size()>i)
				{
					if(!v->isLeaf())
					for(int j=0;j<v->child.size();j++)
					{
						v->childoffset.emplace_back(v->child[j]->offset);
					}
				}
				if(v->child[i]!=nullptr)
				traversenew(static_cast<BAddTreeLeafNode<K, E>*>(v->child[i]));
			}
			else{
				for (; i < _s+1; ++i) {
				if(v->key.size()>i)
				{
				if(!v->isLeaf())
				for(int j=0;j<v->child.size();j++)
					{
						v->childoffset.emplace_back(v->child[j]->offset);
					}
				}
				if(v->child[i]!=nullptr)
				traversenew(static_cast<BAddTreeLeafNode<K, E>*>(v->child[i]));
			}
			}
	}
	template<typename K, typename E>
	void BAddTree<K,E>::saveinfile(BAddTreeLeafNode<K, E>* v,std::string name)
	{
		int _s = v->key.size();
		int i = 0;
		if(!v->isLeaf())
			for (; i < _s; ++i) {
				write_for_binary write;
				std::copy(v->key.begin(), v->key.end(), write.key);
				write.isleaf=0;
				std::copy(v->childoffset.begin(), v->childoffset.begin()+v->key.size()+1, write.childoffset);
				std::ofstream out("name",std::ios::out|std::ios::binary|std::ios::app);
				out.write((char *)&write, sizeof(write));
				out.close();
				if(v->child[i]!=nullptr)
				saveinfile(static_cast<BAddTreeLeafNode<K, E>*>(v->child[i]),name);
			}
			else{
				for (; i < _s+1; ++i) {
				write_for_binary write;
				std::copy(v->key.begin(), v->key.end(), write.key);
				write.isleaf=1;
				auto dec=v->e;
				for(int c=0;c<dec.size();c++)
				{
					write.offset[c]=dec[c]->file_offset;
				}
				std::copy(v->childoffset.begin(), v->childoffset.end(), write.childoffset);
				std::ofstream out("name",std::ios::out|std::ios::binary|std::ios::app);
				out.write((char *)&write, sizeof(write));
				out.close();
				if(v->child[i]!=nullptr)
				saveinfile(static_cast<BAddTreeLeafNode<K, E>*>(v->child[i]),name);
			}
			}
	}
	template<typename K, typename E>
	inline BAddTreeLeafNode<K, E>* BAddTree<K, E>::searchall()
	{
		BAddTreeLeafNode<K, E>*v = static_cast<BAddTreeLeafNode<K, E>*>(this->m_root);//c++式静态转换
		traverse(v);
		traversenew(v);
		std::ofstream outs("name",std::ios::out|std::ios::binary);
		outs.close();
		saveinfile(v,std::string("name"));
		//std::cout<<"success"<<std::endl;
		return v;
	}
	template<typename K, typename E>
	void BAddTree<K, E>::ReadBtree(int offset,std::ifstream& in)
	{
		while(!in.eof())
		{
			write_for_binary win;
			in.read((char *)&win, sizeof(win));
			int i=0;
			while(win.key[i]!=-114514)
			{
				E* e=new E(win.key[i],win.offset[i]);
				insert(*e);
				i++;
			}
		}
		in.close();
	}
	template<typename K, typename E>
	inline bool BAddTree<K, E>::insert(E const &e)
	{
		BAddTreeLeafNode<K, E>*v = this->search(e);
		if (v) {//不允许插入重复值 因为插入要上溢下溢可能导致一个节点的2边都存在重复值，导致查找太过复杂
			return false;
		}
		else {
			K k = e.key();
			v = static_cast<BAddTreeLeafNode<K, E>*>(this->m_hitParentNode);
			int _s = v->key.size();
			int i = 0;
			for (; i < _s; ++i) {
				K&tk = v->key[i];
				if (tk > k)break;
			}
			v->key.insert(v->key.begin()+i,k);//插入关键字
			++this->m_size;//更新规模
			v->child.insert(v->child.begin() + i + 1, nullptr);//插入空分支
			E*_e = new E();
			*_e =std::move(e);//使用右值移动
			v->e.insert(v->e.begin() + i, _e);//插入记录
			solveOverFlow(v);//上溢则分裂
			return true;
		}
	}

	template<typename K, typename E>
	inline bool BAddTree<K, E>::remove(E const &e)
	{
		BAddTreeLeafNode<K, E>*v = this->search(e);
		if (!v) {
			return false;
		}
		int rank = 0; while (v->key[rank] != e.key())++rank;
		//删除
		v->key.erase(v->key.begin() + rank);//删除关键字
		E*_e = v->e[rank];
		v->e.erase(v->e.begin() + rank);//删除记录
		--this->m_size;
		delete _e;//释放记录
		v->child.erase(v->child.begin() + rank + 1);//删除分支
		solveUnderFlow(v);//检测下溢
		return true;
	}

	template<typename K, typename E>
	template<typename Visist>
	inline void BAddTree<K, E>::list_traversal(Visist visit)
	{
		BAddTreeLeafNode<K, E>*p = m_header;
		while ((p=p->next) != m_trail) {
			visit(p->e);
		}
	}
	template<typename K, typename E>
	template<typename Visist>
	inline void BAddTree<K, E>::tree_traversal(Visist visit)
	{
		BAddTreeLeafNode<K, E>*p = m_header;
		while ((p=p->next) != m_trail) {
			visit(p->e);
			
			std::cout<<"|"<<std::endl;
		}
	}
	template<typename K, typename E>
	inline BAddTree<K, E>::BAddTree()
	{
		m_size = -1; m_order = -1; m_root = nullptr; m_hitParentNode = nullptr; m_header = nullptr; m_trail = nullptr;
	}

	template<typename K, typename E>
	inline void BAddTree<K, E>::solveOverFlow(BAddTreeLeafNode<K, E>*v)
	{
		while (v->child.size()>m_order)//上溢
		{
			int mid =m_order / 2;//分裂出去的父序
			BAddTreeLeafNode<K, E>*_r;
			if (v->isLeaf()) {//情况一 叶节点复制上溢点部分并更新链表
				_r= new BAddTreeLeafNode<K, E>();//注：默认有一个空分支
				//记录处理
				{
					_r->e.insert(_r->e.begin(), v->e.begin() + mid, v->e.end());
					v->e.erase(v->e.begin() + mid, v->e.end());
				}
				//关键字处理
				{
					_r->key.insert(_r->key.begin(), v->key.begin() + mid, v->key.end());
					v->key.erase(v->key.begin() + mid+1, v->key.end());
				}
				//分支处理
				{
					_r->child.insert(_r->child.begin(), v->child.begin() + mid, v->child.end() - 1);
					v->child.erase(v->child.begin() + mid + 1, v->child.end() - 1);
					_r->child[_r->child.size() - 1] = v->child[v->child.size() - 1];
					v->child.erase(v->child.end()-1);
				}
				//更新链表 
				v->next->last = _r; _r->next = v->next;
				_r->last = v; v->next = _r;
			}
			else {//情况二 内部节点不复制上溢点部分 且没有记录
				_r=static_cast<BAddTreeLeafNode<K,E>*>(new BAddTreeNode<K, E>());//注：默认有一个空分支
				//关键字处理
				{
					_r->key.insert(_r->key.begin(), v->key.begin() + mid+1, v->key.end());
					v->key.erase(v->key.begin() + mid+1, v->key.end());
				}
				//分支处理
				{
					_r->child.insert(_r->child.begin(), v->child.begin() + mid + 1, v->child.end() - 1);
					v->child.erase(v->child.begin() + mid + 1, v->child.end() - 1);
					_r->child[_r->child.size() - 1] = v->child[v->child.size() - 1];
					v->child.erase(v->child.end()-1);
				}
			}
			//共同部分 更新父节点
			{
				{
					int _s = _r->child.size();
					if (_r->child[0])//节点状态统一
						for (int i = 0; i < _s; ++i)
							_r->child[i]->parent = static_cast<BAddTreeNode<K, E>*>(_r);
				}
				BAddTreeNode<K, E>*p = v->parent;
				if (!p) {
					this->m_root = p = new BAddTreeNode<K, E>(); p->child[0] = v; v->parent = p;//上溢点为根，建立新根
				}
				K& k = v->key[v->key.size() - 1];//上溢码
				int i = 0; while (p->child[i] != v)++i;
				
				p->key.insert(p->key.begin()+i, k);//父节点码更新
				p->child.insert(p->child.begin()+ i + 1, _r);//父节点分支更新
				_r->parent = p;//向上联接
				v->key.erase(v->key.end() - 1);//残留码删除
				v = static_cast<BAddTreeLeafNode<K, E>*>(p);//上溢传播
			}
		}
	}

	template<typename K, typename E>
	inline void BAddTree<K, E>::solveUnderFlow(BAddTreeLeafNode<K, E>*v)
	{
		//和B树一样，不同的是叶子节点不用下拉父节点关键字
		size_t sep = (m_order+1) / 2;//下溢临界点
		while (v->child.size()<sep)//下溢
		{
			BAddTreeNode<K,E>*p = v->parent;
			if (!p) {//根节点最小关键码为1
				if (v->key.empty()&&v->child[0]) {//下溢到根含有唯一非空节点则处理
					this->m_root = v->child[0];
					this->m_root->parent = nullptr;
					if (v->isLeaf()) {//叶子节点则更新链表
						v->next->last = v->last; v->last->next = v->next;
						delete v, v = nullptr;
					}
					else {//内部节点析构
						BAddTreeNode<K, E>* _n = static_cast<BAddTreeNode<K, E>*>(v);
						delete _n, _n = nullptr;
					}
				}
				return;
			}

			int rank = 0;while (p->child[rank] != v)++rank;
			
			//分2块 叶子节点及内部节点 内部节点同B树 叶子节点则删掉B树中的下拉值或借一个同时替换掉父节点(注：替换)
			if (v->isLeaf()) {
				//优先向左合并 满足完全二叉树性
				//情况1：左可借
				if (rank > 0) {
					BAddTreeLeafNode<K, E>*_left = static_cast<BAddTreeLeafNode<K, E>*>(p->child[rank - 1]);//左兄弟
					if (_left->child.size() > sep) {
						v->child.insert(v->child.begin(), _left->child[_left->child.size() - 1]);//借一个分支
						_left->child.erase(_left->child.end() - 1);//删除
						{
							auto it = --_left->key.end();
							p->key[rank - 1] = *(it);//父节点关键字替换
							v->key.insert(v->key.begin(), *it);//借一个关键字
							_left->key.erase(it);//删除
						}
						{
							auto it = --_left->e.end();
							v->e.insert(v->e.begin(), *it);//借一个记录
							_left->e.erase(it);//删除
						}
						return;
					}
				}
				//情况2：右可借
				if (rank < p->child.size() - 1) {
					BAddTreeLeafNode<K, E>*_right = static_cast<BAddTreeLeafNode<K, E>*>(p->child[rank+1]);//右兄弟
					if (_right->child.size() > sep) {
						v->child.insert(v->child.end(), _right->child[0]);//借一个分支
						_right->child.erase(_right->child.begin());//删除
						{
							auto it =_right->key.begin()+1;
							p->key[rank ] = *(it--);//父节点关键字替换
							v->key.insert(v->key.end(), *it);//借一个关键字
							_right->key.erase(it);//删除
						}
						{
							auto it = _right->e.begin();
							v->e.insert(v->e.end(), *it);//借一个记录
							_right->e.erase(it);//删除
						}
						return;
					}
				}
				//情况3:向左或向右合并
				if (rank > 0) {//优先向左合并 满足完全二叉树性
					BAddTreeLeafNode<K, E>*_left = static_cast<BAddTreeLeafNode<K, E>*>(p->child[rank - 1]);//左兄弟
					v->child.erase(v->child.begin());//首先删除一个多余分支，这里根据右归属分支原则删除，当然也可以随便删除
					_left->child.insert(_left->child.end(), v->child.begin(), v->child.end());;//合并分支
					_left->key.insert(_left->key.end(), v->key.begin(), v->key.end());//合并关键码
					//删除父关键码(不下拉)
					p->child.erase(p->child.begin()+rank);
					p->key.erase(p->key.begin() + rank - 1);
					//合并记录
					_left->e.insert(_left->e.end(), v->e.begin(), v->e.end());
					v->e.resize(0);//安全析构
					//重建链表
					v->last->next = v->next; v->next->last = v->last;
					delete v, v = nullptr;
					v = static_cast<BAddTreeLeafNode<K, E>*>(p);//下溢传播
					continue;
				}
				else {
					BAddTreeLeafNode<K, E>*_right = static_cast<BAddTreeLeafNode<K, E>*>(p->child[rank +1]);//右兄弟
					_right->child.erase(_right->child.begin());//首先删除一个多余分支，这里根据右归属分支原则删除，当然也可以随便删除
					_right->child.insert(_right->child.begin(), v->child.begin(), v->child.end());;//合并分支
					_right->key.insert(_right->key.begin(), v->key.begin(), v->key.end());//合并关键码
					//删除父关键码(不下拉)
					p->child.erase(p->child.begin() + rank);
					p->key.erase(p->key.begin() + rank );
					//合并记录
					_right->e.insert(_right->e.begin(), v->e.begin(), v->e.end());
					v->e.resize(0);//安全析构
					//重建链表
					v->last->next = v->next; v->next->last = v->last;
					delete v, v = nullptr;
					v = static_cast<BAddTreeLeafNode<K, E>*>(p);//下溢传播
					continue;
				}
			}
			else {//内部节点
				//优先向左合并 满足完全二叉树性
					//情况1：左可借
				if (rank > 0) {
					BAddTreeNode<K,E>*_left = p->child[rank - 1];//左兄弟
					if (_left->child.size() > sep) {
						v->child.insert(v->child.begin(), _left->child[_left->child.size() - 1]);//借一个分支
						if (v->child[0])v->child[0]->parent = v;//向上联接
						_left->child.erase(_left->child.end() - 1);//删除
							auto it = --_left->key.end();
                            v->key.insert(v->key.begin(), p->key[rank - 1]);//借一个关键字
							p->key[rank - 1] = *(it);//父节点关键字替换
							_left->key.erase(it);//删除
						return;
					}
				}
				//情况2：右可借
				if (rank < p->child.size() - 1) {
					BAddTreeNode<K,E>*_right = p->child[rank + 1];//右兄弟
					if (_right->child.size() > sep) {
						v->child.insert(v->child.end(), _right->child[0]);//借一个分支
						if (v->child[v->child.size()-1])v->child[v->child.size() - 1]->parent = v;//向上联接
						_right->child.erase(_right->child.begin());//删除
							auto it = _right->key.begin();
							v->key.insert(v->key.end(), p->key[rank]);//借一个关键字
							p->key[rank] = *(it);//父节点关键字替换
							_right->key.erase(it);//删除
						return;
					}
				}
				//情况3:向左或向右合并
				if (rank > 0) {//优先向左合并 满足完全二叉树性
					BAddTreeNode<K,E>*_left = p->child[rank - 1];//左兄弟
					_left->child.insert(_left->child.end(), v->child.begin(), v->child.end());;//合并分支
					//向上联接
					{
						auto it = _left->child.end() - v->child.size();
						auto ite = _left->child.end();
						while (it!=ite)
						{
							if(*it)(*it)->parent = _left;
							++it;
						}
					}
					_left->key.insert(_left->key.end(),p->key[rank-1]);
					_left->key.insert(_left->key.end(), v->key.begin(), v->key.end());//合并关键码
					p->child.erase(p->child.begin() + rank);
					p->key.erase(p->key.begin() + rank - 1);
					{
						BAddTreeNode<K, E>*_t = static_cast<BAddTreeNode<K, E>*>(v);//析构内部节点
						delete _t, _t = nullptr;
					}
					v = static_cast<BAddTreeLeafNode<K, E>*>(p);//下溢传播
					continue;
				}
				else {
					BAddTreeNode<K,E>*_right = p->child[rank + 1];//右兄弟
					_right->child.insert(_right->child.begin(), v->child.begin(), v->child.end());;//合并分支
						//向上联接
					{
						auto it = _right->child.begin();
						auto ite = _right->child.begin()+v->child.size();
						while (it != ite)
						{
							if(*it)(*it)->parent = _right;
							++it;
						}
					}
					_right->key.insert(_right->key.begin(), p->key[rank]);
					_right->key.insert(_right->key.begin(), v->key.begin(), v->key.end());//合并关键码
					p->child.erase(p->child.begin() + rank);
					p->key.erase(p->key.begin() + rank);
					{
						BAddTreeNode<K, E>*_t = static_cast<BAddTreeNode<K, E>*>(v);//析构内部节点
						delete _t, _t = nullptr;
					}
					v = static_cast<BAddTreeLeafNode<K, E>*>(p);//下溢传播
					continue;
				}
			}
		}
	}

}
