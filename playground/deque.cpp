#include <iostream>
#include <deque>

using std::cout;

void appendBufferToEndOfDeque(std::deque<int>& deq, const int* buff, size_t n) {
    deq.insert(deq.end(), buff, buff + n);
}

  
int main()
{
    int buffer[] = {1,2,3,4,5,6,7};
    std::deque<int> mydeque;
    mydeque.push_front(3);
    mydeque.push_front(2);
    mydeque.push_front(1);
    //Deque becomes 1, 2, 3

    cout << mydeque[0]<< std::endl;
 
    mydeque.pop_front();
    
    cout << mydeque[0]<< std::endl;
    mydeque.pop_front();
    cout << mydeque[0]<< std::endl;
    //Deque becomes 2, 3

    appendBufferToEndOfDeque(mydeque, buffer, sizeof(buffer)/sizeof(buffer[0]));
  
    for (auto it = mydeque.begin(); it != mydeque.end(); ++it)
        cout << *it << std::endl;

    return 0;
}