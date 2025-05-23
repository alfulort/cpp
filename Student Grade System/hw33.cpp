#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <numeric>
#include <algorithm>
using namespace std;

class StudentManager {


public:
    struct Student {
        string name;
        int score;
    };

    vector<Student> students;
private:
    std::unordered_map<std::string, Student*> studentIndex;// TODO5新增

public:
    // TODO 题目1：正确性
    // 解决：封装一个内部不变式：系统中学生的成绩必须始终处于 [0, 100] 之间，且不允许添加重名学生。
    bool isValidScore(int score) const {
        //TODO
        return score >= 0 && score <= 100;
    }

    bool safeAddStudent(const std::string& name, int score) {
        // TODO
        // 1. 检查姓名是否重复
        if (studentIndex.find(name) != studentIndex.end()) {
            return false; // 重名
        }
        // 2. 检查分数是否有效
        if (!isValidScore(score)) {
            return false; // 分数无效
        }
        students.push_back({ name, score });
        studentIndex[name] = &students.back();

        // 验证 studentIndex 是否正确更新
        cout << "Checking studentIndex after adding Alice and Bob:" << endl;
        // for(auto &i:students)
        //     cout<<i.name<<": "<<i.score<<"\n";
        for (const auto& pair : studentIndex) {
            cout << pair.first << ": " << pair.second->score << endl;
        }

        return true;
    }


    // TODO 题目2：健壮性
    // 解决：校验姓名（仅字母）和分数（0-100）
    void addStudent(const string& name, int score) {
        // TDOD:校验姓名
        for (char c : name) {
            if (!isalpha(c)) {
                throw invalid_argument("Invalid name: " + name);
            }
        }

        if (studentIndex.find(name) != studentIndex.end()) {
        throw invalid_argument("Duplicate name: " + name);
    }

        // TODO:校验分数
        if (!isValidScore(score)) {
            throw out_of_range("Invalid score: " + std::to_string(score));
        }
        students.push_back({ name, score }); // 无校验
        studentIndex[name] = &students.back();
    }

    // TODO 题目3：灵活性
    // 解决：动态评分规则，支持灵活评分
    char getGrade(int score,int aBoundary = 90, int bBoundary = 80) const {
        if (score >= aBoundary) return 'A';
        else if (score >= bBoundary) return 'B';
        else return 'C';
    }

    // TODO 题目4：可重用性
    // 解决：修改countHighScorers，调用countIf将过滤逻辑提取为通用函数
    int countHighScorers() {
        // int count = 0;
        // for (const auto& s : students) {
        //     if (s.score >= 90) {
        //         ++count;
        //     }
        // }
        // return count;
         return countIf([](const Student& s) {
            return s.score >= 90;
        });
    }

    // 通用计数工具
    int countIf(std::function<bool(const Student&)> predicate) const {
        // TODO
        return std::count_if(students.begin(), students.end(), predicate);
    }


    // TODO 题目5：高效性
    // 解决：优化查找性能
    const Student* findStudent(const string& name) {
        // for (const auto& s : students) {
        //     if (s.name == name) return &s; // 查找性能：O(n)
        // }
        if (studentIndex.find(name) != studentIndex.end()) {
            return studentIndex[name];
        }
        return nullptr;
    }
};

// 测试用例
int main() {
    vector<int> values = { 1, 2, 3, 4, 5 };
    StudentManager mgr;
    
    try {
        // 测试题目1
        if (!mgr.safeAddStudent("Alice", 90)) {
            cout << "Failed to add invalid student Alice (score 120)" << endl;
        }
        if (!mgr.safeAddStudent("Bob", 92)) {
            cout << "Failed to add Bob" << endl;
        }
        if (!mgr.safeAddStudent("Bob", 88)) {
            cout << "Duplicate name Bob rejected" << endl;
        }


        // 测试题目2
        //mgr.addStudent("Bob", 92);

        // 测试题目3
        cout << "Alice's grade: " << mgr.getGrade(85) << endl;
        // 预计完成题目3后可以通过以下测试↓
        // cout << "Bob's grade (strict): " << mgr.getGrade(92, 95, 85) << endl;

        // 测试题目4
        int highScorers = mgr.countIf([](const StudentManager::Student& s) {
            return s.score >= 90;
            });
        std::cout << "High scorers: " << highScorers << std::endl;

        // 测试题目5
        if (auto student = mgr.findStudent("Alice")) {
            cout << "Found Alice, score: " << student->score << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
