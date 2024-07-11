#include<bits/stdc++.h>
using namespace std;
class Node
{
public:
    int data;
    Node *right,*left,*parent;
    string color;
    Node (int data)
    {
        this->data=data;
        left=right=parent=NULL;
        this->color="red";
    }
};
class RBT
{
public:
    Node *root;
    RBT()
    {
        root=NULL;
    }
    void inorder(Node *root)
    {
        if (root == NULL) return ;
        inorder(root->left);
        cout <<"  "<< root->data <<"<"<<root->color<<">  ";
        inorder(root->right);
    }

    Node *Insert(Node *root, Node *newItem)
    {
        if(root==NULL) return newItem;
        else if(newItem->data<root->data)
        {
            root->left=Insert(root->left,newItem);
            root->left->parent=root;
        }
        else
        {
            root->right =Insert(root->right,newItem);
            root->right ->parent=root;
        }
        return root;
    }
    Node *RightRotate(Node *root,Node *x)
    {
        Node *x_left = x->left; /// set x_left
        x->left = x_left->right;  /// set new x->l
        if (x->left != NULL)  x->left->parent = x;
        x_left->parent=x->parent;   /// setting the parent
        if(x->parent==NULL) root=x_left;  /// root changes
        else if(x==x->parent->left) x->parent->left=x_left;
        else x->parent->right = x_left;
        x_left->right =x;
        x->parent=x_left;
        return root;
    }
    Node *LeftRotate(Node *root, Node *x)
    {
        Node *x_right=x->right;
        x->right =x_right->left;
        if(x->right !=NULL) x->right ->parent=x;
        x_right->parent=x->parent;
        if (x->parent == NULL)
            root = x_right;
        else if (x == x->parent->left)
            x->parent->left = x_right;
        else
            x->parent->right = x_right;
        x_right->left = x;
        x->parent = x_right;
        return root;
    }
    Node *fixTree(Node *root, Node *newItem)
    {
        Node *pt=NULL;
        Node *gp=NULL;
        while(newItem!=root && newItem->parent->color=="red" && newItem->color=="red")
        {
            pt=newItem->parent;
            gp=pt->parent;
            /*  Case : A
            Parent of pt is left child of Grand-parent of pt */
            if(pt==gp->left)
            {
                Node *uncle=gp->right ;
                /// uncle of newItem is red: do recoloring
                if(uncle!=NULL && uncle->color=="red")
                {
                    pt->color="black";
                    uncle->color="black";
                    gp->color="red";
                    newItem=gp;
                }
                else
                {
                    if(newItem==pt->right)
                    {
                        root = LeftRotate(root,pt);
                        newItem = pt;
                        pt=newItem->parent;
                    }
                    /// if newItem is left child of tempParent:
                    if(newItem==pt->left)
                    {
                        root = RightRotate(root,gp);
                        swap(gp->color, pt->color);
                        newItem=pt;
                    }
                }
            }
            else
            {
                Node *uncle=gp->left;
                /// uncle of newItem is red: do recoloring only
                if(uncle!=NULL && uncle->color=="red")
                {
                    pt->color="black";
                    uncle->color="black";
                    gp->color="red";
                    newItem=gp;
                }
                else
                {
                    if(newItem==pt->left)
                    {
                        root=RightRotate(root,pt);
                        newItem=pt;
                        pt=newItem->parent;
                    }
                    if(newItem==pt->right)
                    {
                        root=LeftRotate(root,gp);

                        swap(pt->color,gp->color);

                        newItem=pt;
                    }
                }
            }
        }
        root->color="black";
        cout<<"  root: "<<root->data<<"<"<<root->color<<">"<<endl<<endl;
        return root;
    }
    Node *RBT_Insert(int element)
    {
        Node *newItem=new Node(element);
        root=Insert(root, newItem);
        root=fixTree(root,newItem);
        return root;
    }
     Node *Delete(Node *root, Node *temp, int element)
    {
        if(temp!=NULL)
        {
            if(element<temp->data)
            {
                root=Delete(root,temp->left,element);
            }
            else if(element>temp->data)
            {
                root=Delete(root,temp->right,element);
            }
            else
            {
                ///case 1 No Child :
                if(temp->left==NULL && temp->right==NULL)
                {
                    Node *t=temp;
                    if(temp->color=="black")
                    {
                        root=fixDoubleBlack(root,temp);
                        //cout<<"here root is "<<root->data<<endl;
                    }
                    ///if temp->color =="red" just delete it
                    if(temp->parent->right ==temp) temp->parent->right=NULL;
                    else if(temp->parent->left==temp) temp->parent->left=NULL;
                    delete(t);
                    return root;
                }
                /// case 2 (One Child):
                if(temp->left==NULL && temp->right!=NULL)
                {
                    int mn=findMin(temp->right);
                    int rp;
                    temp->data=mn;
                    root=Delete(root,temp->right,mn);
                }
                if(root->right==NULL && root->left!=NULL)
                {
                    int mx=findMax(temp->left);
                    temp->data=mx;
                    root=Delete(root,temp->left,mx);
                }
                /// case 3: Two Child
                else if(root->right!=NULL && root->left!=NULL)
                {
                    int mx=findMax(temp->left);
                    temp->data=mx;
                    root=Delete(root,temp->left,mx);
                }
            }
        }
        return root;
    }
    Node *deleteByVal(Node *root,int n)
    {
        if(root==NULL)
        {
            cout<<"tree is empty ";
            return root;
        }
        Node *ans=Search(root,n);
        if(ans==NULL)
        {
            cout<<n <<" The value is not find in the tree."<<endl;
            return root;
        }
        root=Delete(root,root,n);
        return root;

            }

    Node *getSibling(Node *DB)
    {
        // sibling null if no parent
        Node *parent=DB->parent;
        if (parent == NULL)
            return NULL;
        if (DB==parent->left)  return parent->right;

        else return parent->left;
    }
    Node *getRedChild(Node *x)
    {
        if(x->left!=NULL && x->left->color=="red") return x->left;
        else if(x->right !=NULL && x->right ->color=="red") return x->right;
        return NULL;
    }
    bool checkFarRedChild(Node *DB, Node *sibling, Node *rc, Node *parent)
    {
        if((DB==parent->right && rc==sibling->left)||
                (DB==parent->left && rc==sibling->right)) return true;
        else return false;
    }
    Node *fixDoubleBlack(Node *root,Node *DB)
    {
        Node *sibling=getSibling(DB), *parent=DB->parent;
        if(DB==root) return root; /// case
        if(sibling==NULL)
        {
            if(parent->color=="red") parent->color="black";
            if(parent->color=="black") root=fixDoubleBlack(root,parent);
            return root;
        }
        else if(sibling->color=="black")
        {
            Node *rc=getRedChild(sibling);
            if(rc==NULL)  /// case
            {
                if(parent->color=="red")
                {
                    parent->color="black";
                    sibling->color="red";
                    return root;
                }
                if(parent->color=="black")
                {
                    sibling->color="red";
                    root=fixDoubleBlack(root,parent);
                    return root;
                }
            }
            else if(checkFarRedChild(DB,sibling,rc,parent)) ///case
            {
                swap(parent->color,sibling->color);
                if(DB==parent->right ) root=RightRotate(root, parent);
                else root=LeftRotate(root,parent);
                rc->color="black";
                return root;
            }
            else        ///case
            {
                swap(sibling->color, rc->color);
                if(DB==parent->right) root=LeftRotate(root,sibling);
                else root=RightRotate(root,sibling);
                root=fixDoubleBlack(root,DB);
                return root;
            }
        }
        else if(sibling->color=="red") /// case
        {
            swap(parent->color,sibling->color);
            if(DB==parent->left) root=LeftRotate(root,parent);
            else root=RightRotate(root,parent);
            cout<<"root->>> data"<<root->data<<endl;
            root=fixDoubleBlack(root,DB);
            return root;
        }
    }
    int findMax(Node *temp)
    {
        while(temp->right !=NULL)
        {
            temp=temp->right;
        }

        return temp->data;
    }
    int findMin(Node *temp)
    {
        while(temp->left!=NULL)
        {
            temp=temp->left;
        }
        return temp->data;
    }
    Node *Search(Node *temp, int data)
    {
        while(temp!=NULL)
        {
            if(temp->data== data)
            {
                return temp;
            }
            else if(data<temp->data)
            {
                return Search(temp->left,data);
            }
            else if(data >temp->data)
            {
                return Search(temp->right,data);
            }
        }
        return NULL;
    }
};

int main()
{
    RBT *b=new RBT();
    while(1)
    {
        cout<<" \n\t# RED BLACK TREE #\n\n ";
        cout<<"   1.Insertion\n";
        cout<<"   2.Inorder\n" ;
        cout<<"   3.Find Maximum \n";
        cout<<"   4.Find Minimum \n";
        cout<<"   5.Search an Element \n";
        cout<<"   6.Deletion\n";
        cout<<"   7.Exit\n";
        cout<<"\n  Enter the choice: ";
        int choice;
        int element;
        cin>>choice;
        switch(choice)
        {
        case 1:
            cout<<"  Enter the element to insert: ";
            cin>>element;
            b->root=b->RBT_Insert(element);
            break;
        case 2:
            cout<<"  The Inorder Red Black Tree traversal: "<<endl;
            b->inorder(b->root);
            cout<<endl<<endl;
            break;

        case 3:

        {
            int mx=b->findMax(b->root);
            cout<<"  The maximum value in this tree is : "<<mx<<endl<<endl;
            break;
        }
        case 4:
        {
            int mn=b->findMin(b->root);
            cout<<"  The minimum value in this tree is : "<<mn<<endl<<endl;
            break;
        }
        case 5:
        {
            cout<<"  Which element do you want to search: ";
            cin>>element;
            Node *ans=b->Search(b->root,element);
            if(ans==NULL)
            {
                cout<<"  There is no such element in this tree"<<endl;
                break;
            }
            else cout<<"  Element: "<<element<<"present"<<endl;
            cout<<"Do you want to know the color of this element?";
            cout<<"  1.YES    2.NO"<<endl<<endl;
            string a;
            cin>>a;
            if(a=="1") cout<<"color: "<<ans->color<<endl;
            break;
        }
        case 6:
            cout<<" Enter element to be deleted: ";
            cin>>element;
            b->root=b->deleteByVal(b->root,element);
            break;
        case 7:
            cout<< "\n\t* End Program! *" <<endl;
            return 0;
        }
    }
}








