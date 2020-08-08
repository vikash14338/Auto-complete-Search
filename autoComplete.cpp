#include <bits/stdc++.h>
#define ll long long
#define fio ios_base::sync_with_stdio(false); cin.tie(NULL),cout.tie(NULL);
using namespace std;


struct TrieNode{
  bool isEnd;
  ll search_count;
  TrieNode *ptr[26];
};
TrieNode* getNode()
{
  TrieNode *root=new TrieNode;
  root->isEnd=false;
  root->search_count=0;
  for(ll i=0;i<26;i++)
  {
    root->ptr[i]=NULL;
  }
  return root;
}



struct suggString{
    string s;
    ll order;

    suggString(string sugg,ll count)
    {
        s=sugg;
        order=count;
    }
};
// this comparator function is used order the suggestion in most frequent search manner
struct comp{
    bool operator()(suggString const& s1,suggString const& s2)
    {
        return s1.order>s2.order;
    }
};

// To get the most frequent search as a suggestion
priority_queue<suggString,vector<suggString>,comp> pq;

//Insert string in trie

void insert(TrieNode *root,const string s,ll last_matched)
{

  for(ll i=last_matched-1;s[i];i++)
  {
    ll index=s[i]-'a';
    if(!root->ptr[index])
    {
      root->ptr[index]=getNode();
    }
    root=root->ptr[index];
  }
  root->isEnd=true;
  root->search_count=1;
}

//To get suggestion for any string
void findSuggestion(TrieNode *root,string suggestedString)
{
    
    if(root->isEnd)
    {
        pq.push(suggString(suggestedString,root->search_count));
    }
    for(ll i=0;i<26;i++)
    {
        if(root->ptr[i])
        {
            char c=(i+'a');
            findSuggestion(root->ptr[i],suggestedString+c);
        }
    }
    return;
}

//return pointer to last matched character and index of that character
pair<TrieNode*,ll> search(TrieNode *root,string s)
{
    ll i;
    for(i=0;s[i];i++)
    {
        ll index=s[i]-'a';
        if(!root->ptr[index])
        {
          return {root,i+1};
        }
        root=root->ptr[index];
    }
    // check if string already in trie(isEnd==true) or so is a substring of a string(isEnd==false)
    if(!root->isEnd){
        root->search_count += 1;
        return {root,i};
    }

    return {root,i};
}

//Print trie
void printTrie(TrieNode *root,string s="")
{
    if(root->isEnd)
    {
        cout<<s<<"\n";
    }
    for(ll i=0;i<26;i++)
    {
        if(root->ptr[i])
        {
            char c=i+'a';
            printTrie(root->ptr[i],s+c);
        }
    }
    
}

int main()
{
    fio
  ll n;
  cin>>n;
  string s;
  TrieNode *root=getNode();
  while(n--)
  {
    cin>>s;
    //this re-initialization of priority_queue is done so that previously insert element is removed from pq 
    pq=priority_queue<suggString,vector<suggString>,comp>();

    //Whatever we want to do with string first search that if this string is partially or completely inserted in trie
    pair<TrieNode*,ll> p=search(root,s);

    // We only insert any string in trie if given string is not a substring of previously inserted string or 
    // not previously inserted in trie 
    if(p.second==s.length())
    {
        if(p.first->isEnd)
        {
            cout<<"String present"<<"\n";
        }else{
            // string is present an substring in trie, so we also mark this as string 
            cout<<"String is present as substring"<<"\n";
            p.first->isEnd=true;
        }

        // p.first has the pointer of last matched character of a string
        findSuggestion(p.first,s);

        //Print most frequent search string first
        while(!pq.empty())
        {
            suggString s1=pq.top();
            cout<<s1.s<<" \n";
            pq.pop();
        }
    }else{
        //Only that part of string is inserted which is not already present in trie
        insert(p.first,s,p.second);
        cout<<"New string inserted"<<" \n";
    }
    
  }

    // if you want to print the whole trie
    printTrie(root);
  return 0;
}
