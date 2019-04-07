#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<stack>
#include<algorithm>
#include<ctime>

//choose mode 
#define Mode 2
#define ram 0
using namespace std; 

struct puzzle{
	int x ;
	int y ;
	int len ;
	int dir ;
};

struct connect{
	int my ;
	int you ;
	int youname ;
};



struct va{
	int num ;
	string word ;
};

struct pending{
	int youname ;
	int you ;
	int my ;
};

struct p{
	int degree ;
	int num ;
};


int nodecnt = 0;
vector<vector<string>> dict ;
vector<vector<puzzle>> vecvecpuzzle ;

bool cmp(const p &lhs , const p &rhs){
    return lhs.degree > rhs.degree;
}

int myrandom (int i) { return rand()%i; }

void init(){
	vector<string> tmpdict ;
	ifstream indict( "English words 3000.txt" );
	ifstream inpuzz( "puzzle.txt" );
	int maxlen = 0 ;

	while( indict.peek()!=EOF ){
		string tmp ;
		indict >> tmp ;
		if ( maxlen < tmp.length() ) maxlen = tmp.length();
		tmpdict.push_back(tmp);
	}

	
	vector<string> ttmpdict ;
	for ( int i = 0 ; i < maxlen+1 ; i++ ){
		dict.push_back(ttmpdict);
	}
	for ( int i = 0 ; i < tmpdict.size() ; i++ ){
		dict[tmpdict[i].length()].push_back(tmpdict[i]);
	}
	
	

	string tmp ;
	vector<string> tmpvec ;
	while(getline(inpuzz,tmp)){
		tmpvec.push_back(tmp);
	}
	
	for ( int i = 0 ; i < tmpvec.size() ; i++ ){
		puzzle tmppuzzle ;
		vector<puzzle> vecpuzzle ;
		istringstream ss(tmpvec[i]);

		string ttmp ;
		int cnt = 0 ;
		while(getline(ss,ttmp,' ')){
			if ( ttmp == " " || ttmp == "" ) continue ;
			stringstream string_to_int;
			if ( (cnt%4) == 0 ){
				int tmpint ;
				string_to_int << ttmp ;
				string_to_int >> tmpint ;
				tmppuzzle.x = tmpint ;
			}else if ( (cnt%4) == 1 ){
				int tmpint ;
				string_to_int << ttmp ;
				string_to_int >> tmpint ;
				tmppuzzle.y = tmpint ;
			}else if ( (cnt%4) == 2 ){
				int tmpint ;
				string_to_int << ttmp ;
				string_to_int >> tmpint ;
				tmppuzzle.len = tmpint ;
			}else {
				if ( ttmp == "A" ) 
					tmppuzzle.dir = 0 ;
				if ( ttmp == "D" ) 
					tmppuzzle.dir = 1 ;
				vecpuzzle.push_back(tmppuzzle) ;
			}

			cnt++ ;
		}
		vecvecpuzzle.push_back(vecpuzzle) ;
	}
	/*for ( int i = 0 ; i < vecvecpuzzle.size() ; i++ ){
		for ( int j = 0 ; j < vecvecpuzzle[i].size() ; j++ ){
			cout <<  vecvecpuzzle[i][j].x << " " ;
			cout <<  vecvecpuzzle[i][j].y << " " ;
			cout <<  vecvecpuzzle[i][j].len << " " ;
			cout <<  vecvecpuzzle[i][j].dir << "   " ;
		}
		cout << endl ;
	}*///handle input 
}

void buildconnect( vector<vector<connect>> &map, int puzzlenum){
	for( int i = 0 ; i < vecvecpuzzle[puzzlenum].size() ; i++ ){
		vector<connect> tmpcon ;
		for ( int j = 0 ; j < vecvecpuzzle[puzzlenum].size() ; j++ ){
			if ( i == j || vecvecpuzzle[puzzlenum][j].dir == vecvecpuzzle[puzzlenum][i].dir )continue;
			if ( vecvecpuzzle[puzzlenum][i].dir == 0 && (vecvecpuzzle[puzzlenum][i].x <= vecvecpuzzle[puzzlenum][j].x) && (vecvecpuzzle[puzzlenum][i].x + vecvecpuzzle[puzzlenum][i].len -1) >= vecvecpuzzle[puzzlenum][j].x && \
					vecvecpuzzle[puzzlenum][j].y <= vecvecpuzzle[puzzlenum][i].y && (vecvecpuzzle[puzzlenum][j].y + vecvecpuzzle[puzzlenum][j].len -1) >= vecvecpuzzle[puzzlenum][i].y ){
				//if (puzzlenum == 2 ) cout << vecvecpuzzle[puzzlenum][i].x  << " " << vecvecpuzzle[puzzlenum][j].x << endl ;
				connect tmp ; 
				tmp.my = vecvecpuzzle[puzzlenum][j].x - vecvecpuzzle[puzzlenum][i].x ;
				tmp.you = vecvecpuzzle[puzzlenum][i].y - vecvecpuzzle[puzzlenum][j].y  ;
				tmp.youname = j ;
				tmpcon.push_back(tmp);
			}else if ( vecvecpuzzle[puzzlenum][i].dir == 1 && vecvecpuzzle[puzzlenum][i].y <= vecvecpuzzle[puzzlenum][j].y && (vecvecpuzzle[puzzlenum][i].y + vecvecpuzzle[puzzlenum][i].len -1) >= vecvecpuzzle[puzzlenum][j].y && \
					vecvecpuzzle[puzzlenum][j].x <= vecvecpuzzle[puzzlenum][i].x && (vecvecpuzzle[puzzlenum][j].x + vecvecpuzzle[puzzlenum][j].len -1) >= vecvecpuzzle[puzzlenum][i].x ){
				connect tmp ;
				tmp.you = vecvecpuzzle[puzzlenum][i].x - vecvecpuzzle[puzzlenum][j].x ;
				tmp.my = vecvecpuzzle[puzzlenum][j].y - vecvecpuzzle[puzzlenum][i].y  ;
				tmp.youname = j ;
				tmpcon.push_back(tmp);
			}
		}
		map.push_back(tmpcon);
	}
}



int choosenow( vector<va> list , int cnt, vector<vector<connect>> &map , int puzzlenum){//choose the next varible
	if ( Mode == 1 ){//brute force choose
		vector<int> tmpvecc ;
		for ( int i = 0 ; i < list.size() ; i++ ){
			if ( list[i].num == 0 ){
				tmpvecc.push_back(i);
			}
		}
		return tmpvecc[cnt] ;
	}else if (  Mode == 2 ){	// MRV choose
		vector<p> tmpvecc ;
		for ( int i = 0 ; i < list.size() ; i++ ){
			if ( list[i].num == 0 ){
				p tmpp ;
				tmpp.num = i ;
				tmpp.degree = dict[vecvecpuzzle[puzzlenum][i].len].size();
				tmpvecc.push_back(tmpp);
			}
		}
		sort(tmpvecc.begin(),tmpvecc.end(),cmp);
		return tmpvecc[cnt].num ;
	}else if ( Mode == 3 ){	//degree choose
		vector<p> tmpvecc ;
		
		for ( int i = 0 ; i < list.size() ; i++ ){
			if ( list[i].num == 0 ){
				p tmpp ;
				tmpp.num = i ;
				tmpp.degree = map[i].size() ;
				tmpvecc.push_back(tmpp);
			}
		}
		sort(tmpvecc.begin(),tmpvecc.end(),cmp);
		return tmpvecc[cnt].num ;
	}
}

void printpuzzle( vector<va> sta , int puzzlenum ){
	//cout << "printpuzzle" << sta.size() << endl ;
	string tmpstr("        ");
	vector<string> tmpvecstr ;
	//char a[8][8] ;
	for ( int i = 0 ; i < 8 ; i++ )
		tmpvecstr.push_back(tmpstr);
	for ( int i = 0 ; i < sta.size() ; i++ ){
		for ( int j = 0 ; j < vecvecpuzzle[puzzlenum][sta[i].num].len ; j++ ){
			if ( vecvecpuzzle[puzzlenum][sta[i].num].dir == 0 ){
				tmpvecstr[ vecvecpuzzle[puzzlenum][sta[i].num].x + j ][ vecvecpuzzle[puzzlenum][sta[i].num].y ] = sta[i].word[j] ;
			}else{
				tmpvecstr[ vecvecpuzzle[puzzlenum][sta[i].num].x ][ vecvecpuzzle[puzzlenum][sta[i].num].y + j ] = sta[i].word[j] ;
			}
		}
	}
	for ( int i = 0 ; i < 8 ; i++ ){
		for ( int j = 0 ; j < 8 ; j++ ){
			cout << tmpvecstr[j][i] << " " ;
		}
		cout << endl ;
	}
	
	
	
	//cout << "printpuzzle" << endl ;
}

void ans( vector<va> sta , int puzzlenum ){
	printpuzzle(sta,puzzlenum);
	while( sta.size() != 0 ){
		cout << sta.back().word << " "  ;
		sta.pop_back();
	}
	cout << endl << "node:" << nodecnt << endl ;
}

int go( vector<va> list , vector<va> sta , int puzzlenum , vector<vector<connect>> &map ){
	nodecnt++ ;
	//cout << nodecnt << endl ;
	/*
	if( sta.size() >= 4 ){
		cout << nodecnt << endl ;
		for ( int i = 0 ; i < sta.size() ; i++ ){
			cout << sta[i].num << " " << sta[i].word << endl ;
		}
		printpuzzle(sta,puzzlenum);
		
	}*/
	if ( sta.size() == list.size() ){
			ans(sta,puzzlenum);
			return 1 ;
	}
	for ( int i = 0 ; i < (list.size()-sta.size()) ; i++ ){
		int choose = choosenow( list , i , map , puzzlenum );
		vector<pending> con ;
		//contranints
		for ( int j = 0 ; j < map[choose].size() ; j++ ){
			if ( list[map[choose][j].youname].num == 1 ){
				pending tmppending ;
				tmppending.youname = map[choose][j].youname;
				tmppending.my = map[choose][j].my ;
				tmppending.you = map[choose][j].you ;
				con.push_back(tmppending);
			}
		}
		
		for ( int j = 0 ; j < dict[vecvecpuzzle[puzzlenum][choose].len].size() ; j++ ){
			int flag = 0 ;
			for ( int k = 0 ; k < con.size() ; k++ ){
				if ( dict[vecvecpuzzle[puzzlenum][choose].len][j].c_str()[con[k].my] != list[con[k].youname].word.c_str()[con[k].you] ){
					//cout << dict[vecvecpuzzle[puzzlenum][choose].len][j].c_str()[con[k].my] ;
					flag = 1 ;
					break;
				} 
			}
			if ( flag == 0 ){
				va tmpva ;
				tmpva.num = choose ;
				tmpva.word = dict[vecvecpuzzle[puzzlenum][choose].len][j] ;
				list[choose].num = 1 ;
				list[choose].word = dict[vecvecpuzzle[puzzlenum][choose].len][j] ;
				sta.push_back(tmpva);
				if ( go( list , sta , puzzlenum , map ) ){
					return 1 ;
				}
				list[choose].num = 0 ;
				sta.pop_back();
			}else {
				//cout << "jump\n" ;
			}
		}
	}

	return 0 ;
}

void solve(int puzzlenum){
	vector<vector<connect>> map ;
	buildconnect(map,puzzlenum);
	cout << "buildconnect\n" ;
	vector<va> list;
	list.resize(vecvecpuzzle[puzzlenum].size());
	for ( int i = 0 ; i < list.size() ; i++ ){
		list[i].num = 0 ;	
	}
	vector<va> sta;
	int succ = go(list,sta,puzzlenum,map);
	/*
	for ( int j = 0 ; j < vecvecpuzzle[puzzlenum].size() ; j++ ){
		cout <<  vecvecpuzzle[puzzlenum][j].x << " " ;
		cout <<  vecvecpuzzle[puzzlenum][j].y << " " ;
		cout <<  vecvecpuzzle[puzzlenum][j].len << " " ;
		cout <<  vecvecpuzzle[puzzlenum][j].dir << "   " ;
	}
	cout << endl ;

	for ( int i = 0 ; i < map.size() ; i++ ){
		cout << i << " " <<  map[i].size() << endl ;
		for ( int j = 0 ; j < map[i].size() ; j++ ){
			cout << "      " << map[i][j].my << " " << map[i][j].you << " " << map[i][j].youname << " " << endl ;  
		}
		cout << endl ; 
	}*/


}

int main(){
	
	init();
	
	
	if ( ram == 0 )
		for ( int i = 0 ; i < vecvecpuzzle.size()-1 ; i++ ){
			nodecnt = 0 ;
			solve(i);
		}
	if ( ram == 1 ){
		int avg[3] = {0};
		for ( int k = 0 ; k < 10 ; k++ ){
			srand(unsigned(time(0)));
			for ( int i = 0 ; i < dict.size() ; i++ ){
				random_shuffle(dict[i].begin(),dict[i].end(),myrandom);
			}
			for ( int i = 0 ; i < vecvecpuzzle.size()-1 ; i++ ){
				nodecnt = 0 ;
				solve(i);
				avg[i] += nodecnt ;
			}
		}
		cout << "puzzle 1 : " << avg[0]/10 << endl ;
		cout << "puzzle 2 : " << avg[1]/10 << endl ;
		cout << "puzzle 3 : " << avg[2]/10 << endl ;
	}
}