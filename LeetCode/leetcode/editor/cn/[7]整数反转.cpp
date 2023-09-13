//给你一个 32 位的有符号整数 x ，返回将 x 中的数字部分反转后的结果。 
//
// 如果反转后整数超过 32 位的有符号整数的范围 [−2³¹, 231 − 1] ，就返回 0。 
//假设环境不允许存储 64 位整数（有符号或无符号）。
//
// 
//
// 示例 1： 
//
// 
//输入：x = 123
//输出：321
// 
//
// 示例 2： 
//
// 
//输入：x = -123
//输出：-321
// 
//
// 示例 3： 
//
// 
//输入：x = 120
//输出：21
// 
//
// 示例 4： 
//
// 
//输入：x = 0
//输出：0
// 
//
// 
//
// 提示： 
//
// 
// -2³¹ <= x <= 2³¹ - 1 
// 
//
// Related Topics 数学 👍 3905 👎 0

#include <bits/stdc++.h>
using namespace std;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution
{
public:
    /*  int reverse(int x)
      {
          string s = to_string(x);
          int l = 0;
          if (s[0] == '-') {
              l = 1;
          }
          std::reverse(s.begin() + l, s.end());
          string t1 = to_string(INT_MAX);
          string t2 = to_string(INT_MIN);

          if (s[0] == '-') {
              if (s.length() == t2.length() && s > t2) {
                  return 0;
              }
          }
          else {
              if (s.length() == t1.length() && s > t1) {
                  return 0;
              }
          }

          int res = stoi(s);
          return res;
      }*/
    int reverse(int x)
    {
        int res = 0;
        while (x != 0) {
            int t = x % 10;
            if (res < INT_MIN / 10 || res > INT_MAX / 10) {
                return 0;
            }
            res = res * 10 + t;
            x /= 10;
        }
        return res;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
