//
//  main.cpp
//  Grade_Management_Program
//
//  Created by 세광 on 2021/07/21.
//

#include <iostream>
#include <fstream>

#define MAX 30

using namespace std;

typedef struct Node {
    int ID, mid, fin;
    char name[MAX], grade;
    float avg;
    struct Node *link;
    
    friend class List;
} NODE;

class List {
public:
    List();
    void File_Open();
    void Init_Node(int, char*, int, int);
    void Select_Menu();

    template<typename Type>
    void Show_Data(Type);
    void Search_Data();
    void Change_Score();
    void Search_Grade();
    void Add_Data();
    void Remove_Data();
    void Quit_Program();
    
    NODE *Init_Data(int, char*, int, int);
    template<typename Type>
    bool Read_Data(Type);
    char Set_Grade(int);
    void Bubble_Sort();
    void Delete_All();
    
private:
    NODE *head;
    int total;
};

int main() {
    List *list = new List;
    list->File_Open();
    delete list;
}

List::List() : head(NULL), total(0) {
    cout.setf(ios::fixed);
    cout.precision(1);
}

void List::File_Open() {
    char filename[MAX];
    cout << "Input the score file name: "; cin >> filename;
    
    ifstream fin;
    fin.open(filename);
    if (fin.fail()) {
        cout << "Could not open " << filename << " file" << endl;
        return;
    }
    if (fin.is_open()) {
        char new_firstname[MAX], new_secondname[MAX];
        int new_ID, new_mid, new_fin;
        while (!fin.eof()) {
            fin >> new_ID >> new_firstname >> new_secondname >> new_mid >> new_fin;
            strcat(new_firstname, " ");
            strcat(new_firstname, new_secondname);
            Init_Node(new_ID, new_firstname, new_mid, new_fin);
            total++;
        }
    }
    Bubble_Sort();
    Show_Data(true);
    Select_Menu();
    fin.close();
}

void List::Init_Node(int ID, char *name, int mid, int fin) {
    NODE *curr = head;
    NODE *add = Init_Data(ID, name, mid, fin);
    
    add->link = NULL;
    if (head == NULL) {
        head = add;
    } else {
        while (curr->link != NULL) {
            curr = curr->link;
        }
        curr->link = add;
    }
}

void List::Select_Menu() {
    char select[MAX];
    while (true) {
        cout << "\n# "; cin >> select;
        if (strcasecmp(select, "show") == 0) {
            Show_Data(true);
        } else if (strcasecmp(select, "search") == 0) {
            Search_Data();
        } else if (strcasecmp(select, "changescore") == 0) {
            Change_Score();
        } else if (strcasecmp(select, "searchgrade") == 0) {
            Search_Grade();
        } else if (strcasecmp(select, "add") == 0) {
            Add_Data();
        } else if (strcasecmp(select, "remove") == 0) {
            Remove_Data();
        } else if (strcasecmp(select, "quit") == 0) {
            Quit_Program();
            return;
        } else cout << "WRONG INPUT" << endl;
    }
}

template<typename Type>
void List::Show_Data(Type input) {
    NODE *curr = head;
    if (head == NULL) {
        cout << "LIST IS EMPTY" << endl;
        return;
    }
    cout << "Student\t\tName\t\tMidterm\tFinal\tAverage\tGrade" << endl;
    cout << "-------------------------------------------------------------" << endl;
    while (curr != NULL) {
        if (input == curr->ID || input == curr->grade || input == true) {
            cout << curr->ID << '\t' << curr->name << '\t' << curr->mid << '\t' << curr->fin << '\t' << curr->avg << '\t' << curr->grade << endl;
            if (input == curr->ID) break;
        }
        curr = curr->link;
    }
}

void List::Search_Data() {
    int input;
    cout << "Student ID: "; cin >> input;
    
    if (Read_Data(input) == false) {
        cout << "NO SUCH PERSON" << endl;
        return;
    }
    Show_Data(input);
}

void List::Change_Score() {
    NODE *curr = head;
    int input, score;
    char examtype[MAX];
    cout << "Student ID: "; cin >> input;
    
    if (Read_Data(input) == false) {
        cout << "NO SUCH PERSON" << endl;
        return;
    } else {
        cout << "mid/final?: "; cin >> examtype;
        if (!(strcasecmp(examtype, "mid") == 0 || strcasecmp(examtype, "final") == 0)) {
            cout << "WRONG INPUT" << endl;
            return;
        } else {
            cout << "Input new score: ";
            while (true) {
                cin >> score;
                if (0 <= score && score <= 100) break;
                else cout << "INVALID SCORE - Please retype: ";
            }
        }
    }
    
    Show_Data(input);
    cout << "Score changed" << endl;
    while (curr != NULL) {
        if (input == curr->ID) {
            if (strcasecmp(examtype, "mid") == 0) {
                curr->mid = score;
            } else if (strcasecmp(examtype, "final") == 0) {
                curr->fin = score;
            }
            curr->avg = (curr->mid + curr->fin) / (float)2;
            curr->grade = Set_Grade(curr->avg);
            cout << curr->ID << '\t' << curr->name << '\t' << curr->mid << '\t' << curr->fin << '\t' << curr->avg << '\t' << curr->grade << endl;
            break;
        }
        curr = curr->link;
    }
    Bubble_Sort();
}

void List::Search_Grade() {
    char input;
    cout << "Grade to search: "; cin >> input;
    
    if (input == 'A' || input == 'B' || input == 'C' || input == 'D' || input == 'F') {
        if (Read_Data(input) == false) {
            cout << "NO RESULTS" << endl;
            return;
        }
    } else {
        cout << "WRONG INPUT" << endl;
        return;
    }
    Show_Data(input);
}

void List::Add_Data() {
    int input, new_mid, new_fin;
    char new_firstname[MAX];
    char new_secondname[MAX];
    cout << "Student ID: "; cin >> input;
    
    if(Read_Data(input) == true) {
        cout << "ALREADY EXISTS" << endl;
        return;
    } else {
        cout << "Name: "; cin >> new_firstname >> new_secondname;
        strcat(new_firstname, " ");
        strcat(new_firstname, new_secondname);
        
        cout << "Midterm Score: ";
        while (true) {
            cin >> new_mid;
            if (0 <= new_mid && new_mid <= 100) break;
            else cout << "INVALID SCORE - Please retype: ";
        }
        
        cout << "Final Score: ";
        while (true) {
            cin >> new_fin;
            if (0 <= new_fin && new_fin <= 100) break;
            else cout << "INVALID SCORE - Please retype: ";
        }
    }
    
    cout << "Student added" << endl;
    NODE *add = Init_Data(input, new_firstname, new_mid, new_fin);
    add->link = head;
    head = add;
    total++;
    Bubble_Sort();
}

void List::Remove_Data() {
    int input;
    char y_n;
    cout << "Student ID: "; cin >> input;
    
    if (Read_Data(input) == false) {
        cout << "NO SUCH PERSON" << endl;
        return;
    } else {
        cout << "Remove " << input << " (y/n)? ";
        while (true) {
            cin >> y_n;
            if (y_n == 'y' || y_n == 'n') break;
            else cout << "WRONG INPUT - Please retype: " << endl;
        }
    }
    
    if (y_n == 'y') {
        NODE *curr = head;
        NODE *temp;
        int pos = 0;
        cout << "Student Removed" << endl;
        
        while (curr != NULL) {
            pos++;
            if (input == curr->ID) break;
            curr = curr->link;
        }
        curr = head;

        if (pos == 1) {
            head = curr->link;
            delete(curr);
        } else {
            for (int i = 1; i < pos - 1; i++) {
                curr = curr->link;
            }
            temp = curr->link;
            curr->link = curr->link->link;
            delete(temp);
        }
        total--;
        return;
    } else if (y_n == 'n') return;
}

void List::Quit_Program() {
    NODE *curr = head;
    char y_n;
    cout << "Save data(y/n)? ";
    while (true) {
        cin >> y_n;
        if (y_n == 'y' || y_n == 'n') break;
        else cout << "WRONG INPUT - Please retype: " << endl;
    }
    
    if (y_n == 'y') {
        char filename[MAX];
        cout << "File name: "; cin >> filename;
        
        ofstream fout;
        fout.open(filename);
        while (curr != NULL) {
            fout << curr->ID << '\t' << curr->name << '\t' << curr->mid << '\t' << curr->fin;
            if (curr->link != NULL) fout << '\n';
            curr = curr->link;
        }
        fout.close();
        Delete_All();
    } else if (y_n == 'n') return;
}

NODE* List::Init_Data(int ID, char *name, int mid, int fin) {
    NODE *add = new NODE;
    
    strcpy(add->name, name);
    add->ID = ID;
    add->mid = mid;
    add->fin = fin;
    add->avg = (add->mid + add->fin) / (float)2;
    add->grade = Set_Grade(add->avg);
    return add;
}

template<typename Type>
bool List::Read_Data(Type input) {
    NODE *curr = head;
    bool count = false;
    while (curr != NULL) {
        if (input == curr->ID) {
            count = true;
            break;
        }
        else if (input == curr->grade) count = true;
        curr = curr->link;
    }
    return count;
}

char List::Set_Grade(int avg) {
    if (avg >= 90) return 'A';
    else if (80 <= avg && avg < 90) return 'B';
    else if (70 <= avg && avg < 80) return 'C';
    else if (60 <= avg && avg < 70) return 'D';
    else return 'F';
}

void List::Bubble_Sort() {
    NODE *curr = head;
    NODE *prev = NULL;
    NODE *next = NULL;
    
    if (head == NULL) return;

    for (int i = 0; i < total; i++) {
        while (curr->link != NULL) {
            next = curr->link;
            if (curr->avg < next->avg) {
                if (curr == head) head = curr->link;
                else prev->link = curr->link;
                curr->link = next->link;
                next->link = curr;
            }
            prev = curr;
            curr = next;
        }
        curr = head;
        prev = NULL;
        next = NULL;
    }
}

void List::Delete_All() {
    NODE *curr = head;
    NODE *next = NULL;
    
    while (curr != NULL) {
        next = curr->link;
        delete(curr);
        curr = next;
    }
    head = NULL;
}
