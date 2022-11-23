#include "AVL.h"
#ifndef _CRONE_H_
#include "Crone.h"
#endif
class Index
{
public:
    Index()
    {
        root=nullptr;
    }
    ~Index()
    {

    }
    void destroy(node_t* root) {
	if(root == NULL) {
		return;
	}
 
	root->left = NULL;
	root->right = NULL;
	root = NULL;
    }
    node_t *avl_insert(int val){
       root= avl_insert_real(val,root);
    }
    node_t *avl_insert_real(int val,node_t* root)
    {
        if (root == nullptr) //再加入val之前，该树为空
        {
            root=new node_t();
            root->left =root->right = root->parent = nullptr;
            root->height = 1;
            root->val = val;
            return root;
        }
        else if (val <= root->val)
        {
            root->left = avl_insert_real(val,root->left); //通过嵌套函数完成了插入了
            int lh, rh;
            lh = root->left->height; //因为val比root->val小，所以左节点不可能为空
            rh = root->right ? root->right->height : 0;
            if (lh - rh > 1)
            {
                if (val <= root->left->val)
                {
                    LL(&root);
                }
                else
                {
                    LR(&root);
                }
            }
        }
        else if (val > (root->val))
        {
            root->right = avl_insert_real(val,root->right);
            int lh, rh;
            rh = root->right->height; //因为val比root->val大，所以右节点不可能为空
            lh = root->left ? root->left->height : 0;
            int dh = lh - rh; // dh就是高度因子
            if (dh < -1)
            {
                if (val > root->right->val)
                {
                    RR(&root);
                }
                else
                {
                    RL(&root);
                }
            }
        }
        upgrade(root); //最后的最后再来更新root对应的height；
        return root;
    }
    node_t *avl_delete(int val)
    {
        avl_delete_real(val,root);
    }
    node_t* avl_delete_real(int val, node_t* root) {
	// 根为空 或者 没有要删除的节点，直接返回NULL
	if(root == nullptr) {
		return nullptr;
	}
 
	// 待删除的节点在root的左子树
	if(val < root->val) {
		root->left = avl_delete_real(val, root->left);
		// 删除节点后，若AVL树失去平衡，则进行相应的调节
		if(abs(root->right->height - root->left->height) == 2) {
			node_t* p = root->right;
		
			if(p->left->height >p->right->height) {
				RL(&root);
			} else {
			    RR(&root);
			}
		}
	// 待删除的节点在root的右子树
	} else if(val > root->val) {
		root->right = avl_delete_real(val, root->right);
		// 删除节点后，若AVL树失去平衡，则进行相应的调节
        if((root->left==nullptr)||(root->right==nullptr))
        {
            return root;
        }
		if(abs(root->left->height - root->right->height) == 2) {
			node_t* p = root->left;
			if(p->right->height > p->left->height) {
				LR(&root);
			} else {
				LL(&root);
			}
		}
	// root就是要删除的节点
	} else if(val == root->val) {
		//左右孩子非空
		if(root->left && root->right) {
 
			if(root->left->height > root->right->height) {
				// 如果root的左子树比右子树高；
                // 则找出root的左子树中的最大节点
                //   将该最大节点的值赋值给root
                //   删除该最大节点
                // 这类似于用root的左子树中最大节点做root的替身
                // 删除root的左子树中最大节点之后，AVL树仍然是平衡的
				node_t* max = getMaxNum(root->left);
				root->val = max->val;
				root->left = avl_delete_real(max->val, root->left);
			} else {
				// 如果root的左子树比右子树低；
                // 则找出root的左子树中的最小节点
                //   将该最小节点的值赋值给root
                //   删除该最小节点
                // 这类似于用root的右子树中最小节点做root的替身
                // 删除root的左子树中最小节点之后，AVL树仍然是平衡的
				node_t* min = getMinNum(root->right);
				root->val = min->val;
				root->right = avl_delete_real(min->val, root->right);
			}
		}
 
		else {
			//这种情况为左孩子为空右孩子不为空、或者右孩子为空左孩子不为空、左右孩子都为空时的处理方法
			//直接通过一个三目运算，即可完美解决
			node_t* temp = root;
	
			root = root->left ? root->left : root->right;
		
			//destroy(temp);
		}
	}
 
	return root;
    }
    void showBtreeByLeft()
    {
        showBtreeByLeft_real(this->root);
    }
    void showBtreeByLeft_real(node_t* root) {  
    if(root == NULL) {  
        std::cout<<-1<<std::endl;  
        return;  
    }  
 
    std::cout<<root->val<<std::endl;  
    showBtreeByLeft_real(root->left);  
    showBtreeByLeft_real(root->right);  
}
private:
    node_t* root;
    void upgrade(node_t *root)
    {
        int h = 1;
        if (root == nullptr)
        {
            return;
        }
        if (root->left != nullptr) //如果左右为空的话，就是叶子结点，此时h就等于1
        {
            int lh = root->left->height + 1; //从左子树算出来的高度，与之后从右子树算出来的高度，取最大值就是root点的最大高度。
            h = lh;
            root->left->parent = root; //更新parent结点(这个本身也可以放在之后改结点的时候再做，此时做也可以，防止后面被绕晕)
        }
        if (root->right != nullptr)
        {
            int rh = root->right->height + 1;
            h = rh > h ? rh : h;
            root->right->parent = root;
        }

        root->height = h;
    }
    //因为不是在只读t结点了，而是要去改t结点的位置了，故要把结果保留下来就要用到指向指针的指针
    void LL(node_t **t)
    {
        node_t *tmp = (*t)->left;
        (*t)->left = tmp->right;
        tmp->right = (*t);
        (*t) = tmp;                 //因为不是在只读t结点了，而是要去改t结点的位置了(t结点要一直保持是那个root不能改变，免得动一个要动全身)，故要把结果保留下来就要用到指向指针的指针
        upgrade((*t)->right->left); //该结点高度没变但是parent结点变了，故而要更新
        upgrade((*t)->right);
        upgrade((*t));
    }
    void RR(node_t **t)
    {
        node_t *tmp = (*t)->right;
        (*t)->right = tmp->left;
        tmp->left = (*t);
        (*t) = tmp;
        upgrade((*t)->left->right);
        upgrade((*t)->left);
        upgrade((*t));
    }
    void LR(node_t **t)
    {
        RR(&((*t)->left));
        LL(t);
    }
    void RL(node_t **t)
    {
        LL(&((*t)->right));
        RR(t);
    }
    node_t* getMaxNum(node_t* root) {
	if(root == nullptr) {
		return nullptr;
	}
 
	while(root->right != nullptr) {
		root = root->right;
	}
	return root;
    }
    node_t* getMinNum(node_t* root) {
	if(root == nullptr) {
		return nullptr;
	}
 
	while(root->left != nullptr) {
		root = root->left;
	}
	return root;
    }
};
int main() {
	node_t* root;
    Index* index= new Index();
	index->avl_insert(10);
	index->avl_insert(25);
	index->avl_insert(8);
    index->avl_insert(24);
	index->avl_insert(30);
	index->avl_insert(29);
    index->avl_delete(24);
	index->showBtreeByLeft();
}