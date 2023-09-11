//数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。 
//
// 
//
// 示例 1： 
//
// 
//输入：n = 3
//输出：["((()))","(()())","(())()","()(())","()()()"]
// 
//
// 示例 2： 
//
// 
//输入：n = 1
//输出：["()"]
// 
//
// 
//
// 提示： 
//
// 
// 1 <= n <= 8 
// 
//
// Related Topics 字符串 动态规划 回溯 👍 3372 👎 0

#include <bits/stdc++.h>
using namespace std;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution
{
public:
    vector<string> generateParenthesis(int n)
    {
        int l = 0, r = 0;
        vector<string> res;
        string str;
        AddParenthesis(res, str, n, n);
        return res;
    }

    void AddParenthesis(vector<string> &res, string &tmp, int l, int r)
    {
        if (l > r || l < 0) {
            return;
        }
        if (l == 0 && r == 0) {
            res.push_back(tmp);
            return;
        }
        tmp += '(';
        AddParenthesis(res, tmp, l - 1, r);
        tmp.pop_back();
        tmp += ')';
        AddParenthesis(res, tmp, l, r - 1);
        tmp.pop_back();
    }
};
//leetcode submit region end(Prohibit modification and deletion)
