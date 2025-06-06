# tests/test_linked_list.py

from pydatastructs.linked_list import LinkedList

def test_prepend_and_len():
    ll = LinkedList()
    ll.prepend(1)
    ll.prepend(2)
    assert len(ll) == 2

def test_prepend_order():
    ll = LinkedList()
    ll.prepend(1)
    ll.prepend(2)
    ll.prepend(3)
    assert ll.pop() == 3
    assert ll.pop() == 2
    assert ll.pop() == 1

def test_pop_empty():
    ll = LinkedList()
    assert ll.pop() is None

def test_len_empty():
    ll = LinkedList()
    assert len(ll) == 0

def test_len_after_pop():
    ll = LinkedList()
    ll.prepend(10)
    ll.prepend(20)
    ll.pop()
    assert len(ll) == 1

def test_multiple_pops():
    ll = LinkedList()
    values = [5, 10, 15]
    for v in values:
        ll.prepend(v)
    popped = [ll.pop(), ll.pop(), ll.pop()]
    assert popped == [15, 10, 5]
    assert ll.pop() is None
    assert len(ll) == 0

def test_reuse_after_empty():
    ll = LinkedList()
    ll.prepend(1)
    ll.pop()
    assert len(ll) == 0
    ll.prepend(2)
    assert ll.pop() == 2

def test_peek_non_empty():
    ll = LinkedList()
    ll.prepend(5)
    assert ll.peek() == 5
    assert len(ll) == 1

def test_peek_multiple():
    ll = LinkedList()
    ll.prepend(1)
    ll.prepend(2)
    ll.prepend(3)
    assert ll.peek() == 3
    assert len(ll) == 3

def test_peek_empty():
    ll = LinkedList()
    assert ll.peek() is None

def test_peek_after_pop():
    ll = LinkedList()
    ll.prepend(1)
    ll.pop()
    assert ll.peek() is None