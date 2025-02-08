#include <iostream>
#include <queue>
#include <set>

template<typename T>
class UniqueQueue
{
public:
    bool enqueue(const T &value)
    {
        if (set_.find(value) != set_.end()) {
            return false; // 元素已存在
        }
        queue_.push(value);
        set_.insert(value);
        return true; // 元素成功入队
    }

    bool dequeue(T &value)
    {
        if (queue_.empty()) {
            return false; // 队列为空
        }
        value = queue_.front();
        queue_.pop();
        set_.erase(value);
        return true; // 元素成功出队
    }

    bool isEmpty() const
    {
        return queue_.empty();
    }

private:
    std::queue<T> queue_;
    std::set<T> set_;
};

int main()
{
    UniqueQueue<int> uq;

    uq.enqueue(1);
    uq.enqueue(2);
    uq.enqueue(2); // 尝试插入重复元素
    uq.enqueue(3);
    int value;
    uq.dequeue(value);
    uq.enqueue(4);
    while (!uq.isEmpty()) {
        uq.dequeue(value);
        std::cout << value << std::endl; // 输出队列中的元素
    }

    return 0;
}