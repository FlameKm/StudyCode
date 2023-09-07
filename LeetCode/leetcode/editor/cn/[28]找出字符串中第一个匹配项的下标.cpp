//给你两个字符串 haystack 和 needle ，请你在 haystack 字符串中找出 needle 字符串的第一个匹配项的下标（下标从 0 开始）。
//如果 needle 不是 haystack 的一部分，则返回 -1 。 
//
// 
//
// 示例 1： 
//
// 
//输入：haystack = "sadbutsad", needle = "sad"
//输出：0
//解释："sad" 在下标 0 和 6 处匹配。
//第一个匹配项的下标是 0 ，所以返回 0 。
// 
//
// 示例 2： 
//
// 
//输入：haystack = "leetcode", needle = "leeto"
//输出：-1
//解释："leeto" 没有在 "leetcode" 中出现，所以返回 -1 。
// 
//
// 
//
// 提示： 
//
// 
// 1 <= haystack.length, needle.length <= 10⁴ 
// haystack 和 needle 仅由小写英文字符组成 
// 
//
// Related Topics 双指针 字符串 字符串匹配 👍 1999 👎 0

#include <bits/stdc++.h>
using namespace std;

//leetcode submit region begin(Prohibit modification and deletion)
class Solution
{
public:
    int strStr(string haystack, string needle)
    {
        if (needle.size() > haystack.size()) {
            return -1;
        }
        int cnt = 0, head = 0, head2 = 0;
        bool cnt_start = false;
        for (int i = 0; i < haystack.size(); ++i) {
            if (cnt != 0 && haystack[i] == needle[0] && !cnt_start) {
                head2 = i;
                cnt_start = true;
            }
            if (haystack[i] == needle[cnt]) {
                cnt++;
                if (cnt == needle.size()) {
                    return head;
                }
            }
            else {
                if (head == head2) {
                    head2 = i + 1;
                }
                else {
                    i = head2 - 1;
                }
                cnt_start = false;
                cnt = 0;
                head = head2;
            }
        }
        return -1;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
int main()
{
    string s1 = "mississippi";
    string s2 = "pi";
    Solution solution;
    int a = solution.strStr(s1, s2);
    printf("%d", a);
}