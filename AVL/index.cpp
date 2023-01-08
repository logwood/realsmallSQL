#include "AVL.h"
#include <string.h>
class Index
{
public:
    Index(std::string paths)
    {
        path=paths;
        root = nullptr;
    }
    ~Index()
    {
    }
    void destroy(node_t *root)
    {
        if (root == NULL)
        {
            return;
        }

        root->left = NULL;
        root->right = NULL;
        root = NULL;
    }
    node_t *avl_insert(int val, std::vector<std::string> strings)
    {
        root = avl_insert_real(val, root, strings);
    }

    node_t *avl_delete(int val)
    {
        avl_delete_real(val, root);
    }

    void showBtreeByLeftAndWrite()
    {
        std::ofstream fie("idx/"+path,std::ios::binary);
        fie.close();
        std::ofstream file("idx/"+path,std::ios::binary|std::ios::app);
        int nums=root->strs.size();
        file.write((char*)&nums,sizeof(int));
        file.close();
        showBtreeByLeftAndWrite_real(this->root);
    }
    std::vector<std::vector<std::string>> search(int key,int type)
    {
        return searchall(key,type,root);
    }    
    std::vector<std::vector<std::string>> searchall(int key,int type,node_t* root)
    {
        std::vector<std::vector<std::string>> ptrs;
        if(root==nullptr)
        {
            return ptrs;
        }
        else{
            if(root->val>key){
                ptrs.emplace_back(root->strs);
            }
            else{

            }
            auto ptr=searchall(key,type,root->left);
            ptrs.insert(ptrs.end(),ptr.begin(),ptr.end());
            ptr=searchall(key,type,root->right);
            ptrs.insert(ptrs.end(),ptr.begin(),ptr.end());
        }
        return ptrs;
    }
    void readInHex()
    {
        std::ifstream file("idx/"+path,std::ios::binary|std::ios::out);
        int nums;
        file.read((char*)&nums,sizeof(int));
        std::vector<std::vector<std::string>> infos;
        int i=0;
        std::vector<int> heights;
        std::vector<int> vals;
        bool isgood=true;
        while (!file.eof())
        {
            infos.emplace_back();
            for(int j=0;j!=nums;j++)
            {
                int size=0;
                file.read((char*)&size,sizeof(int));
                if(size==-2)
                {
                    infos.emplace_back();
                    j=-1;
                    i++;
                    isgood=false;
                }
                else{
                    char buf[MAX_SIZE];
                    memset(buf,'\0',MAX_SIZE);
                    file.read((char*)buf,size);
                    std::string sts(buf);
                    infos[i].emplace_back(sts);
                    file.read((char*)&size,sizeof(int));
                    file.read((char*)&size,sizeof(int));
                    isgood=true;
                }
            }
        if(isgood==true){
        int size;
            file.read((char*)&size,sizeof(int));
            heights.emplace_back(size);
            file.read((char*)&size,sizeof(int));
            vals.emplace_back(size);
        }
        i++;
        }
       root=saveiterator(root,infos,heights,vals);
       int j=0;
    }

private:
    node_t *root;
    std::string path="";
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
    node_t *getMaxNum(node_t *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        while (root->right != nullptr)
        {
            root = root->right;
        }
        return root;
    }
    node_t *getMinNum(node_t *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        while (root->left != nullptr)
        {
            root = root->left;
        }
        return root;
    }    
    node_t* saveiterator(node_t* node,std::vector<std::vector<std::string>> &infos,std::vector<int> &height,std::vector <int>& vals)
    {
        if(infos.size()<=1)
        {
            node=nullptr;
            return node;
        }
        else if(infos.front().size()==0)
        {
            node=nullptr;
            infos.erase(infos.begin());
            return node;
        }
        else
        {        
        if (node == nullptr) //再加入val之前，该树为空
            {
            node = new node_t();
            node->left = node->right = node->parent = nullptr;
            int i=0;
            int j=0;
            }
            for (int i=0;i<infos.front().size();i++)
            {
                node->strs.emplace_back(infos.front()[i]);
            }
            node->height=height.front();
            node->val=vals.front();
            infos.erase(infos.begin());
            height.erase(height.begin());
            vals.erase(vals.begin());
            if(node->left==nullptr)
            node->left=new node_t();
            if(node->right==nullptr)
            node->right=new node_t();
            node->left->parent=node;
            node->right->parent=node;
    }
       node->left= saveiterator(node->left,infos,height,vals);
      node->right= saveiterator(node->right,infos,height,vals);
      return node;
    }
    void showBtreeByLeftAndWrite_real(node_t *root)
    {
        if (root == NULL)
        {
        std::ofstream file("idx/"+path,std::ios::binary|std::ios::app);
        int size=-2;
        file.write((char*)&size,sizeof(int));
        file.close();
            return;
        }
        else{
        std::ofstream file("idx/"+path,std::ios::binary|std::ios::app);
        for(int j=0;j<root->strs.size();j++)
        {
            int size=root->strs[j].size();
            file.write((char*)&size,sizeof(int));
            file.write(root->strs[j].data(),root->strs[j].size());
            size=root->height;
            file.write((char*)&size,sizeof(int));
            size=root->val;
            file.write((char*)&size,sizeof(int));
        }
        int size=root->height;
            file.write((char*)&size,sizeof(int));
        size=root->val;
            file.write((char*)&size,sizeof(int));
        file.close();
        std::cout << root->val << root->strs[0] << std::endl;
        }
        showBtreeByLeftAndWrite_real(root->left);
        showBtreeByLeftAndWrite_real(root->right);
    }
    node_t *avl_delete_real(int val, node_t *root)
    {
        // 根为空 或者 没有要删除的节点，直接返回NULL
        if (root == nullptr)
        {
            return nullptr;
        }

        // 待删除的节点在root的左子树
        if (val < root->val)
        {
            root->left = avl_delete_real(val, root->left);
            // 删除节点后，若AVL树失去平衡，则进行相应的调节
            if (abs(root->right->height - root->left->height) == 2)
            {
                node_t *p = root->right;

                if (p->left->height > p->right->height)
                {
                    RL(&root);
                }
                else
                {
                    RR(&root);
                }
            }
            // 待删除的节点在root的右子树
        }
        else if (val > root->val)
        {
            root->right = avl_delete_real(val, root->right);
            // 删除节点后，若AVL树失去平衡，则进行相应的调节
            if ((root->left == nullptr) || (root->right == nullptr))
            {
                return root;
            }
            if (abs(root->left->height - root->right->height) == 2)
            {
                node_t *p = root->left;
                if (p->right->height > p->left->height)
                {
                    LR(&root);
                }
                else
                {
                    LL(&root);
                }
            }
            // root就是要删除的节点
        }
        else if (val == root->val)
        {
            //左右孩子非空
            if (root->left && root->right)
            {

                if (root->left->height > root->right->height)
                {
                    // 如果root的左子树比右子树高；
                    // 则找出root的左子树中的最大节点
                    //   将该最大节点的值赋值给root
                    //   删除该最大节点
                    // 这类似于用root的左子树中最大节点做root的替身
                    // 删除root的左子树中最大节点之后，AVL树仍然是平衡的
                    node_t *max = getMaxNum(root->left);
                    root->val = max->val;
                    root->strs = max->strs;
                    root->left = avl_delete_real(max->val, root->left);
                }
                else
                {
                    // 如果root的左子树比右子树低；
                    // 则找出root的左子树中的最小节点
                    //   将该最小节点的值赋值给root
                    //   删除该最小节点
                    // 这类似于用root的右子树中最小节点做root的替身
                    // 删除root的左子树中最小节点之后，AVL树仍然是平衡的
                    node_t *min = getMinNum(root->right);
                    root->val = min->val;
                    root->strs = min->strs;
                    root->right = avl_delete_real(min->val, root->right);
                }
            }

            else
            {
                //这种情况为左孩子为空右孩子不为空、或者右孩子为空左孩子不为空、左右孩子都为空时的处理方法
                //直接通过一个三目运算，即可完美解决
                node_t *temp = root;

                root = root->left ? root->left : root->right;

                // destroy(temp);
            }
        }

        return root;
    }
    node_t *avl_insert_real(int val, node_t *root, std::vector<std::string> const strings)
    {
        if (root == nullptr) //再加入val之前，该树为空
        {
            root = new node_t();
            root->left = root->right = root->parent = nullptr;
            root->height = 1;
            root->val = val;
            root->strs = strings;
            return root;
        }
        else if (val <= root->val)
        {
            root->left = avl_insert_real(val, root->left, strings); //通过嵌套函数完成了插入了
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
            root->right = avl_insert_real(val, root->right, strings);
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
};
 int main()
 {
      node_t *root;
      Index *index = new Index("person");
      index->avl_insert(10, {{"hello", "helex", "hllx"}});
      index->avl_insert(25, {{"hello", "helex", "hllx"}});
      index->avl_insert(8, {{"hello", "fa", "hllx"}});
      index->avl_insert(24, {{"sdkfdsjkfjksdds", "fa", "hllx"}});
      index->avl_insert(30, {{"fsdljdf", "ssds", "hllx"}});
      index->avl_insert(29, {{"dfkdsjkflsd", "hkjdsd", "hllx"}});
      index->avl_delete(24);
      index->showBtreeByLeftAndWrite();
      Index *ind =new Index("person");
      ind->readInHex();
      ind->avl_insert(24, {{"sdkfdsjkfjksdds", "fa", "hllx"}});
      auto inde=ind->search(10,2);
 }