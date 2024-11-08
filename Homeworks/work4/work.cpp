#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

std::vector<std::string> generate_edit_distance_one(const std::string& s, const std::string& v) {
    std::vector<std::string> result;

    // 插入操作
    for (size_t i = 0; i <= s.length(); ++i) {
        for (char c : v) {
            std::string temp = s;
            temp.insert(i, 1, c);
            result.push_back(temp);
        }
    }

    // 删除操作
    for (size_t i = 0; i < s.length(); ++i) {
        std::string temp = s;
        temp.erase(i, 1);
        result.push_back(temp);
    }

    // 替换操作
    for (size_t i = 0; i < s.length(); ++i) {
        for (char c : v) {
            if (s[i] != c) {
                std::string temp = s;
                temp[i] = c;
                result.push_back(temp);
            }
        }
    }

    // 移除原始字符串s，使用unordered_set去重
    std::unordered_set<std::string> unique_result(result.begin(), result.end());
    unique_result.erase(s);

    // 将结果从unordered_set转换回vector
    return std::vector<std::string>(unique_result.begin(), unique_result.end());
}

int main() {
    std::string s = "abc";
    std::string v = "abcde";
    std::vector<std::string> edit_distance_one_strings = generate_edit_distance_one(s, v);

    for (const std::string& str : edit_distance_one_strings) {
        std::cout << str << std::endl;
    }

    return 0;
}