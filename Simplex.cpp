#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class Simplex{
    private:
        int row_size, col_size;
        vector <vector<float>> matrix;
        // vector<float> rightHandSide;
        vector<float> objective;
        bool isUnbounded;//���B�z
        bool have_many_solution; //���B�z
        bool isMax;
        int ctr;
    public :
        Simplex(vector<vector<float>>&, vector<float>&, bool);
        void compute();
        bool optimality_test();
        int find_col_pivot();
        int find_row_pivot(int);
        void Gaussian_elimination(int, int);
        void print();
        void intit(); // �g�i�Hstack��bigM��!

};
Simplex::Simplex(vector<vector<float>>&matrix ,vector<float> &objective,bool isMax){
            row_size = matrix.size();
            col_size = matrix[0].size();
            this -> matrix = matrix;
            // this -> rightHandSide = b;
            this -> objective = objective;
            isUnbounded = false;
            have_many_solution = false;
            this ->isMax = isMax;
            this->ctr = 1;
}
void Simplex::compute(){ //
    for(auto a:objective)cout <<a<<" ";
    cout <<"\n";
    while(true){
        if (optimality_test()) break; 
        int col_pivot = find_col_pivot();
        int row_pivot = find_row_pivot(col_pivot);
        if(isUnbounded)break;
        Gaussian_elimination(row_pivot,col_pivot);
        //�|���P�_���S��unbound
        ctr++;
    }
    //�h���� p111 �A���N�b���@����N�n
    // �L�X�̲׸� �X���������X�ӸѶܧa?�S���N�g 0
    // �Ӧ� �ߤ@�O�@���Ovariable  
    for(int i = 0; i < row_size ; i++){
        int index ;
        for(int j = 0 ; j < col_size ; j++){
            if(matrix[i][j] == 1.0){
                index = j;
                break;
            }
        }
        cout << "variable" << index+1 << " is "<< matrix[i][col_size-1]<<"\n";
    }
    if(isMax){
        cout << "maximum is " <<objective[col_size-1];
    }else{
        cout << "minimum is " <<objective[col_size-1];
    }
    return; 
}
bool Simplex::optimality_test(){
    //�ؼЭȨt�Ƴ������A�Y����
    for(auto o : objective){
        if(o < 0){
            cout <<"~~~~~~~��"<<ctr<<"�^~~~~~~~~~~~\n";
            return false;
        }
    }
    return true;
}
int Simplex::find_col_pivot(){ //�S��
    //objective���̤p�� == �̤j)
    int pos;
    int min = INT_MAX;
    for(int i = 0; i < objective.size(); i++){
        if(min > objective[i]){
            min = objective[i];
            pos = i;
        }
    }
    return pos;
}
int Simplex::find_row_pivot(int col_pivot){ //bound ????
    //minimum ratio test
    int rhs_pos = col_size-1, pos = 0, minI = INT_MAX,ct=0;
    vector<float>result;
    for(int i = 0 ; i < row_size; i++){
        if(matrix[i][col_pivot]> 0.0){
            // if(matrix[i][col_pivot]== 0.0){
            result.push_back(matrix[i][rhs_pos]/matrix[i][col_pivot]);
            // cout << matrix[i][rhs_pos]/matrix[i][col_pivot] <<" ";
        }else{
            result.push_back(10000);//���n���L
            // cout <<10000<<" ";
            ct++;
        }
    }
    // cout <<endl;
    if(ct==row_size){ //�o�ƪ���L�ȳ��O0�A1��ܳo�ӭȨS�����󪺭���
        isUnbounded = true;
        cout <<"ubound!! This "<<col_pivot<<"\n";
        return 0;
    }
    //��̤p�Ȫ���m
    for(int i = 0 ; i < row_size; i++){
        if(minI > result[i]){
            minI = result[i];
            // cout <<"��ܥثe�̤p��minmin ratio "<< result[i]<<"\n";
            pos = i;
        }
    }
    // cout <<"pos " <<pos <<"\n";
    return pos;
}

void Simplex::Gaussian_elimination(int row_pivot, int col_pivot){
    int pivot = matrix[row_pivot][col_pivot];
    vector<float>key_row;
    for (int i = 0; i < col_size ; i++){
        matrix[row_pivot][i] = matrix[row_pivot][i]/pivot;
        key_row.push_back(matrix[row_pivot][i]/pivot);
    }
    cout <<"row_pivot : "<<row_pivot <<" col_pivot : " <<col_pivot<<"\n";
    cout << "pivot : "<<pivot <<"\n�������h�k�e: \n";
    print();
    //matrix ���h
    for(int i =0 ; i < row_size ; i++){
        if(i != row_pivot){
            float multiply = matrix[i][col_pivot]/pivot;
            for(int j =0 ; j <col_size ; j++){
            // col_pivot�P�ƪ��ȡA�n�ݦh��
                matrix[i][j] = matrix[i][j] - multiply*key_row[j]; //�~�t���D ?
            }
        }
    }
    
    float mult_ob= objective[col_pivot]/pivot;
    // objective �]�n������
    cout <<"objective ������: \nmult_ob:"<<mult_ob<<endl;
    for(int i =0 ; i < col_size ; i++){
        objective[i] = objective[i]-mult_ob*key_row[i];
        cout << objective[i]<<" ";
    }
    cout <<"\n���� ���G: \n";
    print();

}
void Simplex::print(){
    for(int i = 0 ; i < row_size ; i++){
        for(int j = 0 ;j < col_size ; j++){
            cout << matrix[i][j] <<" ";
        }
        cout << "\n";
    }
    cout<<"\n";
}
int main(){
    float n;
    int row ,col;
    bool isMax;
    // cout <<"--------------\n";
    // cout <<"�ڭn�ѨM:\n";
    // cout <<"�Ҧp: \nmax:  z -3*a -2*b \n";
    // cout <<"s.t.";
    // cout << "a<=4;\n2*b<=12;\n3*a+2*b<=18;\n\n";
    // cout << "�п�J: row size is 3 \ncol size is 6\n isMax : 1\nobjective : 3 2 0 0 0 0\n";
    // cout << "matrix:\n1 0 1 0 0 4\n0 1 0 1 0 3\n3 2 0 0 1 18\n\n";
    cout << "row size is ";
    cin >> row;
    cout << "col size is : ";
    cin >> col;
    cout <<"isMax (true =1 / false = 0): ";
    cin >> isMax;
    vector<float> objective(col,0);
    vector<vector<float>> matrix(row,vector<float>(col));
    cout <<"objective:";
    for(int i = 0; i < col ; i++){
        cin >> objective[i];
        // objective.push_back(n);
    }
    cout <<"matrix:\n";
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < col ; j++){
            cin >> matrix[i][j];
        }
    }
    
    Simplex Simplex(matrix,objective,isMax);
    // cout <<"here~\n";
    Simplex.compute();
    return 0;
}
