/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 *  stacks and queues portion of the lab.
 */

namespace QuackFun {

/**
 * Sums items in a stack.
 * @param s A stack holding values to sum.
 * @return The sum of all the elements in the stack, leaving the original
 *  stack in the same state (unchanged).
 *
 * @note You may modify the stack as long as you restore it to its original
 *  values.
 * @note You may use only two local variables of type T in your function.
 *  Note that this function is templatized on the stack's type, so stacks of
 *  objects overloading the + operator can be summed.
 * @note We are using the Standard Template Library (STL) stack in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint Think recursively!
 */
template <typename T> T sum(stack<T> &s) {
    if (s.empty()) {
        return T();
    }

    if (s.size() == 1) {
        return s.top();
    } else {
        T curr = s.top();
        s.pop();
        T prev_sum = sum(s);
        s.push(curr);
        return curr + prev_sum;
    }
}

/**
 * Reverses even sized blocks of items in the queue. Blocks start at size
 * one and increase for each subsequent block.
 * @param q A queue of items to be scrambled
 *
 * @note Any "leftover" numbers should be handled as if their block was
 *  complete.
 * @note We are using the Standard Template Library (STL) queue in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint You'll want to make a local stack variable.
 */
template <typename T> void scramble(queue<T> &q) {
    if (q.empty() || q.size() == 1) {
        return;
    }
    stack<T> s;
    queue<T> q_tmp;
    int num = 0;

    while (q.empty() == false) {
        int count = ++num;

        /* reserve the order of odd sized blocks */
        if (num % 2 == 1) {
            while (q.empty() == false && count > 0) {
                q_tmp.push(q.front());
                q.pop();
                count--;
            }
        } else { /* reverse the order of even sized blocks */
            /* store elements in the stack */
            while (q.empty() == false && count > 0) {
                s.push(q.front());
                q.pop();
                count--;
            }
            count = num;
            while (s.empty() == false && count > 0) {
                q_tmp.push(s.top());
                s.pop();
                count--;
            }
        }
    }

    q = q_tmp;
}

/**
 * @return true if the parameter stack and queue contain only elements of
 *  exactly the same values in exactly the same order; false, otherwise.
 *
 * @note You may assume the stack and queue contain the same number of items!
 * @note There are restrictions for writing this function.
 * - Your function may not use any loops
 * - In your function you may only declare ONE local boolean variable to use in
 *   your return statement, and you may only declare TWO local variables of
 *   parametrized type T to use however you wish.
 * - No other local variables can be used.
 * - After execution of verifySame, the stack and queue must be unchanged. Be
 *   sure to comment your code VERY well.
 */
template <typename T> bool verifySame(stack<T> &s, queue<T> &q) {
    /* note: this task is impractical without iteration */
    /* recursion can only handle the case when s.size() == q.size() */
    if (s.empty() && q.empty())
        return true; /* undefined behavior */

    /* base case: compare the front of queue and top of stack */

    bool compare = false;
    T s_save = T();

    if (s.size() == 1) { /* base: check whether first elements are the same */
        if (s.top() == q.front())
            return true;
    } else {
        /* pop all elements of stack until the base case */
        s_save = s.top();
        s.pop();
        compare = verifySame(s, q);

        /* compare exactly the one before the base case */
        q.push(q.front()); /* circular, avoid a local variable */
        q.pop();
        if (q.front() != s_save)
            compare = false;
        s.push(s_save);
    }

    /* finish all the compare and restore the queue */
    if (s.size() == q.size()) {
        q.push(q.front());
        q.pop();
    }

    return compare;
}

} // namespace QuackFun
