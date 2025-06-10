import pytest
from pydatastructs.monotonic_increasing_stack import MonotonicIncreasingStack

def test_push_single():
    stack = MonotonicIncreasingStack()
    stack.push(10)
    assert len(stack) == 1
    assert stack.top() == 10

def test_push_multiple_increasing():
    stack = MonotonicIncreasingStack()
    stack.push(5)
    stack.push(10)
    stack.push(15)
    assert len(stack) == 3
    assert stack.top() == 15

def test_push_decreasing_rejected():
    stack = MonotonicIncreasingStack()
    stack.push(10)
    stack.push(5)
    assert len(stack) == 1
    assert stack.top() == 5

def test_push_equal_allowed():
    stack = MonotonicIncreasingStack()
    stack.push(10)
    stack.push(10)  # Equal values allowed in "non-strict" increasing stack
    assert len(stack) == 2
    assert stack.top() == 10

def test_pop_single():
    stack = MonotonicIncreasingStack()
    stack.push(20)
    val = stack.pop()
    assert val == 20
    assert len(stack) == 0

def test_pop_multiple():
    stack = MonotonicIncreasingStack()
    stack.push(1)
    stack.push(2)
    stack.push(3)
    assert stack.pop() == 3
    assert stack.top() == 2
    assert len(stack) == 2

def test_pop_until_empty():
    stack = MonotonicIncreasingStack()
    for v in [5, 6, 7]:
        stack.push(v)
    assert stack.pop() == 7
    assert stack.pop() == 6
    assert stack.pop() == 5
    assert len(stack) == 0
    assert stack.top() is None

def test_pop_empty_stack():
    stack = MonotonicIncreasingStack()
    assert stack.pop() is None

def test_top_empty_stack():
    stack = MonotonicIncreasingStack()
    assert stack.top() is None

def test_length_after_operations():
    stack = MonotonicIncreasingStack()
    assert len(stack) == 0
    stack.push(1)
    assert len(stack) == 1
    stack.push(2)
    assert len(stack) == 2
    stack.pop()
    assert len(stack) == 1
    stack.pop()
    assert len(stack) == 0

def test_complex_sequence():
    stack = MonotonicIncreasingStack()
    stack.push(3)
    stack.push(5)
    stack.push(5)
    stack.push(2)
    assert stack.top() == 2
    assert len(stack) == 1
