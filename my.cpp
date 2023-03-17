#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<list>

// #include<time.h>
// #include<bits/stdc++.h>

using namespace std;


class trnode{
public:
    trnode *child[26];
    trnode(){
        isend=false;
        for(int i=0;i<26;i++){
            child[i]=nullptr;
        }
    }
    bool isend;
    friend class tries;
};

class tries{
public:
    trnode *root,*sroot;
    string title;

    tries(){
        root=new trnode;
        sroot=new trnode;
        title={};
    }

    void insert(const string& str){
        int len=str.length();
        trnode *tmp=root;
        int index;
        for(int i=0;i<len;i++){
            index=tolower(str[i])-'a';
            if(tmp->child[index]==nullptr){
                tmp->child[index]=new trnode;
            }
            tmp=tmp->child[index];
        }
        tmp->isend=true;

        tmp=sroot;        
        for(int i=len-1;i>=0;i--){
            index=tolower(str[i])-'a';
            if(tmp->child[index]==nullptr){
                tmp->child[index]=new trnode;
            }
            tmp=tmp->child[index];
        }

        
        // string q={};
        // for(int j=len-1;j>=0;j--){
        //     q=q+str[j];
        // }
        //cout<<q<<endl;        
        // insert2(q);
    }

    void insert2(const string& str){
        int len=str.length();
        trnode *tmp=sroot;
        int index;
        for(int i=0;i<len;i++){
            index=tolower(str[i])-'a';
            if(tmp->child[index]==nullptr){
                tmp->child[index]=new trnode;
            }
            tmp=tmp->child[index];
        }
        tmp->isend=true;
    }

    bool search(const string &str,const int &way){
        if(way==0){//exact
            trnode *tmp=root;
            int len=str.size()-2;
            for(int i=0;i<len;i++){
                int index=tolower(str[i+1])-'a';
                if(tmp->child[index]==nullptr){
                    return false;
                }
                else tmp=tmp->child[index];
            }
            return tmp->isend;
        }
        else if(way==1){//noprob
            trnode *tmp=root;
            int len=str.size();
            for(int i=0;i<len;i++){
                int index=tolower(str[i])-'a';
                if(tmp->child[index]==nullptr){
                    return false;
                }
                else tmp=tmp->child[index];
            }
            return true;
        }
        else{//fuck
            trnode *tmp=sroot;
            int len=str.size()-2;
            for(int i=len;i>0;i--){
                int index=tolower(str[i])-'a';
                if(tmp->child[index]==nullptr){
                    return false;
                }
                else tmp=tmp->child[index];
            }
            return true;
        }
    }

    void printitle(){
        cout<<title<<endl;
    }
};



// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}


int main(int argc, char *argv[]){
    // double START,END; START = clock();


    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	// Read File & Parser Example


	string file, title_name, tmp;
	fstream fi,fq;
    std::ofstream ans(output, std::ofstream::out);
	vector<string> tmp_string;


    list<tries*> treelist;

	// from data_dir get file ....
	// eg : use 0.txt in data directory
    while(true){        
        static int i=0;
        fi.open(data_dir+to_string(i)+".txt",ios::in);
        if(!fi) break;
        tries *nt=new tries;
        
         // GET TITLENAME
        getline(fi, title_name);
        nt->title=title_name;
        // GET TITLENAME WORD ARRAY
        tmp_string = split(title_name, " ");
        vector<string> title = word_parse(tmp_string);
        for(auto &word : title){
            nt->insert(word);
        }
        // GET CONTENT LINE BY LINE
        while(getline(fi, tmp)){
            // GET CONTENT WORD VECTOR
            tmp_string = split(tmp, " ");
            // PARSE CONTENT
            vector<string> content = word_parse(tmp_string);
            for(auto &word : content){
                // cout << word << endl;
                nt->insert(word);
            }
            //......
        }
        treelist.push_back(nt);
        i++;
        fi.close();
    }

    fq.open(query,ios::in);//open query
	while(getline(fq, tmp)){    
		tmp_string = split(tmp, " ");
        bool allnotfound=true;
        std::list<tries*>::iterator it;
        for(it=treelist.begin();it!=treelist.end();it++){ 
            bool have,havenxt;
            bool operate;    
            bool first=true;
            for(auto &word : tmp_string){   
                if(word=="+") operate=true;
                else if(word=="/") operate=false;
                else{
                    if(first){
                        if(word[0]=='"'){                            
                            have=(*it)->search(word,0);
                        }
                        else if(word[0]=='*'){
                            have=(*it)->search(word,2);
                        }
                        else{
                            have=(*it)->search(word,1);
                        }
                        first=false;
                    }
                    else {
                        if(word[0]=='"'){                            
                            havenxt=(*it)->search(word,0);
                        }
                        else if(word[0]=='*'){
                            havenxt=(*it)->search(word,2);
                        }
                        else{
                            havenxt=(*it)->search(word,1);
                        }
                        if(operate) have=have&&havenxt;
                        else have=have||havenxt;
                    }
                }
            }
            if(have){
                allnotfound=false;
                ans<<(*it)->title<<endl;
            }
        }
        if(allnotfound) ans<<"Not Found!"<<endl;
	}
    

    // CLOSE FILE
	fq.close();
    ans.close();

    
    // END = clock();
    // cout << (END - START) / CLOCKS_PER_SEC << endl;
}


// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
